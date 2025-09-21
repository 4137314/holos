#ifndef STATICS_H
#define STATICS_H

#include <gsl/gsl_vector.h>

/* statics.h
   Funzioni principali per problemi di statica classica.
   Supporta calcolo di forze risultanti, momento e bilanciamento.
*/

/* Somma vettoriale di più forze
   forces: array di vettori forza
   n: numero di forze
   result: forza risultante
*/
void resultant_force(const gsl_vector **forces, size_t n, gsl_vector *result);

/* Momento di una forza rispetto a un punto
   r: vettore posizione punto di applicazione
   F: vettore forza
   M: momento risultante (r x F)
*/
void torque(const gsl_vector *r, const gsl_vector *F, gsl_vector *M);

/* Controllo equilibrio traslazionale
   forces: array di vettori forza
   n: numero di forze
   return 0 se risultante != 0, 1 se equilibrio traslazionale
*/
int translational_equilibrium(const gsl_vector **forces, size_t n);

/* Controllo equilibrio rotazionale
   torques: array di momenti vettoriali
   n: numero di momenti
   return 0 se somma momenti != 0, 1 se equilibrio rotazionale
*/
int rotational_equilibrium(const gsl_vector **torques, size_t n);

#endif /* STATICS_H */