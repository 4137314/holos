/*
   phonons.h - Phonon module for HOLOS (Solid State Physics)

   This header provides data structures and utilities for modeling phonons in solids,
   including phonon modes, dispersion relations, dynamical matrices, eigenvectors,
   diagnostics, and simulation steps.

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

#ifndef HOLOS_PHONONS_H
#define HOLOS_PHONONS_H

#include <stddef.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/* Phonon mode structure, supporting polarization, symmetry, and lifetime. */
typedef struct {
	int branch_index;                    /* Acoustic/optical branch index */
	double frequency;                    /* Phonon frequency (THz) */
	gsl_vector *kvec;                    /* Wavevector in reciprocal space */
	gsl_vector *eigenvector;             /* Eigenvector for mode */
	char *polarization;                  /* Polarization type (longitudinal, transverse) */
	char *symmetry;                      /* Irrep or symmetry label */
	double lifetime;                     /* Phonon lifetime (ps) */
	double group_velocity;               /* Group velocity (km/s) */
	void *extra;
} ph_mode_t;

/* Phonon dispersion relation structure, supporting multiple branches and velocities. */
typedef struct {
	double *frequencies;                 /* Array of frequencies at k-points */
	double *velocities;                  /* Array of group velocities at k-points */
	size_t n_kpoints;
	int branch_index;
	char *branch_type;                   /* Acoustic/optical label */
	void *extra;
} ph_dispersion_t;

/* Dynamical matrix structure, supporting force constants. */
typedef struct {
	gsl_matrix *matrix;                  /* Dynamical matrix (3N x 3N) */
	double *force_constants;             /* Array of force constants */
	size_t n_atoms;
	size_t n_force_constants;
	void *extra;
} ph_dynamical_matrix_t;

/* Phonon diagnostics structure, supporting participation ratio and mean free path. */
typedef struct {
	double *density_of_states;           /* Phonon DOS array */
	size_t n_dos;
	double *group_velocity;              /* Group velocity array */
	size_t n_velocity;
	double *lifetimes;                   /* Phonon lifetimes array */
	size_t n_lifetimes;
	double *participation_ratio;         /* Array of participation ratios */
	size_t n_participation;
	double *mean_free_path;              /* Array of mean free paths */
	size_t n_mfp;
	void *extra;
} ph_diagnostics_t;

/* Phonon system structure, supporting batch operations and extensibility. */
typedef struct {
	ph_mode_t **modes;
	size_t n_modes;
	ph_dispersion_t **dispersions;
	size_t n_dispersions;
	ph_dynamical_matrix_t *dynmat;
	ph_diagnostics_t diagnostics;
	char **materials;                    /* Array of material/compound names */
	size_t n_materials;
	void *extra;
} ph_system_t;

/* Allocation and management utilities, including batch operations. */
ph_mode_t *ph_mode_alloc(int branch_index, size_t dim);
void ph_mode_free(ph_mode_t *mode);
int ph_mode_batch_alloc(ph_mode_t **modes, size_t n, const int *branch_indices, const double *frequencies, const double *kvecs, const char **polarizations, const char **symmetries, const double *lifetimes, const double *group_velocities);
void ph_mode_batch_free(ph_mode_t **modes, size_t n);
ph_dispersion_t *ph_dispersion_alloc(int branch_index, size_t n_kpoints);
void ph_dispersion_free(ph_dispersion_t *disp);
ph_dynamical_matrix_t *ph_dynamical_matrix_alloc(size_t n_atoms);
void ph_dynamical_matrix_free(ph_dynamical_matrix_t *dynmat);
ph_system_t *ph_system_alloc(size_t n_modes, size_t n_dispersions);
void ph_system_free(ph_system_t *system);

/* Diagnostics and I/O utilities, including participation ratio and mean free path output. */
void ph_diagnostics_update(ph_system_t *system);
void ph_system_print(const ph_system_t *system);
void ph_mode_print(const ph_mode_t *mode);
void ph_dispersion_print(const ph_dispersion_t *disp);
void ph_participation_ratio_print(const double *pr, size_t n);
void ph_mean_free_path_print(const double *mfp, size_t n);

/* Simulation step utilities, including group velocity and mean free path calculations. */
int ph_step_dispersion(ph_system_t *system);
int ph_step_lifetime(ph_system_t *system);
int ph_step_group_velocity(ph_system_t *system);
int ph_step_mean_free_path(ph_system_t *system);

/* Special-case solutions and utilities, including advanced averages and participation ratio. */
double ph_average_frequency(const ph_system_t *system);
double ph_average_lifetime(const ph_system_t *system);
double ph_average_mean_free_path(const ph_system_t *system);
double ph_average_participation_ratio(const ph_system_t *system);
double ph_group_velocity(const ph_mode_t *mode);

#endif /* HOLOS_PHONONS_H */