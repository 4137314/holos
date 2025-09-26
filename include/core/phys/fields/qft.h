/*
	qft.h - Quantum Field Theory (QFT) module for HOLOS

	This header provides a modular, extensible foundation for QFT simulation and analysis.
	It supports scalar, spinor, and gauge fields, particles, interactions, Lagrangians,
	Feynman diagrams, renormalization, boundary conditions, sources, diagnostics, and
	advanced utilities for scientific computing.

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

#ifndef HOLOS_QFT_H
#define HOLOS_QFT_H

#include <stddef.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/* Field types for QFT fields. */
typedef enum {
	QFT_FIELD_SCALAR,
	QFT_FIELD_SPINOR,
	QFT_FIELD_VECTOR,
	QFT_FIELD_GAUGE,
	QFT_FIELD_TENSOR
} qft_field_type_t;

/* Particle types for QFT particles. */
typedef enum {
	QFT_PARTICLE_FERMION,
	QFT_PARTICLE_BOSON,
	QFT_PARTICLE_GAUGE_BOSON,
	QFT_PARTICLE_SCALAR_BOSON,
	QFT_PARTICLE_GHOST
} qft_particle_type_t;

/* Structure for a quantum field. */
typedef struct {
	qft_field_type_t type;
	char name[32];
	size_t dim; /* spacetime dimension */
	gsl_vector *values; /* field values at grid points */
	gsl_matrix *derivatives; /* field derivatives */
	double mass;
	double charge;
	void *extra; /* extensibility */
} qft_field_t;

/* Structure for a quantum particle. */
typedef struct {
	qft_particle_type_t type;
	char name[32];
	double mass;
	double charge;
	double spin;
	gsl_vector *momentum;
	gsl_vector *position;
	void *extra;
} qft_particle_t;

/* Structure for an interaction vertex. */
typedef struct {
	char label[32];
	size_t n_fields;
	qft_field_t **fields;
	double coupling;
	void *extra;
} qft_interaction_t;

/* Structure for a Lagrangian density. */
typedef struct {
	char name[32];
	double (*density)(const qft_field_t *fields, size_t n_fields, double t, double *x, void *params);
	void *params;
} qft_lagrangian_t;

/* Structure for a Feynman diagram. */
typedef struct {
	char label[32];
	size_t n_vertices;
	qft_interaction_t **vertices;
	size_t n_external;
	qft_particle_t **external_particles;
	void *extra;
} qft_feynman_diagram_t;

/* Structure for renormalization group data. */
typedef struct {
	double scale;
	double (*beta_function)(double g, double mu, void *params);
	void *params;
} qft_renormalization_t;

/* Boundary condition types and structure. */
typedef enum {
	QFT_BC_NONE,
	QFT_BC_DIRICHLET,
	QFT_BC_NEUMANN,
	QFT_BC_PERIODIC,
	QFT_BC_MIXED
} qft_boundary_type_t;

typedef struct {
	qft_boundary_type_t type;
	double value;
	size_t dim;
	void *extra;
} qft_boundary_t;

/* Structure for a source term. */
typedef struct {
	char label[32];
	double (*source_fn)(double t, double *x, void *params);
	void *params;
} qft_source_t;

/* Structure for QFT diagnostics. */
typedef struct {
	double total_energy;
	double total_charge;
	double lagrangian_value;
	double hamiltonian_value;
	double *correlators; /* pointer to array of correlators */
	size_t n_correlators;
	void *extra;
} qft_diagnostics_t;

/* Structure for a QFT system. */
typedef struct {
	qft_field_t **fields;
	size_t n_fields;
	qft_particle_t **particles;
	size_t n_particles;
	qft_interaction_t **interactions;
	size_t n_interactions;
	qft_lagrangian_t *lagrangian;
	qft_feynman_diagram_t **diagrams;
	size_t n_diagrams;
	qft_renormalization_t *renormalization;
	qft_boundary_t **boundaries;
	size_t n_boundaries;
	qft_source_t **sources;
	size_t n_sources;
	qft_diagnostics_t diagnostics;
	void *extra;
} qft_system_t;

/* Allocation and management utilities. */
qft_field_t *qft_field_alloc(qft_field_type_t type, const char *name, size_t dim);
void qft_field_free(qft_field_t *field);
qft_particle_t *qft_particle_alloc(qft_particle_type_t type, const char *name);
void qft_particle_free(qft_particle_t *particle);
qft_interaction_t *qft_interaction_alloc(const char *label, size_t n_fields);
void qft_interaction_free(qft_interaction_t *interaction);
qft_lagrangian_t *qft_lagrangian_alloc(const char *name, double (*density)(const qft_field_t *, size_t, double, double *, void *), void *params);
void qft_lagrangian_free(qft_lagrangian_t *lagrangian);
qft_feynman_diagram_t *qft_feynman_diagram_alloc(const char *label, size_t n_vertices, size_t n_external);
void qft_feynman_diagram_free(qft_feynman_diagram_t *diagram);
qft_renormalization_t *qft_renormalization_alloc(double scale, double (*beta_function)(double, double, void *), void *params);
void qft_renormalization_free(qft_renormalization_t *renorm);
qft_boundary_t *qft_boundary_alloc(qft_boundary_type_t type, double value, size_t dim);
void qft_boundary_free(qft_boundary_t *boundary);
qft_source_t *qft_source_alloc(const char *label, double (*source_fn)(double, double *, void *), void *params);
void qft_source_free(qft_source_t *source);
qft_system_t *qft_system_alloc(size_t n_fields, size_t n_particles, size_t n_interactions, size_t n_diagrams, size_t n_boundaries, size_t n_sources);
void qft_system_free(qft_system_t *system);

/* Diagnostics and I/O utilities. */
void qft_diagnostics_update(qft_system_t *system);
void qft_system_print(const qft_system_t *system);
void qft_field_print(const qft_field_t *field);
void qft_particle_print(const qft_particle_t *particle);
void qft_interaction_print(const qft_interaction_t *interaction);
void qft_lagrangian_print(const qft_lagrangian_t *lagrangian);
void qft_feynman_diagram_print(const qft_feynman_diagram_t *diagram);

/* Simulation step utilities. */
int qft_step_euler(qft_system_t *system, double dt);
int qft_step_runge_kutta(qft_system_t *system, double dt);
int qft_step_path_integral(qft_system_t *system, double dt);

/* Special-case solutions and utilities. */
double qft_two_point_correlator(const qft_field_t *field, double t1, double *x1, double t2, double *x2);
double qft_propagator(const qft_field_t *field, double t1, double *x1, double t2, double *x2);

#endif /* HOLOS_QFT_H */