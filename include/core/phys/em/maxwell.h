/* maxwell.h - Maxwell's equations and electromagnetic field API for HOLOS
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

#ifndef HOLOS_MAXWELL_H
#define HOLOS_MAXWELL_H

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <stddef.h>

/* Structure for electromagnetic field at a point */
typedef struct {
    gsl_vector *E;   /* Electric field [V/m] */
    gsl_vector *B;   /* Magnetic field [T] */
    gsl_vector *D;   /* Electric displacement [C/m^2] */
    gsl_vector *H;   /* Magnetic field intensity [A/m] */
    double rho;      /* Charge density [C/m^3] */
    gsl_vector *J;   /* Current density [A/m^2] */
    double t;        /* Time [s] */
    double epsilon;  /* Permittivity [F/m] */
    double mu;       /* Permeability [H/m] */
    double sigma;    /* Conductivity [S/m] */
    int has_source;  /* 1 if source present */
    gsl_vector *source_E; /* External E source */
    gsl_vector *source_B; /* External B source */
    char *material;  /* Material label */
    char *tag;       /* Optional label */
    void *userdata;  /* User-defined pointer */
} maxwell_field_point;

/* Structure for a grid of field points */
typedef struct {
    size_t n;                   /* Number of points */
    maxwell_field_point *pts;   /* Array of field points */
    char *name;
    double t;                   /* Current simulation time */
    int dim;                    /* Grid dimension (1D, 2D, 3D) */
    gsl_matrix *coords;         /* Coordinates of points (n x dim) */
    int has_boundaries;         /* 1 if boundaries present */
    void *boundaries;           /* Pointer to boundary structure */
    void *materials;            /* Pointer to material map/structure */
    void *sources;              /* Pointer to source definitions */
    void *userdata;
} maxwell_field_grid;
/* Structure for boundary condition (Dirichlet, Neumann, periodic, etc.) */
typedef struct {
    char *type;         /* "dirichlet", "neumann", "periodic", etc. */
    size_t *indices;    /* Indices of affected points */
    size_t n_indices;
    gsl_vector *value;  /* Value for Dirichlet/Neumann */
    void *userdata;
} maxwell_boundary;

/* Structure for material properties */
typedef struct {
    char *name;
    double epsilon;
    double mu;
    double sigma;
    void *userdata;
} maxwell_material;

/* Structure for a time-dependent source (current, field, etc.) */
typedef double (*maxwell_source_func)(double t, void *userdata);
typedef struct {
    char *type;            /* "current", "field", etc. */
    size_t *indices;       /* Indices of affected points */
    size_t n_indices;
    maxwell_source_func func;
    void *userdata;
    char *label;
} maxwell_source;
/* ---- Grid/Field Utilities ---- */

maxwell_field_point *maxwell_field_point_clone(const maxwell_field_point *src);
maxwell_field_grid *maxwell_field_grid_clone(const maxwell_field_grid *src);
int maxwell_field_grid_add_point(maxwell_field_grid *grid, const maxwell_field_point *pt);
int maxwell_field_grid_remove_point(maxwell_field_grid *grid, size_t idx);
maxwell_field_point *maxwell_field_grid_find_point(maxwell_field_grid *grid, const char *tag);
int maxwell_field_grid_add_material(maxwell_field_grid *grid, const maxwell_material *mat);
int maxwell_field_grid_remove_material(maxwell_field_grid *grid, const char *name);
maxwell_material *maxwell_field_grid_find_material(maxwell_field_grid *grid, const char *name);
int maxwell_field_grid_add_source(maxwell_field_grid *grid, const maxwell_source *src);
int maxwell_field_grid_remove_source(maxwell_field_grid *grid, const char *label);
maxwell_source *maxwell_field_grid_find_source(maxwell_field_grid *grid, const char *label);
int maxwell_field_grid_add_boundary(maxwell_field_grid *grid, const maxwell_boundary *bnd);
int maxwell_field_grid_remove_boundary(maxwell_field_grid *grid, const char *type);
maxwell_boundary *maxwell_field_grid_find_boundary(maxwell_field_grid *grid, const char *type);
/* ---- Diagnostics and Energy ---- */

void maxwell_poynting_vector(const maxwell_field_point *pt, gsl_vector *S);
void maxwell_grid_poynting_vector(const maxwell_field_grid *grid, size_t idx, gsl_vector *S);
double maxwell_energy_density(const maxwell_field_point *pt);
double maxwell_grid_energy_density(const maxwell_field_grid *grid, size_t idx);
double maxwell_grid_total_energy(const maxwell_field_grid *grid);
/* ---- Special Cases and Field Operations ---- */

void maxwell_static_fields(const maxwell_field_grid *grid, gsl_vector **E_static, gsl_vector **B_static);
void maxwell_plane_wave(const gsl_vector *k, double omega, double t, gsl_vector *E0, gsl_vector *B0, gsl_vector *E, gsl_vector *B);
void maxwell_apply_boundary_conditions(maxwell_field_grid *grid);

/* Compute divergence of E: div(E) = rho/epsilon0 */
double maxwell_div_E(const maxwell_field_point *pt, double epsilon0);

/* Compute divergence of B: div(B) = 0 */
double maxwell_div_B(const maxwell_field_point *pt);

/* Compute curl of E: curl(E) = -dB/dt */
void maxwell_curl_E(const maxwell_field_point *pt_now, const maxwell_field_point *pt_prev, double dt, gsl_vector *curlE);

/* Compute curl of B: curl(B) = mu0 J + mu0 epsilon0 dE/dt */
void maxwell_curl_B(const maxwell_field_point *pt_now, const maxwell_field_point *pt_prev, double mu0, double epsilon0, double dt, gsl_vector *curlB);

/* Update E and B fields at a point using finite difference (FDTD step) */
void maxwell_fdtd_step(maxwell_field_point *pt, const maxwell_field_point *pt_prev, double mu0, double epsilon0, double dt);

/* Utility: allocate/free field point and grid */
maxwell_field_point *maxwell_field_point_alloc(void);
void maxwell_field_point_free(maxwell_field_point *pt);
maxwell_field_grid *maxwell_field_grid_alloc(size_t n);
void maxwell_field_grid_free(maxwell_field_grid *grid);

/* Print field point/grid to file or stdout */
void maxwell_field_point_print(const maxwell_field_point *pt, FILE *f);
void maxwell_field_grid_print(const maxwell_field_grid *grid, FILE *f);

#endif /* HOLOS_MAXWELL_H */