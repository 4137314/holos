/*
   entropy.h - Entropy module for HOLOS (Thermodynamics)

   This header provides data structures and utilities for modeling entropy in thermodynamic systems,
   including entropy state, processes, diagnostics, and calculation steps.

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

#ifndef HOLOS_ENTROPY_H
#define HOLOS_ENTROPY_H

#include <stddef.h>

/* Entropy state structure, supporting phase, chemical potential, and extensibility. */
typedef struct {
	double S;                      /* Entropy (J/K) */
	double T;                      /* Temperature (K) */
	double V;                      /* Volume (m^3) */
	double P;                      /* Pressure (Pa) */
	double U;                      /* Internal energy (J) */
	double n;                      /* Number of moles */
	double *composition;           /* Array of component fractions */
	size_t n_components;
	char *phase;                   /* Phase (solid, liquid, gas, plasma) */
	double *mu;                    /* Array of chemical potentials (J/mol) */
	double entropy_flux;           /* Entropy flux (J/K/s) */
	void *extra;
} ent_state_t;

/* Entropy process structure, supporting mixing, phase change, and chemical processes. */
typedef struct {
	char *type;                    /* Process type (reversible, irreversible, mixing, phase_change, chemical, etc.) */
	double delta_S;                /* Entropy change (J/K) */
	double Q;                      /* Heat exchanged (J) */
	double W;                      /* Work done (J) */
	double T_env;                  /* Environment temperature (K) */
	double *delta_mu;              /* Array of chemical potential changes (J/mol) */
	size_t n_components;
	double entropy_flux;           /* Entropy flux (J/K/s) */
	void *extra;
} ent_process_t;

/* Entropy diagnostics structure, supporting flux, spatial/temporal profiles, and advanced observables. */
typedef struct {
	double total_entropy;
	double entropy_production;
	double *entropy_profile;       /* Array of entropy values over time or space */
	size_t n_profile;
	double *entropy_flux_profile;  /* Array of entropy flux values */
	size_t n_flux;
	double *entropy_production_profile; /* Array of entropy production values */
	size_t n_prod;
	void *extra;
} ent_diagnostics_t;

/* Entropy system structure, supporting batch operations and extensibility. */
typedef struct {
	ent_state_t *state;
	ent_process_t **processes;
	size_t n_processes;
	ent_diagnostics_t diagnostics;
	char **materials;              /* Array of material/compound names */
	size_t n_materials;
	void *extra;
} ent_system_t;

ent_system_t *ent_system_alloc(size_t n_processes);
/* Allocation and management utilities, including batch operations. */
ent_state_t *ent_state_alloc(size_t n_components);
void ent_state_free(ent_state_t *state);
int ent_state_batch_alloc(ent_state_t **states, size_t n, const double *S, const double *T, const double *V, const double *P, const double *U, const double *n_moles, const double **composition, const char **phases, const double **mu, const double *entropy_flux, const size_t *n_components);
void ent_state_batch_free(ent_state_t **states, size_t n);
ent_process_t *ent_process_alloc(const char *type);
void ent_process_free(ent_process_t *process);
ent_system_t *ent_system_alloc(size_t n_processes);
void ent_system_free(ent_system_t *system);

/* Diagnostics and I/O utilities, including entropy flux and profile output. */
void ent_diagnostics_update(ent_system_t *system);
void ent_system_print(const ent_system_t *system);
void ent_state_print(const ent_state_t *state);
void ent_process_print(const ent_process_t *process);
void ent_entropy_profile_print(const double *profile, size_t n);
void ent_entropy_flux_profile_print(const double *flux, size_t n);
void ent_entropy_production_profile_print(const double *prod, size_t n);

/* Entropy calculation utilities, including advanced and process-specific calculations. */
double ent_entropy_ideal_gas(double n, double V, double T);
double ent_entropy_mixing(const double *composition, size_t n_components);
double ent_entropy_phase_change(double n, double L, double T);
double ent_entropy_chemical(const double *mu, const double *composition, size_t n_components);
double ent_entropy_change(const ent_state_t *initial, const ent_state_t *final);

#endif /* HOLOS_ENTROPY_H */