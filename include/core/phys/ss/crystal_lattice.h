/*
   crystal_lattice.h - Crystal lattice module for HOLOS (Solid State Physics)

   This header provides data structures and utilities for modeling crystal lattices in solids,
   including geometry, symmetry, atomic basis, unit cell, reciprocal lattice, diagnostics, and simulation steps.

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

#ifndef HOLOS_CRYSTAL_LATTICE_H
#define HOLOS_CRYSTAL_LATTICE_H

#include <stddef.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/* Crystal lattice geometry structure, supporting lattice type, centering, and Wyckoff positions. */
typedef struct {
	char name[32];
	size_t dim;                          /* Dimensionality (1D, 2D, 3D) */
	gsl_matrix *vectors;                 /* Lattice vectors (dim x dim) */
	double *parameters;                  /* Lattice parameters (a, b, c, alpha, beta, gamma) */
	size_t n_parameters;
	char *symmetry;                      /* Space group or point group */
	char *lattice_type;                  /* Bravais lattice type (e.g., cubic, hexagonal) */
	char *centering;                     /* Centering type (P, I, F, C, R) */
	double **wyckoff_positions;          /* Array of Wyckoff positions */
	size_t n_wyckoff;
	void *extra;
} cl_geometry_t;

/* Atomic basis structure, supporting atomic numbers, masses, and charges. */
typedef struct {
	char **species;                      /* Array of atomic species */
	int *atomic_numbers;                 /* Array of atomic numbers */
	double *masses;                      /* Array of atomic masses (u) */
	double *charges;                     /* Array of atomic charges (e) */
	gsl_matrix *positions;               /* Atomic positions in unit cell (n_atoms x dim) */
	size_t n_atoms;
	void *extra;
} cl_basis_t;

/* Unit cell structure, supporting supercell and defect information. */
typedef struct {
	cl_geometry_t *geometry;
	cl_basis_t *basis;
	int *supercell;                        /* Supercell replication (nx, ny, nz) */
	int *defect_indices;                    /* Indices of defect sites */
	size_t n_defects;
	void *extra;
} cl_unit_cell_t;

/* Reciprocal lattice structure, supporting symmetry and centering. */
typedef struct {
	gsl_matrix *vectors;                 /* Reciprocal lattice vectors (dim x dim) */
	char *symmetry;                      /* Reciprocal space symmetry */
	char *centering;                     /* Centering type */
	void *extra;
} cl_reciprocal_t;

/* Crystal lattice diagnostics, supporting neighbor lists and advanced geometry. */
typedef struct {
	double volume;                       /* Unit cell volume */
	double *distances;                   /* Array of interatomic distances */
	size_t n_distances;
	double *angles;                      /* Array of bond angles */
	size_t n_angles;
	int **neighbor_list;                 /* Neighbor list for each atom */
	size_t *n_neighbors;
	void *extra;
} cl_diagnostics_t;

/* Crystal lattice system, supporting batch operations and extensibility. */
typedef struct {
	cl_unit_cell_t *unit_cell;
	cl_reciprocal_t *reciprocal;
	cl_diagnostics_t diagnostics;
	char **materials;                    /* Array of material/compound names */
	size_t n_materials;
	void *extra;
} cl_system_t;

/* Allocation and management utilities, including batch operations. */
int cl_basis_batch_alloc(cl_basis_t **bases, size_t n, size_t dim, const char ***species, const int **atomic_numbers, const double **masses, const double **charges, const double **positions, const size_t *n_atoms);
void cl_basis_batch_free(cl_basis_t **bases, size_t n);
cl_geometry_t *cl_geometry_alloc(const char *name, size_t dim);
void cl_geometry_free(cl_geometry_t *geom);
cl_basis_t *cl_basis_alloc(size_t n_atoms, size_t dim);
void cl_basis_free(cl_basis_t *basis);
cl_unit_cell_t *cl_unit_cell_alloc(cl_geometry_t *geometry, cl_basis_t *basis);
void cl_unit_cell_free(cl_unit_cell_t *cell);
cl_reciprocal_t *cl_reciprocal_alloc(size_t dim);
void cl_reciprocal_free(cl_reciprocal_t *recip);
cl_system_t *cl_system_alloc(void);
void cl_system_free(cl_system_t *system);

/* Diagnostics and I/O utilities, including symmetry and neighbor list output. */
void cl_symmetry_print(const char *symmetry);
void cl_neighbor_list_print(const int **neighbor_list, const size_t *n_neighbors, size_t n_atoms);
void cl_diagnostics_update(cl_system_t *system);
void cl_system_print(const cl_system_t *system);
void cl_geometry_print(const cl_geometry_t *geom);
void cl_basis_print(const cl_basis_t *basis);
void cl_unit_cell_print(const cl_unit_cell_t *cell);

/* Simulation step utilities, including defect and supercell operations. */
int cl_step_defect(cl_system_t *system, int defect_index);
int cl_step_relaxation(cl_system_t *system, double tol, int max_iter);
int cl_step_supercell(cl_system_t *system, size_t nx, size_t ny, size_t nz);

/* Special-case solutions and utilities, including Wyckoff and advanced geometry calculations. */
double *cl_wyckoff_position(const cl_geometry_t *geom, size_t index);
double cl_unit_cell_volume(const cl_geometry_t *geom);
double cl_distance(const gsl_vector *a, const gsl_vector *b);
double cl_angle(const gsl_vector *a, const gsl_vector *b, const gsl_vector *c);

#endif /* HOLOS_CRYSTAL_LATTICE_H */