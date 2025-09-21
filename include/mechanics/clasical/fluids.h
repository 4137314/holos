#ifndef FLUIDS_H
#define FLUIDS_H

#include <stddef.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/**
 * @file fluids.h
 * @brief Fundamental equations and utilities for classical fluid mechanics.
 *
 * Defines the primary functions for incompressible/compressible flows,
 * Navier–Stokes dynamics, and hydrodynamic relations.
 *
 * All functions use GNU GSL vectors and matrices.
 */

/* -------------------------------------------------------------------------- */
/* Fluid state structures                                                     */
/* -------------------------------------------------------------------------- */

/** Fluid element: density, pressure, velocity vector */
typedef struct {
    double rho;       /**< density [kg/m^3] */
    double p;         /**< pressure [Pa] */
    gsl_vector *v;    /**< velocity vector [m/s] */
} fluid_element;

/** Fluid system: collection of fluid elements */
typedef struct {
    size_t n;              /**< number of elements */
    fluid_element *elems;  /**< array of elements */
} fluid_system;


/* -------------------------------------------------------------------------- */
/* Core fluid mechanics                                                       */
/* -------------------------------------------------------------------------- */

/**
 * @brief Continuity equation: ∂ρ/∂t + ∇·(ρv) = 0
 * @param rho_time_deriv [out] time derivative of density field
 * @param rho            current density field
 * @param v              velocity field (matrix with velocity components)
 * @return 0 on success, non-zero on error
 */
int fluid_continuity_equation(gsl_vector *rho_time_deriv,
                              const gsl_vector *rho,
                              const gsl_matrix *v);

/**
 * @brief Incompressible Navier–Stokes equations.
 *        ρ ( ∂v/∂t + (v·∇)v ) = -∇p + μ∇²v + f
 * @param v_time_deriv [out] time derivative of velocity field
 * @param v            velocity field
 * @param p            pressure field
 * @param rho          density field
 * @param mu           dynamic viscosity
 * @param f            external force field
 * @return 0 on success, non-zero on error
 */
int fluid_navier_stokes(gsl_matrix *v_time_deriv,
                        const gsl_matrix *v,
                        const gsl_vector *p,
                        const gsl_vector *rho,
                        double mu,
                        const gsl_matrix *f);

/**
 * @brief Bernoulli’s equation for incompressible, inviscid flow.
 *        p + 0.5 * ρ v^2 + ρ g h = const
 * @param rho fluid density
 * @param v   velocity magnitude
 * @param g   gravitational acceleration
 * @param h   height
 * @return energy per unit volume [Pa]
 */
double fluid_bernoulli(double rho, double v, double g, double h);


/* -------------------------------------------------------------------------- */
/* Utilities                                                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Allocate a new fluid element.
 * @param dim dimension of velocity vector
 * @return pointer to new fluid_element, NULL on failure
 */
fluid_element *fluid_element_alloc(size_t dim);

/**
 * @brief Free a fluid element.
 * @param elem pointer to fluid_element
 */
void fluid_element_free(fluid_element *elem);

/**
 * @brief Initialize a fluid system with n elements.
 * @param n   number of elements
 * @param dim dimension of velocity vectors
 * @return pointer to fluid_system, NULL on failure
 */
fluid_system *fluid_system_alloc(size_t n, size_t dim);

/**
 * @brief Free a fluid system.
 * @param sys pointer to fluid_system
 */
void fluid_system_free(fluid_system *sys);

#endif /* FLUIDS_H */