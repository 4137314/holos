/*
   heat_transfer.h - Heat transfer module for HOLOS (Thermodynamics)

   This header provides data structures and utilities for modeling heat transfer in thermodynamic systems,
   including heat state, materials, boundary conditions, diagnostics, and calculation steps.

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

#ifndef HOLOS_HEAT_TRANSFER_H
#define HOLOS_HEAT_TRANSFER_H

#include <stddef.h>

/* Heat state structure, supporting phase, anisotropy, and heat source. */
typedef struct {
	double T;                      /* Temperature (K) */
	double Q;                      /* Heat content (J) */
	double m;                      /* Mass (kg) */
	double c;                      /* Specific heat (J/kg/K) */
	double k;                      /* Thermal conductivity (W/m/K) */
	double x;                      /* Position (m) */
	double t;                      /* Time (s) */
	char *phase;                   /* Phase (solid, liquid, gas) */
	double *anisotropy;            /* Array of anisotropic conductivities */
	size_t n_anisotropy;
	double heat_source;            /* Volumetric heat source (W/m^3) */
	void *extra;
} ht_state_t;

/* Material property structure, supporting anisotropy and phase change. */
typedef struct {
	char *name;                    /* Material name */
	double density;                /* Density (kg/m^3) */
	double specific_heat;          /* Specific heat (J/kg/K) */
	double conductivity;           /* Thermal conductivity (W/m/K) */
	double *anisotropy;            /* Array of anisotropic conductivities */
	size_t n_anisotropy;
	double emissivity;             /* Emissivity (0-1) */
	double absorptivity;           /* Absorptivity (0-1) */
	double latent_heat;            /* Latent heat (J/kg) */
	char *phase;                   /* Phase (solid, liquid, gas) */
	void *extra;
} ht_material_t;

/* Boundary condition structure, supporting moving, radiative, and convective boundaries. */
typedef enum {
	HT_BC_NONE,
	HT_BC_DIRICHLET,
	HT_BC_NEUMANN,
	HT_BC_ROBIN,
	HT_BC_PERIODIC,
	HT_BC_MOVING,
	HT_BC_RADIATIVE,
	HT_BC_CONVECTIVE
} ht_bc_type_t;

typedef struct {
	ht_bc_type_t type;
	double value;
	double h;                      /* Heat transfer coefficient (W/m^2/K) for Robin/convective */
	double velocity;               /* Boundary velocity (m/s) for moving boundaries */
	double emissivity;             /* Emissivity for radiative boundaries */
	double T_inf;                  /* Ambient temperature (K) for convective/radiative */
	void *extra;
} ht_boundary_t;

/* Heat transfer diagnostics structure, supporting spatial/temporal profiles and advanced observables. */
typedef struct {
	double total_heat;
	double heat_flux;
	double *temperature_profile;       /* Array of temperature values */
	size_t n_profile;
	double *heat_flux_profile;         /* Array of heat flux values */
	size_t n_flux;
	double *nusselt_number;            /* Array of Nusselt numbers */
	size_t n_nusselt;
	double *biot_number;               /* Array of Biot numbers */
	size_t n_biot;
	double *time_profile;              /* Array of time values */
	size_t n_time;
	void *extra;
} ht_diagnostics_t;

/* Heat transfer system structure, supporting batch operations and extensibility. */
typedef struct {
	ht_state_t *state;
	ht_material_t **materials;
	size_t n_materials;
	ht_boundary_t **boundaries;
	size_t n_boundaries;
	ht_diagnostics_t diagnostics;
	void *extra;
} ht_system_t;

/* Allocation and management utilities, including batch operations. */
ht_state_t *ht_state_alloc(void);
void ht_state_free(ht_state_t *state);
int ht_state_batch_alloc(ht_state_t **states, size_t n, const double *T, const double *Q, const double *m, const double *c, const double *k, const double *x, const double *t, const char **phases, const double **anisotropy, const size_t *n_anisotropy, const double *heat_source);
void ht_state_batch_free(ht_state_t **states, size_t n);
ht_material_t *ht_material_alloc(const char *name);
void ht_material_free(ht_material_t *mat);
ht_boundary_t *ht_boundary_alloc(ht_bc_type_t type, double value, double h);
void ht_boundary_free(ht_boundary_t *bc);
ht_system_t *ht_system_alloc(size_t n_materials, size_t n_boundaries);
void ht_system_free(ht_system_t *system);

/* Diagnostics and I/O utilities, including Nusselt/Biot number and profile output. */
void ht_diagnostics_update(ht_system_t *system);
void ht_system_print(const ht_system_t *system);
void ht_state_print(const ht_state_t *state);
void ht_material_print(const ht_material_t *mat);
void ht_boundary_print(const ht_boundary_t *bc);
void ht_temperature_profile_print(const double *profile, size_t n);
void ht_heat_flux_profile_print(const double *profile, size_t n);
void ht_nusselt_number_print(const double *Nu, size_t n);
void ht_biot_number_print(const double *Bi, size_t n);

/* Heat transfer calculation utilities, including advanced and transient calculations. */
double ht_conduction(double k, double A, double dT, double dx);
double ht_convection(double h, double A, double dT);
double ht_radiation(double emissivity, double A, double T1, double T2);
double ht_nusselt_number(double h, double L, double k);
double ht_biot_number(double h, double L, double k);
double ht_transient_temperature(double T0, double T_inf, double Bi, double Fo);

#endif /* HOLOS_HEAT_TRANSFER_H */