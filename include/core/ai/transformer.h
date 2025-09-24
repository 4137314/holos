/* transformer.h - Transformer Neural Network API for HOLOS
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
   API for Transformer neural networks.
*/

#ifndef HOLOS_TRANSFORMER_H
#define HOLOS_TRANSFORMER_H


#include <stddef.h>

/* Structure representing a transformer layer. */
typedef struct {
   size_t d_model;
   size_t num_heads;
   size_t d_ff;
   double *Wq; /* query weights */
   double *Wk; /* key weights */
   double *Wv; /* value weights */
   double *Wo; /* output weights */
   double *Wff1; /* feedforward weights 1 */
   double *Wff2; /* feedforward weights 2 */
   double *bq; /* query bias */
   double *bk; /* key bias */
   double *bv; /* value bias */
   double *bo; /* output bias */
   double *bff1; /* feedforward bias 1 */
   double *bff2; /* feedforward bias 2 */
} transformer_layer_t;

/* Structure representing the full transformer network. */
typedef struct {
   size_t num_layers;
   transformer_layer_t *layers;
} transformer_t;

/* Allocate a new transformer with the given layer parameters. */
transformer_t *transformer_alloc(const size_t *d_models, const size_t *num_heads, const size_t *d_ffs, size_t num_layers);

/* Free a transformer and all associated memory. */
void transformer_free(transformer_t *tr);

/* Forward pass: compute output for given input sequence. */
void transformer_forward(const transformer_t *tr, const double *input_seq, size_t seq_len, double *output_seq);

/* Train the transformer on a single sequence. */
void transformer_train(transformer_t *tr, const double *input_seq, const double *target_seq, size_t seq_len, double learning_rate);

/* Save/load transformer to/from file. */
int transformer_save(const transformer_t *tr, const char *filename);
transformer_t *transformer_load(const char *filename);

#endif /* HOLOS_TRANSFORMER_H */
