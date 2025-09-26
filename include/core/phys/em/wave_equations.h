/* wave_equations.h - Wave equations and propagation API for HOLOS
   Copyright (C) 2025 4137314

   This file is part of HOLOS.

   HOLOS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   HOLOS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with HOLOS.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HOLOS_WAVE_EQUATIONS_H
#define HOLOS_WAVE_EQUATIONS_H

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <stddef.h>

/* Structure for a wave field at a point (scalar or vector) */
typedef struct {
	double u;              /* Scalar field (e.g. pressure, E_z, etc.) */
	gsl_vector *U;         /* Vector field (e.g. E, B, displacement) */
	double t;              /* Time [s] */
	double c;              /* Wave speed [m/s] */
	double rho;            /* Density (for acoustic/mechanical waves) */
	double epsilon;        /* Permittivity (EM) */
	double mu;             /* Permeability (EM) */
	double sigma;          /* Conductivity (EM) */
	char *type;            /* "em", "acoustic", "elastic", etc. */
	char *tag;             /* Optional label */
	void *userdata;        /* User-defined pointer */
} wave_field_point;

/* Structure for a grid of wave field points */
typedef struct {
	size_t n;                  /* Number of points */
	wave_field_point *pts;     /* Array of field points */
	char *name;
	double t;                  /* Current simulation time */
	int dim;                   /* Grid dimension (1D, 2D, 3D) */
	gsl_matrix *coords;        /* Coordinates of points (n x dim) */
	int has_boundaries;        /* 1 if boundaries present */
	void *boundaries;          /* Pointer to boundary structure */
	void *sources;             /* Pointer to source definitions */
	void *userdata;
} wave_field_grid;

/* Structure for boundary condition (Dirichlet, Neumann, periodic, etc.) */
typedef struct {
	char *type;         /* "dirichlet", "neumann", "periodic", etc. */
	size_t *indices;    /* Indices of affected points */
	size_t n_indices;
	double value;       /* Value for Dirichlet/Neumann */
	void *userdata;
} wave_boundary;

/* Structure for a time-dependent source */
typedef double (*wave_source_func)(double t, void *userdata);
typedef struct {
	char *type;            /* "point", "plane", etc. */
	size_t *indices;       /* Indices of affected points */
	size_t n_indices;
	wave_source_func func;
	void *userdata;
	char *label;
} wave_source;

/* ---- Wave Equation Operations ---- */

/* Compute Laplacian of scalar field at a point (finite difference) */
double wave_laplacian(const wave_field_grid *grid, size_t idx);

/* Compute time derivative (finite difference) */
double wave_time_derivative(const wave_field_point *pt_now, const wave_field_point *pt_prev, double dt);

/* Update field at a point using finite difference (FDTD step) */
void wave_fdtd_step(wave_field_point *pt, const wave_field_point *pt_prev, double c, double dt, double laplacian);

/* Update all points in a grid (FDTD step) */
void wave_grid_fdtd_step(wave_field_grid *grid, const wave_field_grid *grid_prev, double c, double dt);

/* Special cases: standing wave, traveling wave, Gaussian pulse */
double wave_standing(double A, double k, double omega, double x, double t, double phi);
double wave_traveling(double A, double k, double omega, double x, double t, double phi);
double wave_gaussian_pulse(double A, double x0, double sigma, double x, double t, double c);

/* ---- Diagnostics and Energy ---- */

/* Compute energy density at a point: u = 0.5 * rho * (du/dt)^2 + 0.5 * T * (grad u)^2 */
double wave_energy_density(const wave_field_point *pt, double dudt, double grad_u_sq, double rho, double T);

/* Compute total energy in the grid */
double wave_grid_total_energy(const wave_field_grid *grid);

/* ---- Utilities ---- */

wave_field_point *wave_field_point_alloc(void);
void wave_field_point_free(wave_field_point *pt);
wave_field_point *wave_field_point_clone(const wave_field_point *src);
wave_field_grid *wave_field_grid_alloc(size_t n);
void wave_field_grid_free(wave_field_grid *grid);
wave_field_grid *wave_field_grid_clone(const wave_field_grid *src);
int wave_field_grid_add_point(wave_field_grid *grid, const wave_field_point *pt);
int wave_field_grid_remove_point(wave_field_grid *grid, size_t idx);
wave_field_point *wave_field_grid_find_point(wave_field_grid *grid, const char *tag);
int wave_field_grid_add_source(wave_field_grid *grid, const wave_source *src);
int wave_field_grid_remove_source(wave_field_grid *grid, const char *label);
wave_source *wave_field_grid_find_source(wave_field_grid *grid, const char *label);
int wave_field_grid_add_boundary(wave_field_grid *grid, const wave_boundary *bnd);
int wave_field_grid_remove_boundary(wave_field_grid *grid, const char *type);
wave_boundary *wave_field_grid_find_boundary(wave_field_grid *grid, const char *type);
void wave_field_point_print(const wave_field_point *pt, FILE *f);
void wave_field_grid_print(const wave_field_grid *grid, FILE *f);
int wave_field_grid_save(const wave_field_grid *grid, const char *filename);
wave_field_grid *wave_field_grid_load(const char *filename);

#endif /* HOLOS_WAVE_EQUATIONS_H */