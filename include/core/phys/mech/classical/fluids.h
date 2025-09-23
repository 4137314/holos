/* fluids.h - Classical fluid mechanics API for HOLOS
    Copyright (C) 2025  4137314 <holos@mail.com>

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


/* Structure representing a single fluid element: density, pressure, velocity. */
typedef struct {
    double rho;       /* Density [kg/m^3] */
    double p;         /* Pressure [Pa] */
    gsl_vector *v;    /* Velocity vector [m/s] */
} fluid_element;

/* Structure representing a system of fluid elements. */
typedef struct {
    size_t n;              /* Number of elements */
    fluid_element *elems;  /* Array of elements */
} fluid_system;


/* -------------------------------------------------------------------------- */
/* Core fluid mechanics                                                       */
/* -------------------------------------------------------------------------- */


/*
    Compute the continuity equation: ∂ρ/∂t + ∇·(ρv) = 0
    rho_time_deriv: [out] time derivative of density field
    rho: current density field
    v: velocity field (matrix with velocity components)
    Returns 0 on success, non-zero on error.
*/
int fluid_continuity_equation(gsl_vector *rho_time_deriv,
                                        const gsl_vector *rho,
                                        const gsl_matrix *v);


/*
    Compute the incompressible Navier–Stokes equations:
    ρ ( ∂v/∂t + (v·∇)v ) = -∇p + μ∇²v + f
    v_time_deriv: [out] time derivative of velocity field
    v: velocity field
    p: pressure field
    rho: density field
    mu: dynamic viscosity
    f: external force field
    Returns 0 on success, non-zero on error.
*/
int fluid_navier_stokes(gsl_matrix *v_time_deriv,
                                const gsl_matrix *v,
                                const gsl_vector *p,
                                const gsl_vector *rho,
                                double mu,
                                const gsl_matrix *f);


/*
    Compute Bernoulli’s equation for incompressible, inviscid flow:
    p + 0.5 * ρ v^2 + ρ g h = const
    rho: fluid density
    v: velocity magnitude
    g: gravitational acceleration
    h: height
    Returns energy per unit volume [Pa].
*/
double fluid_bernoulli(double rho, double v, double g, double h);


/* -------------------------------------------------------------------------- */
/* Utilities                                                                  */
/* -------------------------------------------------------------------------- */


/* Allocate a new fluid element. dim: dimension of velocity vector. Returns pointer or NULL. */
fluid_element *fluid_element_alloc(size_t dim);

/* Free a fluid element. */
void fluid_element_free(fluid_element *elem);

/* Initialize a fluid system with n elements and velocity dimension dim. Returns pointer or NULL. */
fluid_system *fluid_system_alloc(size_t n, size_t dim);

/* Free a fluid system. */
void fluid_system_free(fluid_system *sys);

#endif /* HOLOS_FLUIDS_H */