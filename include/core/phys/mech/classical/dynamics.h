
/* dynamics.h - Newtonian dynamics API for HOLOS
   Copyright (C) 2025 4137314

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
*/

/*
    Newtonian dynamics API for HOLOS (GNU Scientific Library based)
    - 3D positions, velocities, forces (gsl_vector)
    - System = collection of particles
    - API exposes force computation, integration, diagnostics, and utilities
    - Integrators: Euler, Verlet, Leapfrog, RK4, GSL ODE
    - Diagnostics: energy, momentum, center of mass, conservation checks
*/

#ifndef HOLOS_DYNAMICS_H
#define HOLOS_DYNAMICS_H



#include <gsl/gsl_vector.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


/* -------------------------------
   Type Definitions
   ------------------------------- */


typedef struct {
    double mass;          /* Mass of the particle */
    gsl_vector *pos;      /* Position (3D vector) */
    gsl_vector *vel;      /* Velocity (3D vector) */
    gsl_vector *force;    /* Net force (3D vector) */
    double charge;        /* Electric charge (optional, for EM) */
    int id;               /* Unique particle ID */
    char *tag;            /* Optional label/tag */
    double radius;        /* Particle radius (for collisions) */
    int fixed;            /* 1 if particle is fixed/immobile */
    void *userdata;       /* User-defined pointer for extensions */
} dyn_particle_t;

/* Forward declaration for the system structure. */
struct dyn_system_s;


typedef void (*dyn_force_cb)(struct dyn_system_s *system, void *userdata);


typedef enum {
    DYN_INT_EULER = 0,      /* Euler integrator */
    DYN_INT_VERLET,         /* Verlet integrator */
    DYN_INT_LEAPFROG,       /* Leapfrog integrator */
    DYN_INT_RK4,            /* Runge-Kutta 4 integrator */
    DYN_INT_GSL_ODE         /* Use GSL ODE integrator */
} dyn_integrator_t;


typedef struct dyn_system_s {
    size_t N;                   /* Number of particles */
    dyn_particle_t *particles;  /* Array of particles */

    double G;                   /* Gravitational constant */
    bool use_softening;         /* Use softening for gravity */
    double softening_eps;       /* Softening parameter */

    dyn_force_cb force_cb;      /* Custom force callback */
    void *force_cb_userdata;    /* User data for callback */

    void *gsl_workspace;        /* GSL workspace (optional) */

    double t;                   /* Current simulation time */
    double dt_last;             /* Last timestep used */
    int step_count;             /* Number of steps taken */
    char *name;                 /* System name/label */

    double temperature;         /* System temperature (optional) */
    double pressure;            /* System pressure (optional) */
    double volume;              /* System volume (optional) */
    int has_constraints;        /* 1 if constraints are present */
    void *constraints;          /* Pointer to constraints structure */
    void *userdata;             /* User-defined pointer for extensions */
} dyn_system_t;


/* -------------------------------
    API Functions
    ------------------------------- */


/* ---- Memory Management ---- */

/* Allocate a new system with N particles. */
dyn_system_t *dyn_system_alloc(size_t N);

/* Free a system and all associated memory. */
void dyn_system_free(dyn_system_t *sys);

/* Set all forces in the system to zero. */
void dyn_system_zero_forces(dyn_system_t *sys);

/* ---- System Configuration ---- */

/* Set the gravitational constant for the system. */
void dyn_system_set_gravity(dyn_system_t *sys, double G);

/* Enable or disable gravitational softening and set epsilon. */
void dyn_system_set_softening(dyn_system_t *sys, bool use_softening, double eps);

/* Set a custom force callback for the system. */
void dyn_system_set_force_callback(dyn_system_t *sys, dyn_force_cb cb, void *userdata);

/* ---- Force Computation ---- */

/* Compute all forces in the system (calls force callback or default). */
int dyn_system_compute_forces(dyn_system_t *sys);

/* Compute pairwise gravitational forces. */
int dyn_system_compute_pairwise_gravity(dyn_system_t *sys);

/* ---- Integration ---- */

/* Perform an integration step using the chosen integrator. */
int dyn_system_step(dyn_system_t *sys, double dt, dyn_integrator_t integrator);

/* Set the system's simulation time. */
void dyn_system_set_time(dyn_system_t *sys, double t);

/* Get the system's simulation time. */
double dyn_system_get_time(const dyn_system_t *sys);

/* ---- Diagnostics ---- */

/* Compute total kinetic energy of the system. */
double dyn_system_total_kinetic(const dyn_system_t *sys);

/* Compute total potential energy of the system. */
double dyn_system_total_potential(const dyn_system_t *sys);

/* Compute total energy of the system. */
double dyn_system_total_energy(const dyn_system_t *sys);

/* Compute total momentum of the system. */
int dyn_system_total_momentum(const dyn_system_t *sys, gsl_vector *out_p);

/* Compute total angular momentum of the system. */
int dyn_system_total_angular_momentum(const dyn_system_t *sys, gsl_vector *out_L);

/* Compute center of mass position and velocity. */
int dyn_system_center_of_mass(const dyn_system_t *sys, gsl_vector *out_r, gsl_vector *out_v);

/* Compute moment of inertia tensor. */
int dyn_system_moment_of_inertia(const dyn_system_t *sys, gsl_vector *origin, gsl_matrix *out_I);

/* Check conservation of energy and momentum (returns 0 if conserved). */
int dyn_system_check_conservation(const dyn_system_t *sys, double tol);

/* Compute temperature, pressure, and volume if available. */
double dyn_system_temperature(const dyn_system_t *sys);
double dyn_system_pressure(const dyn_system_t *sys);
double dyn_system_volume(const dyn_system_t *sys);

/* ---- Utility ---- */

/* Print system state to file or stdout. */
void dyn_system_print(const dyn_system_t *sys, FILE *f);

/* Print a single particle to file or stdout. */
void dyn_particle_print(const dyn_particle_t *p, FILE *f);

/* Save system state to file (ASCII or binary). */
int dyn_system_save(const dyn_system_t *sys, const char *filename);

/* Load system state from file (ASCII or binary). */
dyn_system_t *dyn_system_load(const char *filename);

/* Deep copy of a system. */
dyn_system_t *dyn_system_clone(const dyn_system_t *src);

/* Add a particle to the system (returns new index or -1 on error). */
int dyn_system_add_particle(dyn_system_t *sys, const dyn_particle_t *p);

/* Remove a particle by index (returns 0 on success). */
int dyn_system_remove_particle(dyn_system_t *sys, size_t idx);

/* Find a particle by ID (returns pointer or NULL). */
dyn_particle_t *dyn_system_find_particle(dyn_system_t *sys, int id);

/* Apply constraints if present. */
int dyn_system_apply_constraints(dyn_system_t *sys);

#endif /* HOLOS_DYNAMICS_H */