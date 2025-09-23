/* statics.h - Classical statics API for HOLOS
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
   API for classical statics problems using GNU Scientific Library (GSL).
   Supports resultant force, torque, and equilibrium checks.
*/

#ifndef HOLOS_STATICS_H
#define HOLOS_STATICS_H

#include <gsl/gsl_vector.h>


/*
   Vector sum of multiple forces.
   forces: array of force vectors
   n: number of forces
   result: [out] resultant force
*/
void resultant_force(const gsl_vector **forces, size_t n, gsl_vector *result);


/*
   Torque of a force with respect to a point.
   r: position vector of application point
   F: force vector
   M: [out] torque (r x F)
*/
void torque(const gsl_vector *r, const gsl_vector *F, gsl_vector *M);


/*
   Check translational equilibrium.
   forces: array of force vectors
   n: number of forces
   Returns 1 if equilibrium, 0 otherwise.
*/
int translational_equilibrium(const gsl_vector **forces, size_t n);


/*
   Check rotational equilibrium.
   torques: array of torque vectors
   n: number of torques
   Returns 1 if equilibrium, 0 otherwise.
*/
int rotational_equilibrium(const gsl_vector **torques, size_t n);

#endif /* HOLOS_STATICS_H */