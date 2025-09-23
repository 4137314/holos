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


/*
   Simple harmonic motion:
   x(t) = A * cos(omega * t + phi)
   A: amplitude
   omega: angular frequency
   phi: initial phase
   t: time
   x: [out] position at time t
*/
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
void forced_harmonic_motion(double A, double F0, double k, double omega, 
                            double omega_f, double phi, double t, gsl_vector *x);

#endif /* HOLOS_OSCILLATIONS_H */