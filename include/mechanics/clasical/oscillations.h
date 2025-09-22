#ifndef OSCILLATIONS_H
#define OSCILLATIONS_H

#include <gsl/gsl_vector.h>

/* oscillations.h
   Main functions for classical mechanical oscillations.
   Supports simple harmonic motion, damped oscillations, and forced oscillations.
*/

/* Simple harmonic motion
   x(t) = A * cos(omega * t + phi)
   A: amplitude
   omega: angular frequency
   phi: initial phase
*/
void harmonic_motion(double A, double omega, double phi, double t, gsl_vector *x);

/* Velocity in simple harmonic motion
   v(t) = -A * omega * sin(omega * t + phi)
*/
void harmonic_velocity(double A, double omega, double phi, double t, gsl_vector *v);

/* Acceleration in simple harmonic motion
   a(t) = -A * omega^2 * cos(omega * t + phi)
*/
void harmonic_acceleration(double A, double omega, double phi, double t, gsl_vector *a);

/* Damped harmonic motion
   x(t) = A * exp(-gamma * t) * cos(omega_d * t + phi)
   gamma: damping coefficient
   omega_d: damped angular frequency
*/
void damped_harmonic_motion(double A, double gamma, double omega_d,
                            double phi, double t, gsl_vector *x);

/* Forced harmonic motion (sinusoidal forcing)
   x(t) = A * cos(omega * t + phi) + (F0 / k) * cos(omega_f * t)
   F0: external force
   k: spring constant
   omega_f: driving frequency
*/
void forced_harmonic_motion(double A, double F0, double k, double omega, 
                            double omega_f, double phi, double t, gsl_vector *x);

#endif /* OSCILLATIONS_H */