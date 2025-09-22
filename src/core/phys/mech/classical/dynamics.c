#include <gsl/gsl_vector.h>

/* F = m * a */
void compute_force(double mass, const gsl_vector *acc, gsl_vector *force) {
    gsl_vector_memcpy(force, acc);
    gsl_vector_scale(force, mass);
}

/* a = F / m */
void compute_acceleration(const gsl_vector *force, double mass, gsl_vector *acc) {
    gsl_vector_memcpy(acc, force);
    gsl_vector_scale(acc, 1.0 / mass);
}

/* Aggiorna posizione e velocità con integrazione di Eulero */
void euler_step(gsl_vector *pos, gsl_vector *vel, const gsl_vector *acc, double dt) {
    gsl_vector *tmp = gsl_vector_alloc(pos->size);

    /* v = v + a*dt */
    gsl_vector_memcpy(tmp, acc);
    gsl_vector_scale(tmp, dt);
    gsl_vector_add(vel, tmp);

    /* x = x + v*dt */
    gsl_vector_memcpy(tmp, vel);
    gsl_vector_scale(tmp, dt);
    gsl_vector_add(pos, tmp);

    gsl_vector_free(tmp);
}

/* Energia cinetica */
double kinetic_energy(double mass, const gsl_vector *vel) {
    double v2;
    gsl_blas_ddot(vel, vel, &v2);
    return 0.5 * mass * v2;
}

/* Quantità di moto */
void momentum(double mass, const gsl_vector *vel, gsl_vector *p) {
    gsl_vector_memcpy(p, vel);
    gsl_vector_scale(p, mass);
}

/* Integrazione di Verlet */
void verlet_step(gsl_vector *pos, gsl_vector *vel,
                 const gsl_vector *acc, double dt) {
    gsl_vector *tmp = gsl_vector_alloc(pos->size);

    /* x = x + v*dt + 0.5*a*dt^2 */
    gsl_vector_memcpy(tmp, acc);
    gsl_vector_scale(tmp, 0.5 * dt * dt);
    gsl_vector_add(tmp, vel);
    gsl_vector_scale(tmp, dt);
    gsl_vector_add(pos, tmp);

    /* v = v + a*dt */
    gsl_vector_memcpy(tmp, acc);
    gsl_vector_scale(tmp, dt);
    gsl_vector_add(vel, tmp);

    gsl_vector_free(tmp);
}
