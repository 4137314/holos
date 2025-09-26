/*
   nuclear_decay.h - Nuclear decay module for HOLOS

   This header provides data structures and utilities for modeling nuclear decay processes,
   including nuclei, decay modes, decay chains, branching ratios, diagnostics, and simulation steps.

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

#ifndef HOLOS_NUCLEAR_DECAY_H
#define HOLOS_NUCLEAR_DECAY_H

#include <stddef.h>
#include <gsl/gsl_vector.h>

/* Nucleus structure for decay modeling, supporting extensible nuclear properties. */
typedef struct {
	char symbol[8];
	int Z;                      /* Atomic number */
	int N;                      /* Neutron number */
	double mass;                /* Atomic mass (u) */
	double energy;              /* Excitation energy (MeV) */
	double spin;                /* Spin quantum number */
	double parity;              /* Parity */
	double deformation;         /* Nuclear deformation parameter */
	double temperature;         /* Nuclear temperature (MeV) */
	gsl_vector *momentum;       /* 3-momentum */
	gsl_vector *position;       /* 3-position */
	void *extra;                /* For extensibility */
} nd_nucleus_t;

/* Decay mode enumeration, including rare and isomeric decays. */
typedef enum {
	ND_DECAY_ALPHA,
	ND_DECAY_BETA_MINUS,
	ND_DECAY_BETA_PLUS,
	ND_DECAY_GAMMA,
	ND_DECAY_ELECTRON_CAPTURE,
	ND_DECAY_SPONTANEOUS_FISSION,
	ND_DECAY_PROTON_EMISSION,
	ND_DECAY_NEUTRON_EMISSION,
	ND_DECAY_DOUBLE_BETA,
	ND_DECAY_CLUSTER,
	ND_DECAY_ISOMERIC,
	ND_DECAY_DELAYED_NEUTRON,
	ND_DECAY_DELAYED_GAMMA,
	ND_DECAY_RARE
} nd_decay_mode_t;

/* Decay branch structure, supporting emission spectra and time evolution. */
typedef struct {
	nd_decay_mode_t mode;
	double branching_ratio;         /* Probability of this branch */
	double Q_value;                 /* Energy released (MeV) */
	double half_life;               /* Half-life (s) for this branch */
	double *emission_spectrum;      /* Array of emission spectrum values */
	size_t n_spectrum;
	double *time_evolution;         /* Array of time-dependent observables */
	size_t n_time;
	nd_nucleus_t *daughter;         /* Daughter nucleus */
	void *extra;
} nd_decay_branch_t;

/* Decay chain structure, supporting time evolution and batch operations. */
typedef struct {
	nd_nucleus_t *parent;
	nd_decay_branch_t **branches;
	size_t n_branches;
	double *time_evolution;         /* Array of time-dependent observables */
	size_t n_time;
	void *extra;
} nd_decay_chain_t;

/* Diagnostics structure for decay processes, supporting advanced observables. */
typedef struct {
	double total_activity;          /* Total decay rate (Bq) */
	double total_energy;            /* Total energy released (MeV) */
	double *branch_activities;      /* Array of activities per branch */
	size_t n_branches;
	double *time_history;           /* Array of time-dependent observables */
	size_t n_time;
	double *energy_spectrum;        /* Array of energy spectrum values */
	size_t n_spectrum;
	void *extra;
} nd_diagnostics_t;

/* Nuclear decay system structure, supporting batch operations and extensibility. */
typedef struct {
	nd_nucleus_t **nuclei;
	size_t n_nuclei;
	nd_decay_chain_t **chains;
	size_t n_chains;
	char **materials;               /* Array of material/medium names */
	size_t n_materials;
	nd_diagnostics_t diagnostics;
	void *extra;
} nd_system_t;

/* Allocation and management utilities, including batch operations. */
int nd_nucleus_batch_alloc(nd_nucleus_t **nuclei, size_t n, const char **symbols, const int *Zs, const int *Ns);
void nd_nucleus_batch_free(nd_nucleus_t **nuclei, size_t n);
nd_nucleus_t *nd_nucleus_alloc(const char *symbol, int Z, int N);
void nd_nucleus_free(nd_nucleus_t *nucleus);
nd_decay_branch_t *nd_decay_branch_alloc(nd_decay_mode_t mode, double branching_ratio, double Q_value, double half_life, nd_nucleus_t *daughter);
void nd_decay_branch_free(nd_decay_branch_t *branch);
nd_decay_chain_t *nd_decay_chain_alloc(nd_nucleus_t *parent, size_t n_branches);
void nd_decay_chain_free(nd_decay_chain_t *chain);
nd_system_t *nd_system_alloc(size_t n_nuclei, size_t n_chains);
void nd_system_free(nd_system_t *system);

/* Diagnostics and I/O utilities, including spectrum and time history output. */
void nd_diagnostics_print(const nd_diagnostics_t *diag);
void nd_spectrum_print(const double *spectrum, size_t n);
void nd_time_history_print(const double *history, size_t n);
void nd_diagnostics_update(nd_system_t *system);
void nd_system_print(const nd_system_t *system);
void nd_nucleus_print(const nd_nucleus_t *nucleus);
void nd_decay_branch_print(const nd_decay_branch_t *branch);
void nd_decay_chain_print(const nd_decay_chain_t *chain);

/* Simulation step utilities, including delayed emission. */
int nd_emit_delayed_neutrons(nd_decay_branch_t *branch, double *energies, int n);
int nd_emit_delayed_gammas(nd_decay_branch_t *branch, double *energies, int n);
int nd_step_decay(nd_system_t *system, double dt);

/* Special-case solutions and utilities, including Bateman equations and advanced activity calculations. */
double nd_bateman_chain(const nd_decay_chain_t *chain, double t, double n0);
double nd_average_emission_energy(const nd_decay_branch_t *branch);
double nd_branch_activity(const nd_decay_branch_t *branch, double n_parent);
double nd_chain_activity(const nd_decay_chain_t *chain, double n_parent);

#endif /* HOLOS_NUCLEAR_DECAY_H */
