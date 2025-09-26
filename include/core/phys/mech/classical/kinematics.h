/* kinematics.h - Classical kinematics API for HOLOS
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
   API for classical Newtonian kinematics using GNU Scientific Library (GSL).
   All functions use GSL vectors.
*/

#ifndef HOLOS_KINEMATICS_H
#define HOLOS_KINEMATICS_H


#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <stddef.h>
/* Structure representing a kinematic state (position, velocity, acceleration, time, tag, etc.) */
typedef struct {
   gsl_vector *pos;      /* Position vector */
   gsl_vector *vel;      /* Velocity vector */
   gsl_vector *acc;      /* Acceleration vector */
   double t;             /* Time */
   char *tag;            /* Optional label/tag */
   void *userdata;       /* User-defined pointer for extensions */
} kin_state;

/* Structure representing a trajectory (sequence of kinematic states) */
typedef struct {
   size_t n;             /* Number of states */
   kin_state *states;    /* Array of states */
   char *name;           /* Trajectory name/label */
   void *userdata;       /* User-defined pointer for extensions */
} kin_trajectory;


/*
   Uniform linear motion:
   x(t) = x0 + v * t
   x0: initial position vector
   v: velocity vector
   t: time
   x: [out] position at time t
*/
/* Uniform linear motion: x(t) = x0 + v * t */
void motion_uniform(const gsl_vector *x0, const gsl_vector *v, double t, gsl_vector *x);


/*
   Uniformly accelerated linear motion:
   x(t) = x0 + v0 * t + 0.5 * a * t^2
   x0: initial position
   v0: initial velocity
   a: acceleration
   t: time
   x: [out] position at time t
*/
/* Uniformly accelerated linear motion: x(t) = x0 + v0 * t + 0.5 * a * t^2 */
void motion_accelerated(const gsl_vector *x0, const gsl_vector *v0,
                        const gsl_vector *a, double t, gsl_vector *x);


/*
   Velocity in uniformly accelerated motion:
   v(t) = v0 + a * t
   v0: initial velocity
   a: acceleration
   t: time
   v: [out] velocity at time t
*/
/* Velocity in uniformly accelerated motion: v(t) = v0 + a * t */
void velocity_accelerated(const gsl_vector *v0, const gsl_vector *a,
                          double t, gsl_vector *v);


/*
   Uniform circular motion in 2D:
   x(t) = (R cos(ωt), R sin(ωt))
   R: radius
   omega: angular velocity
   t: time
   x: [out] position at time t
*/
/* Uniform circular motion in 2D: x(t) = (R cos(ωt), R sin(ωt)) */
void motion_circular(double R, double omega, double t, gsl_vector *x);


/*
   Centripetal acceleration in uniform circular motion:
   a = v^2 / R = ω^2 R
   omega: angular velocity
   R: radius
   Returns centripetal acceleration.
*/
/* Centripetal acceleration in uniform circular motion: a = ω^2 R */
double centripetal_acceleration(double omega, double R);


/*
   Angular velocity from period:
   ω = 2π / T
   period: period of revolution
   Returns angular velocity.
*/
/* Angular velocity from period: ω = 2π / T */
double angular_velocity(double period);


/*
   General time law (explicit Euler integration):
   Updates position and velocity given dt.
   pos: [in/out] position vector
   vel: [in/out] velocity vector
   acc: acceleration vector
   dt: time step
*/
/* General time law (explicit Euler integration): updates position and velocity given dt */
void euler_step(gsl_vector *pos, gsl_vector *vel,
                const gsl_vector *acc, double dt);

/* ---- Diagnostics ---- */

/* Compute displacement vector: Δx = x2 - x1 */
void kinematics_displacement(const gsl_vector *x1, const gsl_vector *x2, gsl_vector *dx);

/* Compute average velocity: v_avg = Δx / Δt */
void kinematics_average_velocity(const gsl_vector *x1, const gsl_vector *x2, double t1, double t2, gsl_vector *v_avg);

/* Compute average acceleration: a_avg = Δv / Δt */
void kinematics_average_acceleration(const gsl_vector *v1, const gsl_vector *v2, double t1, double t2, gsl_vector *a_avg);

/* Compute trajectory length (sum of segment lengths) */
double kinematics_trajectory_length(const kin_trajectory *traj);

/* ---- Utilities ---- */

/* Allocate a kinematic state (dim: vector dimension) */
kin_state *kin_state_alloc(size_t dim);

/* Free a kinematic state */
void kin_state_free(kin_state *state);

/* Print a kinematic state to file or stdout */
void kin_state_print(const kin_state *state, FILE *f);

/* Allocate a trajectory with n states (dim: vector dimension) */
kin_trajectory *kin_trajectory_alloc(size_t n, size_t dim);

/* Free a trajectory */
void kin_trajectory_free(kin_trajectory *traj);

/* Print a trajectory to file or stdout */
void kin_trajectory_print(const kin_trajectory *traj, FILE *f);

/* Add a state to a trajectory (returns new index or -1 on error) */
int kin_trajectory_add_state(kin_trajectory *traj, const kin_state *state);

/* Remove a state by index (returns 0 on success) */
int kin_trajectory_remove_state(kin_trajectory *traj, size_t idx);

/* Find a state by tag (returns pointer or NULL) */
kin_state *kin_trajectory_find_state(kin_trajectory *traj, const char *tag);

#endif /* HOLOS_KINEMATICS_H */