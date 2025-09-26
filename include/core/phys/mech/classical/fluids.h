/* fluids.h - Classical fluid mechanics API for HOLOS
    Copyright (C) 2025  4137314

    This file is part of HOLOS.

    HOLOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HOLOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with HOLOS.  If not, see <https://www.gnu.org/licenses/>.

    ----------------------------------------------------------------------
    API for classical fluid mechanics using GNU Scientific Library (GSL).
    - Incompressible/compressible flows, Navier–Stokes, hydrodynamics
    - All functions use GSL vectors and matrices
*/

#ifndef HOLOS_FLUIDS_H
#define HOLOS_FLUIDS_H


#include <stddef.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>


/*
    Fundamental equations and utilities for classical fluid mechanics.
    Defines the primary functions for incompressible/compressible flows,
    Navier–Stokes dynamics, and hydrodynamic relations.
    All functions use GNU GSL vectors and matrices.
*/

/* -------------------------------------------------------------------------- */
/* Fluid state structures                                                     */
/* -------------------------------------------------------------------------- */


/* Structure representing a single fluid element: density, pressure, velocity, temperature, etc. */
typedef struct {
    double rho;           /* Density [kg/m^3] */
    double p;             /* Pressure [Pa] */
    gsl_vector *v;        /* Velocity vector [m/s] */
    double T;             /* Temperature [K] */
    double mu;            /* Dynamic viscosity [Pa s] */
    double kappa;         /* Thermal conductivity [W/(m K)] */
    double E;             /* Internal energy per unit mass [J/kg] */
    char *tag;            /* Optional label/tag */
    int fixed;            /* 1 if element is fixed/immobile */
    void *userdata;       /* User-defined pointer for extensions */
} fluid_element;

/* Structure representing a system of fluid elements, with boundaries and properties. */
typedef struct {
    size_t n;                  /* Number of elements */
    fluid_element *elems;      /* Array of elements */
    double t;                  /* Current simulation time */
    double dt_last;            /* Last timestep used */
    int step_count;            /* Number of steps taken */
    char *name;                /* System name/label */
    double volume;             /* System volume [m^3] */
    double boundary_p;         /* Boundary pressure [Pa] */
    double boundary_T;         /* Boundary temperature [K] */
    int has_boundaries;        /* 1 if boundaries are present */
    void *boundaries;          /* Pointer to boundary structure */
    void *userdata;            /* User-defined pointer for extensions */
} fluid_system;


/* -------------------------------------------------------------------------- */
/* Core fluid mechanics and diagnostics                                       */
/* -------------------------------------------------------------------------- */


/* Compute the continuity equation: ∂ρ/∂t + ∇·(ρv) = 0 */
int fluid_continuity_equation(gsl_vector *rho_time_deriv,
                             const gsl_vector *rho,
                             const gsl_matrix *v);


/* Compute the incompressible Navier–Stokes equations:
   ρ ( ∂v/∂t + (v·∇)v ) = -∇p + μ∇²v + f */
int fluid_navier_stokes(gsl_matrix *v_time_deriv,
                       const gsl_matrix *v,
                       const gsl_vector *p,
                       const gsl_vector *rho,
                       double mu,
                       const gsl_matrix *f);


/* Compute Bernoulli’s equation for incompressible, inviscid flow:
   p + 0.5 * ρ v^2 + ρ g h = const */
double fluid_bernoulli(double rho, double v, double g, double h);

/* Compute Reynolds number: Re = (rho * v * L) / mu */
double fluid_reynolds_number(double rho, double v, double L, double mu);

/* Compute Mach number: Ma = v / c */
double fluid_mach_number(double v, double c);

/* Compute speed of sound in fluid: c = sqrt(gamma * p / rho) */
double fluid_speed_of_sound(double gamma, double p, double rho);

/* Compute vorticity field from velocity field. */
int fluid_vorticity(gsl_matrix *vorticity, const gsl_matrix *v);

/* Compute divergence of velocity field. */
int fluid_divergence(gsl_vector *div, const gsl_matrix *v);


/* -------------------------------------------------------------------------- */
/* Utilities and advanced operations                                          */
/* -------------------------------------------------------------------------- */


/* Allocate a new fluid element. dim: dimension of velocity vector. Returns pointer or NULL. */
fluid_element *fluid_element_alloc(size_t dim);

/* Free a fluid element. */
void fluid_element_free(fluid_element *elem);

/* Print a fluid element to file or stdout. */
void fluid_element_print(const fluid_element *elem, FILE *f);

/* Initialize a fluid system with n elements and velocity dimension dim. Returns pointer or NULL. */
fluid_system *fluid_system_alloc(size_t n, size_t dim);

/* Free a fluid system. */
void fluid_system_free(fluid_system *sys);

/* Print a fluid system to file or stdout. */
void fluid_system_print(const fluid_system *sys, FILE *f);

/* Deep copy of a fluid system. */
fluid_system *fluid_system_clone(const fluid_system *src);

/* Add a fluid element to the system (returns new index or -1 on error). */
int fluid_system_add_element(fluid_system *sys, const fluid_element *elem);

/* Remove a fluid element by index (returns 0 on success). */
int fluid_system_remove_element(fluid_system *sys, size_t idx);

/* Find a fluid element by tag (returns pointer or NULL). */
fluid_element *fluid_system_find_element(fluid_system *sys, const char *tag);

/* Apply boundary conditions if present. */
int fluid_system_apply_boundaries(fluid_system *sys);

#endif /* HOLOS_FLUIDS_H */