/* include/dynamics.h
 *
 * Header for Newtonian dynamics with GNU Scientific Library (GSL).
 *
 * Design:
 *  - positions, velocities, forces are gsl_vector of dimension 3
 *  - system = collection of particles
 *  - API exposes force computation, integration, and diagnostics
 *  - integrators can be backed by GSL ODE solvers (gsl_odeiv2)
 *
 * License: (put GPL or LGPL if GNU-style project)
 */

#ifndef DYNAMICS_H
#define DYNAMICS_H

#include <gsl/gsl_vector.h>
#include <stddef.h>
#include <stdbool.h>

/* -------------------------------
   Basic Types
   ------------------------------- */

/* Particle structure */
typedef struct {
    double mass;          /* mass */
    gsl_vector *pos;      /* position (3D vector) */
    gsl_vector *vel;      /* velocity (3D vector) */
    gsl_vector *force;    /* net force (3D vector) */
} dyn_particle_t;

/* Forward declaration */
struct dyn_system_s;

/* Custom user-defined force callback */
typedef void (*dyn_force_cb)(struct dyn_system_s *system, void *userdata);

/* Integrators */
typedef enum {
    DYN_INT_EULER = 0,
    DYN_INT_VERLET,
    DYN_INT_LEAPFROG,
    DYN_INT_RK4,
    DYN_INT_GSL_ODE /* delegate to gsl_odeiv2 */
} dyn_integrator_t;

/* System structure */
typedef struct dyn_system_s {
    size_t N;                   /* number of particles */
    dyn_particle_t *particles;  /* array of particles */

    double G;                   /* gravitational constant */
    bool use_softening;
    double softening_eps;

    dyn_force_cb force_cb;
    void *force_cb_userdata;

    /* GSL workspace (optional, for ODE solvers) */
    void *gsl_workspace;
} dyn_system_t;

/* -------------------------------
   API
   ------------------------------- */

/* Allocate/free system */
dyn_system_t *dyn_system_alloc(size_t N);
void dyn_system_free(dyn_system_t *sys);

/* Zero forces */
void dyn_system_zero_forces(dyn_system_t *sys);

/* Set parameters */
void dyn_system_set_gravity(dyn_system_t *sys, double G);
void dyn_system_set_softening(dyn_system_t *sys, bool use_softening, double eps);
void dyn_system_set_force_callback(dyn_system_t *sys, dyn_force_cb cb, void *userdata);

/* Compute forces */
int dyn_system_compute_forces(dyn_system_t *sys);
int dyn_system_compute_pairwise_gravity(dyn_system_t *sys);

/* Integration step (with chosen integrator) */
int dyn_system_step(dyn_system_t *sys, double dt, dyn_integrator_t integrator);

/* Diagnostics */
double dyn_system_total_kinetic(const dyn_system_t *sys);
double dyn_system_total_potential(const dyn_system_t *sys);
double dyn_system_total_energy(const dyn_system_t *sys);

int dyn_system_total_momentum(const dyn_system_t *sys, gsl_vector *out_p);
int dyn_system_center_of_mass(const dyn_system_t *sys, gsl_vector *out_r, gsl_vector *out_v);

#endif /* DYNAMICS_H */