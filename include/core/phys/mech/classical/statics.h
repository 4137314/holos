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
#include <gsl/gsl_matrix.h>
#include <stddef.h>
/* Structure representing a force (vector, application point, tag, etc.) */
typedef struct {
   gsl_vector *F;        /* Force vector */
   gsl_vector *r;        /* Application point */
   char *tag;            /* Optional label/tag */
   void *userdata;       /* User-defined pointer for extensions */
} statics_force;

/* Structure representing a torque (vector, application point, tag, etc.) */
typedef struct {
   gsl_vector *M;        /* Torque vector */
   gsl_vector *r;        /* Application point */
   char *tag;            /* Optional label/tag */
   void *userdata;       /* User-defined pointer for extensions */
} statics_torque;

/* Structure representing a statics system (forces, torques, constraints, etc.) */
typedef struct {
   size_t n_forces;          /* Number of forces */
   statics_force *forces;    /* Array of forces */
   size_t n_torques;         /* Number of torques */
   statics_torque *torques;  /* Array of torques */
   int has_constraints;      /* 1 if constraints are present */
   void *constraints;        /* Pointer to constraints structure */
   char *name;               /* System name/label */
   void *userdata;           /* User-defined pointer for extensions */
} statics_system;


/*
   Vector sum of multiple forces.
   forces: array of force vectors
   n: number of forces
   result: [out] resultant force
*/
/* Vector sum of multiple forces. */
void resultant_force(const gsl_vector **forces, size_t n, gsl_vector *result);

/* Vector sum of all forces in a statics_system. */
void statics_system_resultant_force(const statics_system *sys, gsl_vector *result);


/*
   Torque of a force with respect to a point.
   r: position vector of application point
   F: force vector
   M: [out] torque (r x F)
*/
/* Torque of a force with respect to a point: M = r x F */
void torque(const gsl_vector *r, const gsl_vector *F, gsl_vector *M);

/* Total torque for a system of forces about a reference point. */
void total_torque(const gsl_vector **r, const gsl_vector **F, size_t n, gsl_vector *M);

/* Total torque for all forces in a statics_system about a reference point. */
void statics_system_total_torque(const statics_system *sys, const gsl_vector *ref, gsl_vector *M);


/*
   Check translational equilibrium.
   forces: array of force vectors
   n: number of forces
   Returns 1 if equilibrium, 0 otherwise.
*/
/* Check translational equilibrium for an array of forces. */
int translational_equilibrium(const gsl_vector **forces, size_t n);

/* Check translational equilibrium for a statics_system. */
int statics_system_translational_equilibrium(const statics_system *sys);


/*
   Check rotational equilibrium.
   torques: array of torque vectors
   n: number of torques
   Returns 1 if equilibrium, 0 otherwise.
*/
/* Check rotational equilibrium for an array of torques. */
int rotational_equilibrium(const gsl_vector **torques, size_t n);

/* Check rotational equilibrium for a statics_system. */
int statics_system_rotational_equilibrium(const statics_system *sys);
#endif /* HOLOS_STATICS_H */

/* ---- Diagnostics ---- */

/* Compute the moment arm for a force: d = |r x F| / |F| */
double moment_arm(const gsl_vector *r, const gsl_vector *F);

/* Compute the static friction force: F_s = mu_s * N */
double static_friction(double mu_s, double N);

/* Compute the normal force for a given weight and angle. */
double normal_force(double weight, double angle);

/* ---- Utilities ---- */

/* Allocate a statics_force (dim: vector dimension) */
statics_force *statics_force_alloc(size_t dim);

/* Free a statics_force */
void statics_force_free(statics_force *f);

/* Print a statics_force to file or stdout */
void statics_force_print(const statics_force *f, FILE *fp);

/* Allocate a statics_torque (dim: vector dimension) */
statics_torque *statics_torque_alloc(size_t dim);

/* Free a statics_torque */
void statics_torque_free(statics_torque *t);

/* Print a statics_torque to file or stdout */
void statics_torque_print(const statics_torque *t, FILE *fp);

/* Allocate a statics_system with n_forces and n_torques (dim: vector dimension) */
statics_system *statics_system_alloc(size_t n_forces, size_t n_torques, size_t dim);

/* Free a statics_system */
void statics_system_free(statics_system *sys);

/* Print a statics_system to file or stdout */
void statics_system_print(const statics_system *sys, FILE *fp);

/* Add a force to a statics_system (returns new index or -1 on error) */
int statics_system_add_force(statics_system *sys, const statics_force *f);

/* Remove a force by index (returns 0 on success) */
int statics_system_remove_force(statics_system *sys, size_t idx);

/* Find a force by tag (returns pointer or NULL) */
statics_force *statics_system_find_force(statics_system *sys, const char *tag);

/* Add a torque to a statics_system (returns new index or -1 on error) */
int statics_system_add_torque(statics_system *sys, const statics_torque *t);

/* Remove a torque by index (returns 0 on success) */
int statics_system_remove_torque(statics_system *sys, size_t idx);

/* Find a torque by tag (returns pointer or NULL) */
statics_torque *statics_system_find_torque(statics_system *sys, const char *tag);

#endif /* HOLOS_STATICS_H */