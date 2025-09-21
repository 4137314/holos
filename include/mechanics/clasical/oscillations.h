#ifndef OSCILLATIONS_H
#define OSCILLATIONS_H

#include <gsl/gsl_vector.h>

/* oscillations.h
   Funzioni principali per oscillazioni meccaniche classiche.
   Supporta moto armonico semplice, smorzato e forzato.
*/

/* Moto armonico semplice
   x(t) = A * cos(omega * t + phi)
   A: ampiezza
   omega: pulsazione
   phi: fase iniziale
*/
void harmonic_motion(double A, double omega, double phi, double t, gsl_vector *x);

/* Velocità nel moto armonico semplice
   v(t) = -A * omega * sin(omega * t + phi)
*/
void harmonic_velocity(double A, double omega, double phi, double t, gsl_vector *v);

/* Accelerazione nel moto armonico semplice
   a(t) = -A * omega^2 * cos(omega * t + phi)
*/
void harmonic_acceleration(double A, double omega, double phi, double t, gsl_vector *a);

/* Moto armonico smorzato (damping)
   x(t) = A * exp(-gamma * t) * cos(omega_d * t + phi)
   gamma: coefficiente di smorzamento
   omega_d: pulsazione smorzata
*/
void damped_harmonic_motion(double A, double gamma, double omega_d,
                            double phi, double t, gsl_vector *x);

/* Moto armonico forzato (sinusoidale)
   x(t) = A * cos(omega * t) + (F0 / k) * cos(omega_f * t)
   F0: forza esterna, k: costante elastica, omega_f: frequenza forzante
*/
void forced_harmonic_motion(double A, double F0, double k, double omega, 
                            double omega_f, double phi, double t, gsl_vector *x);

#endif /* OSCILLATIONS_H */