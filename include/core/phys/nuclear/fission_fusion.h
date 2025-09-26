/*
   fission_fusion.h - Nuclear fission and fusion module for HOLOS

   This header provides data structures and utilities for modeling nuclear fission and fusion
   processes, including nuclei, reactions, cross-sections, diagnostics, and simulation steps.

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

#ifndef HOLOS_FISSION_FUSION_H
#define HOLOS_FISSION_FUSION_H

#include <stddef.h>
#include <gsl/gsl_vector.h>

/* Nucleus structure, supporting extensible nuclear properties. */
typedef struct {
	char symbol[8];
	int Z;                  /* Atomic number */
	int N;                  /* Neutron number */
	double mass;            /* Atomic mass (u) */
	double energy;          /* Excitation energy (MeV) */
	double spin;            /* Spin quantum number */
	double parity;          /* Parity */
	double deformation;     /* Nuclear deformation parameter */
	double temperature;     /* Nuclear temperature (MeV) */
	gsl_vector *momentum;   /* 3-momentum */
	gsl_vector *position;   /* 3-position */
	void *extra;            /* For extensibility */
} ff_nucleus_t;

/* Reaction type, including advanced channels. */
typedef enum {
	FF_REACTION_FISSION,
	FF_REACTION_FUSION,
	FF_REACTION_CAPTURE,
	FF_REACTION_DECAY,
	FF_REACTION_NEUTRON_EMISSION,
	FF_REACTION_GAMMA_EMISSION,
	FF_REACTION_ALPHA_EMISSION,
	FF_REACTION_BETA_DECAY,
	FF_REACTION_PHOTODISINTEGRATION
} ff_reaction_type_t;

/* Nuclear reaction structure, supporting multiple channels and emission products. */
typedef struct {
	ff_reaction_type_t type;
	char label[32];
	ff_nucleus_t **reactants;
	size_t n_reactants;
	ff_nucleus_t **products;
	size_t n_products;
	int n_neutrons_emitted;
	int n_gammas_emitted;
	double *neutron_energies; /* Array of neutron energies (MeV) */
	double *gamma_energies;   /* Array of gamma energies (MeV) */
	double Q_value;           /* Energy released (MeV) */
	double cross_section;     /* Cross-section (barns) */
	double rate;              /* Reaction rate (1/s) */
	void *extra;
} ff_reaction_t;

/* Cross-section data, supporting energy-dependent and angular distributions. */
typedef struct {
	char label[32];
	double energy;              /* Incident energy (MeV) */
	double value;               /* Cross-section (barns) */
	double error;               /* Uncertainty */
	double angle;               /* Scattering angle (deg) */
	double *angular_dist;       /* Array of angular distribution values */
	size_t n_angles;
	void *extra;
} ff_cross_section_t;

/* Diagnostics structure, supporting advanced observables and time histories. */
typedef struct {
	double total_energy;
	double neutron_multiplicity;
	double gamma_multiplicity;
	double reaction_rate;
	double *yields;             /* Array of product yields */
	size_t n_yields;
	double *energy_spectrum;    /* Array of energy spectrum values */
	size_t n_spectrum;
	double *time_history;       /* Array of time-dependent observables */
	size_t n_time;
	void *extra;
} ff_diagnostics_t;

/* Fission/Fusion system structure, supporting boundaries, materials, and extensibility. */
typedef struct {
	ff_nucleus_t **nuclei;
	size_t n_nuclei;
	ff_reaction_t **reactions;
	size_t n_reactions;
	ff_cross_section_t **cross_sections;
	size_t n_cross_sections;
	char **materials;           /* Array of material/medium names */
	size_t n_materials;
	double *boundaries;         /* Array of boundary values (e.g., geometry) */
	size_t n_boundaries;
	ff_diagnostics_t diagnostics;
	void *extra;
} ff_system_t;

/* Allocation and management utilities, including batch operations. */
int ff_nucleus_batch_alloc(ff_nucleus_t **nuclei, size_t n, const char **symbols, const int *Zs, const int *Ns);
void ff_nucleus_batch_free(ff_nucleus_t **nuclei, size_t n);
ff_nucleus_t *ff_nucleus_alloc(const char *symbol, int Z, int N);
void ff_nucleus_free(ff_nucleus_t *nucleus);
ff_reaction_t *ff_reaction_alloc(ff_reaction_type_t type, const char *label, size_t n_reactants, size_t n_products);
void ff_reaction_free(ff_reaction_t *reaction);
ff_cross_section_t *ff_cross_section_alloc(const char *label, double energy, double value, double error);
void ff_cross_section_free(ff_cross_section_t *cs);
ff_system_t *ff_system_alloc(size_t n_nuclei, size_t n_reactions, size_t n_cross_sections);
void ff_system_free(ff_system_t *system);

/* Diagnostics and I/O utilities, including spectrum and time history output. */
void ff_diagnostics_print(const ff_diagnostics_t *diag);
void ff_spectrum_print(const double *spectrum, size_t n);
void ff_time_history_print(const double *history, size_t n);
void ff_diagnostics_update(ff_system_t *system);
void ff_system_print(const ff_system_t *system);
void ff_nucleus_print(const ff_nucleus_t *nucleus);
void ff_reaction_print(const ff_reaction_t *reaction);
void ff_cross_section_print(const ff_cross_section_t *cs);

/* Simulation step utilities, including neutron/gamma emission. */
int ff_emit_neutrons(ff_reaction_t *reaction, double *energies, int n);
int ff_emit_gammas(ff_reaction_t *reaction, double *energies, int n);
int ff_step_euler(ff_system_t *system, double dt);
int ff_step_runge_kutta(ff_system_t *system, double dt);

/* Special-case solutions and utilities, including advanced yield and rate calculations. */
double ff_average_neutron_energy(const ff_reaction_t *reaction);
double ff_average_gamma_energy(const ff_reaction_t *reaction);
double ff_fission_yield(const ff_nucleus_t *nucleus, double energy);
double ff_fusion_rate(const ff_nucleus_t *nucleus1, const ff_nucleus_t *nucleus2, double temperature);

#endif /* HOLOS_FISSION_FUSION_H */
