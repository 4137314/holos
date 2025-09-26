/* oscillations.h - Classical oscillations API for HOLOS
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
   API for classical mechanical oscillations using GNU Scientific Library (GSL).
   Supports simple harmonic, damped, and forced oscillations.
*/

#ifndef HOLOS_OSCILLATIONS_H
#define HOLOS_OSCILLATIONS_H


#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <stddef.h>
/* Structure representing the state of a single oscillator (position, velocity, acceleration, time, tag, etc.) */
typedef struct {
   gsl_vector *x;        /* Position vector */
   gsl_vector *v;        /* Velocity vector */
   gsl_vector *a;        /* Acceleration vector */
   double t;             /* Time */
   double energy;        /* Instantaneous energy */
   char *tag;            /* Optional label/tag */
   void *userdata;       /* User-defined pointer for extensions */
} osc_state;

/* Structure representing a system of oscillators */
typedef struct {
   size_t n;             /* Number of oscillators */
   osc_state *states;    /* Array of oscillator states */
   char *name;           /* System name/label */
   double t;             /* Current simulation time */
   int step_count;       /* Number of steps taken */
   void *userdata;       /* User-defined pointer for extensions */
} osc_system;


/*
   Simple harmonic motion:
   x(t) = A * cos(omega * t + phi)
   A: amplitude
   omega: angular frequency
   phi: initial phase
   t: time
   x: [out] position at time t
*/
/* Simple harmonic motion: x(t) = A * cos(omega * t + phi) */
void harmonic_motion(double A, double omega, double phi, double t, gsl_vector *x);


/*
   Velocity in simple harmonic motion:
   v(t) = -A * omega * sin(omega * t + phi)
   A: amplitude
   omega: angular frequency
   phi: initial phase
   t: time
   v: [out] velocity at time t
*/
/* Velocity in simple harmonic motion: v(t) = -A * omega * sin(omega * t + phi) */
void harmonic_velocity(double A, double omega, double phi, double t, gsl_vector *v);


/*
   Acceleration in simple harmonic motion:
   a(t) = -A * omega^2 * cos(omega * t + phi)
   A: amplitude
   omega: angular frequency
   phi: initial phase
   t: time
   a: [out] acceleration at time t
*/
/* Acceleration in simple harmonic motion: a(t) = -A * omega^2 * cos(omega * t + phi) */
void harmonic_acceleration(double A, double omega, double phi, double t, gsl_vector *a);


/*
   Damped harmonic motion:
   x(t) = A * exp(-gamma * t) * cos(omega_d * t + phi)
   A: amplitude
   gamma: damping coefficient
   omega_d: damped angular frequency
   phi: initial phase
   t: time
   x: [out] position at time t
*/
/* Damped harmonic motion: x(t) = A * exp(-gamma * t) * cos(omega_d * t + phi) */
void damped_harmonic_motion(double A, double gamma, double omega_d,
                            double phi, double t, gsl_vector *x);


/*
   Forced harmonic motion (sinusoidal forcing):
   x(t) = A * cos(omega * t + phi) + (F0 / k) * cos(omega_f * t)
   A: amplitude
   F0: external force
   k: spring constant
   omega: angular frequency
   omega_f: driving frequency
   phi: initial phase
   t: time
   x: [out] position at time t
*/
/* Forced harmonic motion (sinusoidal forcing):
   x(t) = A * cos(omega * t + phi) + (F0 / k) * cos(omega_f * t) */
void forced_harmonic_motion(double A, double F0, double k, double omega, 
                            double omega_f, double phi, double t, gsl_vector *x);

/* ---- Diagnostics ---- */

/* Compute total energy of a harmonic oscillator: E = 0.5 m omega^2 A^2 */
double oscillator_total_energy(double mass, double omega, double A);

/* Compute period of a simple harmonic oscillator: T = 2π / omega */
double oscillator_period(double omega);

/* Compute damping ratio: zeta = gamma / (2 * sqrt(k/m)) */
double oscillator_damping_ratio(double gamma, double k, double mass);

/* Compute quality factor: Q = 1 / (2 * zeta) */
double oscillator_quality_factor(double zeta);

/* ---- Utilities ---- */

/* Allocate an oscillator state (dim: vector dimension) */
osc_state *osc_state_alloc(size_t dim);

/* Free an oscillator state */
void osc_state_free(osc_state *state);

/* Print an oscillator state to file or stdout */
void osc_state_print(const osc_state *state, FILE *f);

/* Allocate a system of n oscillators (dim: vector dimension) */
osc_system *osc_system_alloc(size_t n, size_t dim);

/* Free a system of oscillators */
void osc_system_free(osc_system *sys);

/* Print a system of oscillators to file or stdout */
void osc_system_print(const osc_system *sys, FILE *f);

/* Add an oscillator state to the system (returns new index or -1 on error) */
int osc_system_add_state(osc_system *sys, const osc_state *state);

/* Remove an oscillator state by index (returns 0 on success) */
int osc_system_remove_state(osc_system *sys, size_t idx);

/* Find an oscillator state by tag (returns pointer or NULL) */
osc_state *osc_system_find_state(osc_system *sys, const char *tag);

#endif /* HOLOS_OSCILLATIONS_H */