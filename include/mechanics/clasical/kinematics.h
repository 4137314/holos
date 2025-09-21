#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <gsl/gsl_vector.h>

/* kinematics.h
   Funzioni principali di cinematica classica newtoniana.
   Le implementazioni numeriche usano GNU GSL per la gestione di vettori.
*/

/* 
   Moto rettilineo uniforme
   x(t) = x0 + v * t
*/
void motion_uniform(const gsl_vector *x0, const gsl_vector *v, double t, gsl_vector *x);

/*
   Moto rettilineo uniformemente accelerato
   x(t) = x0 + v0 * t + 0.5 * a * t^2
*/
void motion_accelerated(const gsl_vector *x0, const gsl_vector *v0,
                        const gsl_vector *a, double t, gsl_vector *x);

/*
   Velocità nel moto uniformemente accelerato
   v(t) = v0 + a * t
*/
void velocity_accelerated(const gsl_vector *v0, const gsl_vector *a,
                          double t, gsl_vector *v);

/*
   Moto circolare uniforme in 2D
   x(t) = (R cos(ωt), R sin(ωt))
*/
void motion_circular(double R, double omega, double t, gsl_vector *x);

/*
   Accelerazione centripeta nel moto circolare uniforme
   a = v^2 / R = ω^2 R
*/
double centripetal_acceleration(double omega, double R);

/*
   Velocità angolare da periodo
   ω = 2π / T
*/
double angular_velocity(double period);

/*
   Legge oraria generale (integrazione di Eulero esplicito)
   Aggiorna posizione e velocità dato dt
*/
void euler_step(gsl_vector *pos, gsl_vector *vel,
                const gsl_vector *acc, double dt);

#endif /* KINEMATICS_H */