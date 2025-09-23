/* dynamics.h - Newtonian dynamics API for HOLOS
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
    API for Newtonian dynamics using GNU Scientific Library (GSL).
    - positions, velocities, forces are gsl_vector of dimension 3
    - system = collection of particles
    - API exposes force computation, integration, and diagnostics
    - integrators can be backed by GSL ODE solvers (gsl_odeiv2)
*/

#ifndef HOLOS_DYNAMICS_H
#define HOLOS_DYNAMICS_H


#include <gsl/gsl_vector.h>
#include <stddef.h>
#include <stdbool.h>


/* -------------------------------
   Type Definitions
   ------------------------------- */

/* Structure representing a single particle in the system. */
typedef struct {
    double mass;          /* Mass of the particle */
    gsl_vector *pos;      /* Position (3D vector) */
    gsl_vector *vel;      /* Velocity (3D vector) */
    gsl_vector *force;    /* Net force (3D vector) */
} dyn_particle_t;

/* Forward declaration for the system structure. */
struct dyn_system_s;

/* User-defined force callback type. */
typedef void (*dyn_force_cb)(struct dyn_system_s *system, void *userdata);

/* Enumeration of available integrators. */
typedef enum {
    DYN_INT_EULER = 0,      /* Euler integrator */
    DYN_INT_VERLET,         /* Verlet integrator */
    DYN_INT_LEAPFROG,       /* Leapfrog integrator */
    DYN_INT_RK4,            /* Runge-Kutta 4 integrator */
    DYN_INT_GSL_ODE         /* Use GSL ODE integrator */
} dyn_integrator_t;

/* Structure representing a system of particles. */
typedef struct dyn_system_s {
    size_t N;                   /* Number of particles */
    dyn_particle_t *particles;  /* Array of particles */

    double G;                   /* Gravitational constant */
    bool use_softening;         /* Use softening for gravity */
    double softening_eps;       /* Softening parameter */

    dyn_force_cb force_cb;      /* Custom force callback */
    void *force_cb_userdata;    /* User data for callback */

    void *gsl_workspace;        /* GSL workspace (optional) */
} dyn_system_t;


/* -------------------------------
    API Functions
    ------------------------------- */

/* Allocate a new system with N particles. */
dyn_system_t *dyn_system_alloc(size_t N);

/* Free a system and all associated memory. */
void dyn_system_free(dyn_system_t *sys);

/* Set all forces in the system to zero. */
void dyn_system_zero_forces(dyn_system_t *sys);

/* Set the gravitational constant for the system. */
void dyn_system_set_gravity(dyn_system_t *sys, double G);

/* Enable or disable gravitational softening and set epsilon. */
void dyn_system_set_softening(dyn_system_t *sys, bool use_softening, double eps);

/* Set a custom force callback for the system. */
void dyn_system_set_force_callback(dyn_system_t *sys, dyn_force_cb cb, void *userdata);

/* Compute all forces in the system. */
int dyn_system_compute_forces(dyn_system_t *sys);

/* Compute pairwise gravitational forces. */
int dyn_system_compute_pairwise_gravity(dyn_system_t *sys);

/* Perform an integration step using the chosen integrator. */
int dyn_system_step(dyn_system_t *sys, double dt, dyn_integrator_t integrator);

/* Compute total kinetic energy of the system. */
double dyn_system_total_kinetic(const dyn_system_t *sys);

/* Compute total potential energy of the system. */
double dyn_system_total_potential(const dyn_system_t *sys);

/* Compute total energy of the system. */
double dyn_system_total_energy(const dyn_system_t *sys);

/* Compute total momentum of the system. */
int dyn_system_total_momentum(const dyn_system_t *sys, gsl_vector *out_p);

/* Compute center of mass position and velocity. */
int dyn_system_center_of_mass(const dyn_system_t *sys, gsl_vector *out_r, gsl_vector *out_v);

#endif /* HOLOS_DYNAMICS_H */