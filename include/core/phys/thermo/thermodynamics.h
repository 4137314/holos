/*
   thermodynamics.h - Thermodynamics module for HOLOS

   This header provides data structures and utilities for modeling thermodynamic systems,
   including thermodynamic state, processes, equations of state, diagnostics, and calculation steps.

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

#ifndef HOLOS_THERMODYNAMICS_H
#define HOLOS_THERMODYNAMICS_H

#include <stddef.h>

/* Thermodynamic state structure, supporting fugacity, compressibility, transport, and field properties. */
typedef struct {
	double T;                          /* Temperature (K) */
	double P;                          /* Pressure (Pa) */
	double V;                          /* Volume (m^3) */
	double U;                          /* Internal energy (J) */
	double S;                          /* Entropy (J/K) */
	double H;                          /* Enthalpy (J) */
	double G;                          /* Gibbs free energy (J) */
	double F;                          /* Helmholtz free energy (J) */
	double n;                          /* Number of moles */
	double *composition;               /* Array of component fractions */
	size_t n_components;
	char *phase;                       /* Phase (solid, liquid, gas, plasma) */
	double *mu;                        /* Array of chemical potentials (J/mol) */
	double *fugacity;                  /* Array of fugacities (Pa) */
	double compressibility;            /* Compressibility factor Z */
	double viscosity;                  /* Viscosity (Pa s) */
	double thermal_conductivity;       /* Thermal conductivity (W/m/K) */
	double diffusion_coeff;            /* Diffusion coefficient (m^2/s) */
	double surface_tension;            /* Surface tension (N/m) */
	double *electric_field;            /* Array of electric field components (V/m) */
	double *magnetic_field;            /* Array of magnetic field components (T) */
	size_t n_field;
	void *extra;
} td_state_t;

/* Thermodynamic process structure, supporting mixing, phase change, chemical, and field-driven processes. */
typedef struct {
	char *type;                        /* Process type (isothermal, adiabatic, mixing, phase_change, chemical, electrochemical, transport, surface, etc.) */
	double Q;                          /* Heat exchanged (J) */
	double W;                          /* Work done (J) */
	double delta_S;                    /* Entropy change (J/K) */
	double delta_U;                    /* Internal energy change (J) */
	double delta_H;                    /* Enthalpy change (J) */
	double delta_G;                    /* Gibbs free energy change (J) */
	double delta_F;                    /* Helmholtz free energy change (J) */
	double *delta_mu;                  /* Array of chemical potential changes (J/mol) */
	size_t n_components;
	double *delta_fugacity;            /* Array of fugacity changes (Pa) */
	double *delta_composition;         /* Array of composition changes */
	double *delta_electric_field;      /* Array of electric field changes (V/m) */
	double *delta_magnetic_field;      /* Array of magnetic field changes (T) */
	double delta_surface_tension;      /* Surface tension change (N/m) */
	void *extra;
} td_process_t;

/* Equation of state structure, supporting extensibility and parameterization. */
typedef struct {
	char *name;                    /* EOS name (ideal_gas, van_der_waals, etc.) */
	double (*eos_fn)(const td_state_t *state, void *params);
	void *params;
	void *extra;
} td_eos_t;

/* Thermodynamics diagnostics structure, supporting phase boundaries, critical points, and field profiles. */
typedef struct {
	double *property_profile;          /* Array of property values (e.g., T, P, S) */
	size_t n_profile;
	double *response_functions;        /* Array of response function values (e.g., heat capacity, compressibility) */
	size_t n_response;
	double *time_profile;              /* Array of time values */
	size_t n_time;
	double *phase_boundaries;          /* Array of phase boundary values */
	size_t n_boundaries;
	double *critical_points;           /* Array of critical point values */
	size_t n_critical;
	double *electric_field_profile;    /* Array of electric field values */
	double *magnetic_field_profile;    /* Array of magnetic field values */
	size_t n_field_profile;
	void *extra;
} td_diagnostics_t;

/* Thermodynamics system structure, supporting batch operations and extensibility. */
typedef struct {
	td_state_t *state;
	td_process_t **processes;
	size_t n_processes;
	td_eos_t *eos;
	td_diagnostics_t diagnostics;
	char **materials;              /* Array of material/compound names */
	size_t n_materials;
	void *extra;
} td_system_t;

/* Allocation and management utilities, including batch operations and field support. */
td_state_t *td_state_alloc(size_t n_components);
void td_state_free(td_state_t *state);
int td_state_batch_alloc(td_state_t **states, size_t n, const double *T, const double *P, const double *V, const double *U, const double *S, const double *H, const double *G, const double *F, const double *n_moles, const double **composition, const char **phases, const double **mu, const double **fugacity, const double *compressibility, const double *viscosity, const double *thermal_conductivity, const double *diffusion_coeff, const double *surface_tension, const double **electric_field, const double **magnetic_field, const size_t *n_field, const size_t *n_components);
void td_state_batch_free(td_state_t **states, size_t n);
td_process_t *td_process_alloc(const char *type);
void td_process_free(td_process_t *process);
td_eos_t *td_eos_alloc(const char *name, double (*eos_fn)(const td_state_t *, void *), void *params);
void td_eos_free(td_eos_t *eos);
td_system_t *td_system_alloc(size_t n_processes);
void td_system_free(td_system_t *system);

/* Diagnostics and I/O utilities, including response function and profile output. */
void td_diagnostics_update(td_system_t *system);
void td_system_print(const td_system_t *system);
void td_state_print(const td_state_t *state);
void td_process_print(const td_process_t *process);
void td_eos_print(const td_eos_t *eos);
void td_property_profile_print(const double *profile, size_t n);
void td_response_functions_print(const double *rf, size_t n);
void td_time_profile_print(const double *t, size_t n);
void td_phase_boundaries_print(const double *boundaries, size_t n);
void td_critical_points_print(const double *critical, size_t n);
void td_field_profile_print(const double *field, size_t n);

/* Thermodynamics calculation utilities, including advanced, field, and response calculations. */
double td_surface_tension(const td_state_t *state);
double td_electric_field_energy(const td_state_t *state);
double td_magnetic_field_energy(const td_state_t *state);
double td_ideal_gas_pressure(double n, double T, double V);
double td_ideal_gas_entropy(double n, double V, double T);
double td_van_der_waals_pressure(double n, double T, double V, double a, double b);
double td_gibbs_free_energy(double H, double T, double S);
double td_helmholtz_free_energy(double U, double T, double S);
double td_heat_capacity(const td_state_t *state);
double td_compressibility(const td_state_t *state);
double td_thermal_expansion(const td_state_t *state);

#endif /* HOLOS_THERMODYNAMICS_H */