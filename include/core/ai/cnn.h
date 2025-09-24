/* cnn.h - Convolutional Neural Network API for HOLOS
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
   API for Convolutional Neural Networks (CNN).
*/

#ifndef HOLOS_CNN_H
#define HOLOS_CNN_H


#include <stddef.h>

/* Structure representing a convolutional layer. */
typedef struct {
   size_t input_channels;
   size_t output_channels;
   size_t kernel_size;
   double *kernels;   /* kernels: output_channels x input_channels x kernel_size x kernel_size */
   double *biases;    /* biases: output_channels */
} cnn_layer_t;

/* Structure representing the full CNN network. */
typedef struct {
   size_t num_layers;
   cnn_layer_t *layers;
} cnn_t;

/* Allocate a new CNN with the given layer parameters. */
cnn_t *cnn_alloc(const size_t *input_channels, const size_t *output_channels, const size_t *kernel_sizes, size_t num_layers);

/* Free a CNN and all associated memory. */
void cnn_free(cnn_t *cnn);

/* Forward pass: compute output for given input. */
void cnn_forward(const cnn_t *cnn, const double *input, double *output);

/* Train the CNN on a single example. */
void cnn_train(cnn_t *cnn, const double *input, const double *target, double learning_rate);

/* Save/load CNN to/from file. */
int cnn_save(const cnn_t *cnn, const char *filename);
cnn_t *cnn_load(const char *filename);

#endif /* HOLOS_CNN_H */
