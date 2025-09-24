/* mlp.h - Multilayer Perceptron Neural Network API for HOLOS
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
   API for Multilayer Perceptron (MLP) neural networks.
*/

#ifndef HOLOS_MLP_H
#define HOLOS_MLP_H


#include <stddef.h>

/* Structure representing a single layer in the MLP. */
typedef struct {
   size_t input_size;
   size_t output_size;
   double *weights;   /* weights: output_size x input_size */
   double *biases;    /* biases: output_size */
} mlp_layer_t;

/* Structure representing the full MLP network. */
typedef struct {
   size_t num_layers;
   mlp_layer_t *layers;
   double *activations; /* optional: buffer for activations */
} mlp_t;

/* Allocate a new MLP with the given layer sizes. */
mlp_t *mlp_alloc(const size_t *layer_sizes, size_t num_layers);

/* Free an MLP and all associated memory. */
void mlp_free(mlp_t *mlp);

/* Forward pass: compute output for given input. */
void mlp_forward(const mlp_t *mlp, const double *input, double *output);

/* Train the MLP on a single example (stochastic gradient descent). */
void mlp_train(mlp_t *mlp, const double *input, const double *target, double learning_rate);

/* Save/load MLP to/from file. */
int mlp_save(const mlp_t *mlp, const char *filename);
mlp_t *mlp_load(const char *filename);

#endif /* HOLOS_MLP_H */
