/* kinematics.h - Classical kinematics API for HOLOS
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
   API for classical Newtonian kinematics using GNU Scientific Library (GSL).
   All functions use GSL vectors.
*/

#ifndef HOLOS_KINEMATICS_H
#define HOLOS_KINEMATICS_H

#include <gsl/gsl_vector.h>


/*
   Uniform linear motion:
   x(t) = x0 + v * t
   x0: initial position vector
   v: velocity vector
   t: time
   x: [out] position at time t
*/
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
void motion_circular(double R, double omega, double t, gsl_vector *x);


/*
   Centripetal acceleration in uniform circular motion:
   a = v^2 / R = ω^2 R
   omega: angular velocity
   R: radius
   Returns centripetal acceleration.
*/
double centripetal_acceleration(double omega, double R);


/*
   Angular velocity from period:
   ω = 2π / T
   period: period of revolution
   Returns angular velocity.
*/
double angular_velocity(double period);


/*
   General time law (explicit Euler integration):
   Updates position and velocity given dt.
   pos: [in/out] position vector
   vel: [in/out] velocity vector
   acc: acceleration vector
   dt: time step
*/
void euler_step(gsl_vector *pos, gsl_vector *vel,
                const gsl_vector *acc, double dt);

#endif /* HOLOS_KINEMATICS_H */