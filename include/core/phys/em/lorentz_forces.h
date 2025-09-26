/* lorentz_forces.h - Lorentz force calculations for HOLOS
   Copyright (C) 2025 4137314

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
*/

#ifndef HOLOS_LORENTZ_FORCES_H
#define HOLOS_LORENTZ_FORCES_H

#include <gsl/gsl_vector.h>
#include <stddef.h>

/* Structure for a charged particle (classical or relativistic) */
typedef struct {
    double q;              /* Charge [C] */
    double m;              /* Mass [kg] */
    gsl_vector *pos;       /* Position [m] */
    gsl_vector *vel;       /* Velocity [m/s] */
    gsl_vector *acc;       /* Acceleration [m/s^2] */
    double gamma;          /* Lorentz factor (relativistic) */
    double energy;         /* Total energy [J] */
    char *tag;             /* Optional label */
    void *userdata;        /* User-defined pointer */
} lorentz_particle;

/* Structure for a system of charged particles, with trajectory and diagnostics */
typedef struct {
    size_t n;
    lorentz_particle *particles;
    char *name;
    double t;                  /* Current simulation time */
    int step_count;            /* Number of steps taken */
    void *trajectory;          /* Pointer to trajectory/history structure */
    void *diagnostics;         /* Pointer to diagnostics structure */
    void *userdata;
} lorentz_system;

/* Compute Lorentz force: F = q (E + v x B) */
void lorentz_force(const gsl_vector *E, const gsl_vector *B,
                   const gsl_vector *v, double q, gsl_vector *F);

/* Compute relativistic Lorentz force: d/dt (gamma m v) = q (E + v x B) */
void lorentz_force_relativistic(const lorentz_particle *p,
                                const gsl_vector *E, const gsl_vector *B,
                                gsl_vector *F);

/* Compute Lorentz force for a particle (classical) */
void lorentz_force_particle(const lorentz_particle *p,
                           const gsl_vector *E, const gsl_vector *B,
                           gsl_vector *F);

/* Compute Lorentz force for all particles in a system (classical) */
void lorentz_force_system(const lorentz_system *sys,
                         const gsl_vector *E, const gsl_vector *B,
                         gsl_vector **F_array);

/* Compute relativistic Lorentz force for all particles in a system */
void lorentz_force_system_relativistic(const lorentz_system *sys,
                                       const gsl_vector *E, const gsl_vector *B,
                                       gsl_vector **F_array);

/* Update gamma and energy for a particle (relativistic) */
void lorentz_update_gamma_energy(lorentz_particle *p);

/* Batch update gamma and energy for all particles in a system */
void lorentz_system_update_gamma_energy(lorentz_system *sys);

/* Compute total energy of a system */
double lorentz_system_total_energy(const lorentz_system *sys);

/* Compute center of charge and center of mass */
void lorentz_system_center_of_charge(const lorentz_system *sys, gsl_vector *out_r);
void lorentz_system_center_of_mass(const lorentz_system *sys, gsl_vector *out_r);


/* Utility: allocate/free particle and system */
lorentz_particle *lorentz_particle_alloc(void);
void lorentz_particle_free(lorentz_particle *p);
lorentz_system *lorentz_system_alloc(size_t n);
void lorentz_system_free(lorentz_system *sys);

/* Clone a particle or system (deep copy) */
lorentz_particle *lorentz_particle_clone(const lorentz_particle *src);
lorentz_system *lorentz_system_clone(const lorentz_system *src);

/* Add/remove/find particle in a system */
int lorentz_system_add_particle(lorentz_system *sys, const lorentz_particle *p);
int lorentz_system_remove_particle(lorentz_system *sys, size_t idx);
lorentz_particle *lorentz_system_find_particle(lorentz_system *sys, const char *tag);

/* Print particle/system to file or stdout */
void lorentz_particle_print(const lorentz_particle *p, FILE *f);
void lorentz_system_print(const lorentz_system *sys, FILE *f);

/* Save/load system state to file (ASCII or binary) */
int lorentz_system_save(const lorentz_system *sys, const char *filename);
lorentz_system *lorentz_system_load(const char *filename);

#endif /* HOLOS_LORENTZ_FORCES_H */
