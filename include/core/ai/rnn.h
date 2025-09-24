/* rnn.h - Recurrent Neural Network API for HOLOS
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
   API for Recurrent Neural Networks (RNN).
*/

#ifndef HOLOS_RNN_H
#define HOLOS_RNN_H


#include <stddef.h>

/* Structure representing a recurrent layer. */
typedef struct {
   size_t input_size;
   size_t hidden_size;
   double *Wxh;   /* input-to-hidden weights */
   double *Whh;   /* hidden-to-hidden weights */
   double *Why;   /* hidden-to-output weights */
   double *bh;    /* hidden bias */
   double *by;    /* output bias */
} rnn_layer_t;

/* Structure representing the full RNN network. */
typedef struct {
   size_t num_layers;
   rnn_layer_t *layers;
   double *hidden_state;
} rnn_t;

/* Allocate a new RNN with the given layer sizes. */
rnn_t *rnn_alloc(const size_t *input_sizes, const size_t *hidden_sizes, size_t num_layers);

/* Free an RNN and all associated memory. */
void rnn_free(rnn_t *rnn);

/* Forward pass: compute output for given input sequence. */
void rnn_forward(const rnn_t *rnn, const double *input_seq, size_t seq_len, double *output_seq);

/* Train the RNN on a single sequence. */
void rnn_train(rnn_t *rnn, const double *input_seq, const double *target_seq, size_t seq_len, double learning_rate);

/* Save/load RNN to/from file. */
int rnn_save(const rnn_t *rnn, const char *filename);
rnn_t *rnn_load(const char *filename);

#endif /* HOLOS_RNN_H */
