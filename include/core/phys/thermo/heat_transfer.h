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

/* Heat state structure. */
typedef struct {
	double T;                  /* Temperature (K) */
	double Q;                  /* Heat content (J) */
	double m;                  /* Mass (kg) */
	double c;                  /* Specific heat (J/kg/K) */
	double k;                  /* Thermal conductivity (W/m/K) */
	double x;                  /* Position (m) */
	double t;                  /* Time (s) */
	void *extra;
} ht_state_t;

/* Material property structure. */
typedef struct {
	char *name;                /* Material name */
	double density;            /* Density (kg/m^3) */
	double specific_heat;      /* Specific heat (J/kg/K) */
	double conductivity;       /* Thermal conductivity (W/m/K) */
	double emissivity;         /* Emissivity (0-1) */
	double absorptivity;       /* Absorptivity (0-1) */
	void *extra;
} ht_material_t;

/* Boundary condition structure. */
typedef enum {
	HT_BC_NONE,
	HT_BC_DIRICHLET,
	HT_BC_NEUMANN,
	HT_BC_ROBIN,
	HT_BC_PERIODIC
} ht_bc_type_t;

typedef struct {
	ht_bc_type_t type;
	double value;
	double h;                  /* Heat transfer coefficient (W/m^2/K) for Robin */
	void *extra;
} ht_boundary_t;

/* Heat transfer diagnostics structure. */
typedef struct {
	double total_heat;
	double heat_flux;
	double *temperature_profile;   /* Array of temperature values */
	size_t n_profile;
	double *heat_flux_profile;     /* Array of heat flux values */
	size_t n_flux;
	void *extra;
} ht_diagnostics_t;

/* Heat transfer system structure. */
typedef struct {
	ht_state_t *state;
	ht_material_t **materials;
	size_t n_materials;
	ht_boundary_t **boundaries;
	size_t n_boundaries;
	ht_diagnostics_t diagnostics;
	void *extra;
} ht_system_t;

/* Allocation and management utilities. */
ht_state_t *ht_state_alloc(void);
void ht_state_free(ht_state_t *state);
ht_material_t *ht_material_alloc(const char *name);
void ht_material_free(ht_material_t *mat);
ht_boundary_t *ht_boundary_alloc(ht_bc_type_t type, double value, double h);
void ht_boundary_free(ht_boundary_t *bc);
ht_system_t *ht_system_alloc(size_t n_materials, size_t n_boundaries);
void ht_system_free(ht_system_t *system);

/* Diagnostics and I/O utilities. */
void ht_diagnostics_update(ht_system_t *system);
void ht_system_print(const ht_system_t *system);
void ht_state_print(const ht_state_t *state);
void ht_material_print(const ht_material_t *mat);
void ht_boundary_print(const ht_boundary_t *bc);

/* Heat transfer calculation utilities. */
double ht_conduction(double k, double A, double dT, double dx);
double ht_convection(double h, double A, double dT);
double ht_radiation(double emissivity, double A, double T1, double T2);

#endif /* HOLOS_HEAT_TRANSFER_H */
