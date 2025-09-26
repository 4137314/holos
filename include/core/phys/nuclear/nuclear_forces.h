/*
   nuclear_forces.h - Nuclear forces module for HOLOS

   This header provides data structures and utilities for modeling nuclear forces,
   including nucleons, potentials, force models, diagnostics, and simulation steps.

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

#ifndef HOLOS_NUCLEAR_FORCES_H
#define HOLOS_NUCLEAR_FORCES_H

#include <stddef.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/* Nucleon structure for nuclear force modeling, supporting extensible properties. */
typedef enum {
	NF_NUCLEON_PROTON,
	NF_NUCLEON_NEUTRON
} nf_nucleon_type_t;

typedef struct {
	nf_nucleon_type_t type;
	char symbol[4];
	double mass;                    /* Mass (MeV/c^2) */
	double charge;                  /* Charge (e) */
	double spin;                    /* Spin quantum number */
	double isospin;                 /* Isospin projection */
	double magnetic_moment;         /* Magnetic moment (n.m.) */
	gsl_vector *position;           /* 3-position */
	gsl_vector *momentum;           /* 3-momentum */
	void *extra;
} nf_nucleon_t;

/* Nuclear potential types, including advanced and density-dependent models. */
typedef enum {
	NF_POTENTIAL_YUKAWA,
	NF_POTENTIAL_SQUARE_WELL,
	NF_POTENTIAL_HARMONIC_OSC,
	NF_POTENTIAL_SKYRME,
	NF_POTENTIAL_GOGNY,
	NF_POTENTIAL_AV18,
	NF_POTENTIAL_TENSOR,
	NF_POTENTIAL_THREE_BODY,
	NF_POTENTIAL_DENSITY_DEPENDENT,
	NF_POTENTIAL_CUSTOM
} nf_potential_type_t;

/* Nuclear potential structure, supporting tensor and three-body terms. */
typedef struct {
	nf_potential_type_t type;
	char label[32];
	double (*potential_fn)(double r, void *params); /* Potential as function of r */
	double (*tensor_fn)(double r, void *params);    /* Tensor term (optional) */
	double (*three_body_fn)(double r1, double r2, void *params); /* Three-body term (optional) */
	void *params;
	void *extra;
} nf_potential_t;

/* Force model structure, supporting density dependence and extensibility. */
typedef struct {
	nf_potential_t *potential;
	double coupling_constant;
	double range;                   /* Range parameter (fm) */
	double strength;                /* Strength parameter (MeV) */
	double density;                 /* Density parameter (fm^-3) */
	void *extra;
} nf_force_model_t;

/* Diagnostics structure for nuclear forces, supporting form factors and correlation functions. */
typedef struct {
	double total_energy;
	double binding_energy;
	double rms_radius;
	double *energy_spectrum;        /* Array of energy spectrum values */
	size_t n_spectrum;
	double *form_factors;           /* Array of form factor values */
	size_t n_form_factors;
	double *correlation_function;   /* Array of correlation function values */
	size_t n_correlation;
	void *extra;
} nf_diagnostics_t;

/* Nuclear force system structure, supporting batch operations and extensibility. */
typedef struct {
	nf_nucleon_t **nucleons;
	size_t n_nucleons;
	nf_force_model_t **force_models;
	size_t n_force_models;
	char **materials;               /* Array of material/medium names */
	size_t n_materials;
	nf_diagnostics_t diagnostics;
	void *extra;
} nf_system_t;

/* Allocation and management utilities, including batch operations. */
int nf_nucleon_batch_alloc(nf_nucleon_t **nucleons, size_t n, const nf_nucleon_type_t *types, const char **symbols);
void nf_nucleon_batch_free(nf_nucleon_t **nucleons, size_t n);
nf_nucleon_t *nf_nucleon_alloc(nf_nucleon_type_t type, const char *symbol);
void nf_nucleon_free(nf_nucleon_t *nucleon);
nf_potential_t *nf_potential_alloc(nf_potential_type_t type, const char *label, double (*potential_fn)(double, void *), void *params);
void nf_potential_free(nf_potential_t *pot);
nf_force_model_t *nf_force_model_alloc(nf_potential_t *potential, double coupling_constant, double range, double strength);
void nf_force_model_free(nf_force_model_t *model);
nf_system_t *nf_system_alloc(size_t n_nucleons, size_t n_force_models);
void nf_system_free(nf_system_t *system);

/* Diagnostics and I/O utilities, including spectrum and form factor output. */
void nf_diagnostics_print(const nf_diagnostics_t *diag);
void nf_spectrum_print(const double *spectrum, size_t n);
void nf_form_factors_print(const double *form_factors, size_t n);
void nf_correlation_function_print(const double *corr, size_t n);
void nf_diagnostics_update(nf_system_t *system);
void nf_system_print(const nf_system_t *system);
void nf_nucleon_print(const nf_nucleon_t *nucleon);
void nf_potential_print(const nf_potential_t *pot);
void nf_force_model_print(const nf_force_model_t *model);

/* Simulation step utilities, including density-dependent and tensor force evolution. */
int nf_step_density_dependent(nf_system_t *system, double dt);
int nf_step_tensor(nf_system_t *system, double dt);
int nf_step_euler(nf_system_t *system, double dt);
int nf_step_runge_kutta(nf_system_t *system, double dt);

/* Special-case solutions and utilities, including advanced binding and correlation calculations. */
double nf_average_form_factor(const nf_system_t *system);
double nf_average_correlation(const nf_system_t *system);
double nf_binding_energy(const nf_system_t *system);
double nf_pair_potential(const nf_nucleon_t *n1, const nf_nucleon_t *n2, const nf_potential_t *pot);

#endif /* HOLOS_NUCLEAR_FORCES_H */