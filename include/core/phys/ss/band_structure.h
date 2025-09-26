/*
   band_structure.h - Band structure module for HOLOS (Solid State Physics)

   This header provides data structures and utilities for modeling and calculating
   electronic band structures in solids, including lattices, reciprocal space,
   bands, k-points, eigenvalues, diagnostics, and simulation steps.

   Copyright (C) 2025 HOLOS Scientific Computing Project

   This file is part of HOLOS.

   HOLOS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   HOLOS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with HOLOS; if not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HOLOS_BAND_STRUCTURE_H
#define HOLOS_BAND_STRUCTURE_H

#include <stddef.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/* Lattice structure for solid state systems, supporting symmetry and atomic basis. */
typedef struct {
	char name[32];
	size_t dim;                      /* Dimensionality (1D, 2D, 3D) */
	gsl_matrix *vectors;             /* Lattice vectors (dim x dim) */
	double *parameters;              /* Lattice parameters (a, b, c, alpha, beta, gamma) */
	size_t n_parameters;
	char *symmetry;                  /* Space group or point group */
	char **atomic_basis;             /* Array of atomic species */
	size_t n_basis;
	void *extra;
} bs_lattice_t;

/* Reciprocal lattice structure, supporting symmetry. */
typedef struct {
	gsl_matrix *vectors;             /* Reciprocal lattice vectors (dim x dim) */
	char *symmetry;                  /* Reciprocal space symmetry */
	void *extra;
} bs_reciprocal_t;

/* k-point structure, supporting high-symmetry paths and labels. */
typedef struct {
	gsl_vector *kvec;                /* k-point vector */
	double weight;                   /* Weight in Brillouin zone integration */
	char label[8];                   /* High-symmetry label (e.g., 9, M, K) */
	int path_index;                  /* Path segment index */
	void *extra;
} bs_kpoint_t;

/* Band structure eigenvalue at a k-point, supporting spin and orbital indices. */
typedef struct {
	double energy;                   /* Eigenvalue (eV) */
	int band_index;                  /* Band index */
	int spin_index;                  /* Spin index (0=up, 1=down, etc.) */
	int orbital_index;               /* Orbital index (if relevant) */
	bs_kpoint_t *kpoint;             /* Associated k-point */
	void *extra;
} bs_eigenvalue_t;

/* Band structure band, supporting spin/orbital resolved bands. */
typedef struct {
	int index;                       /* Band index */
	int spin_index;                  /* Spin index */
	int orbital_index;               /* Orbital index */
	double *energies;                /* Array of energies at k-points */
	size_t n_kpoints;
	void *extra;
} bs_band_t;

/* Band structure diagnostics, supporting Fermi surface and advanced DOS. */
typedef struct {
	double band_gap;                 /* Band gap (eV) */
	double fermi_energy;             /* Fermi energy (eV) */
	double *density_of_states;       /* Array of DOS values */
	size_t n_dos;
	double *fermi_surface;           /* Fermi surface data */
	size_t n_fermi_surface;
	double *velocity;                /* Group velocity at k-points */
	size_t n_velocity;
	void *extra;
} bs_diagnostics_t;

/* Band structure system, supporting batch operations and extensibility. */
typedef struct {
	bs_lattice_t *lattice;
	bs_reciprocal_t *reciprocal;
	bs_kpoint_t **kpoints;
	size_t n_kpoints;
	bs_band_t **bands;
	size_t n_bands;
	bs_eigenvalue_t **eigenvalues;
	size_t n_eigenvalues;
	char **materials;                   /* Array of material/compound names */
	size_t n_materials;
	bs_diagnostics_t diagnostics;
	void *extra;
} bs_system_t;

/* Allocation and management utilities, including batch operations. */
int bs_kpoint_batch_alloc(bs_kpoint_t **kpoints, size_t n, size_t dim, const double *kvecs, const double *weights, const char **labels, const int *path_indices);
void bs_kpoint_batch_free(bs_kpoint_t **kpoints, size_t n);
bs_lattice_t *bs_lattice_alloc(const char *name, size_t dim);
void bs_lattice_free(bs_lattice_t *lattice);
bs_reciprocal_t *bs_reciprocal_alloc(size_t dim);
void bs_reciprocal_free(bs_reciprocal_t *recip);
bs_kpoint_t *bs_kpoint_alloc(size_t dim);
void bs_kpoint_free(bs_kpoint_t *kpt);
bs_band_t *bs_band_alloc(int index, size_t n_kpoints);
void bs_band_free(bs_band_t *band);
bs_eigenvalue_t *bs_eigenvalue_alloc(int band_index, bs_kpoint_t *kpoint, double energy);
void bs_eigenvalue_free(bs_eigenvalue_t *eig);
bs_system_t *bs_system_alloc(size_t n_kpoints, size_t n_bands, size_t n_eigenvalues);
void bs_system_free(bs_system_t *system);

/* Diagnostics and I/O utilities, including DOS, Fermi surface, and velocity output. */
void bs_dos_print(const double *dos, size_t n);
void bs_fermi_surface_print(const double *fs, size_t n);
void bs_velocity_print(const double *velocity, size_t n);
void bs_diagnostics_update(bs_system_t *system);
void bs_system_print(const bs_system_t *system);
void bs_lattice_print(const bs_lattice_t *lattice);
void bs_band_print(const bs_band_t *band);
void bs_eigenvalue_print(const bs_eigenvalue_t *eig);

/* Simulation step utilities, including Fermi surface and velocity calculations. */
int bs_step_fermi_surface(bs_system_t *system);
int bs_step_velocity(bs_system_t *system);
int bs_step_scf(bs_system_t *system, double tol, int max_iter);
int bs_step_band_calculation(bs_system_t *system);

/* Special-case solutions and utilities, including advanced band gap and Fermi surface calculations. */
double bs_average_velocity(const bs_system_t *system);
double bs_fermi_surface_area(const bs_system_t *system);
double bs_find_band_gap(const bs_system_t *system);
double bs_fermi_energy(const bs_system_t *system);

#endif /* HOLOS_BAND_STRUCTURE_H */
