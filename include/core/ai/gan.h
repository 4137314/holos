/* gan.h - Generative Adversarial Network API for HOLOS
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
   API for Generative Adversarial Networks (GAN).
*/

#ifndef HOLOS_GAN_H
#define HOLOS_GAN_H


#include <stddef.h>

/* Structure representing a simple GAN (generator and discriminator as MLPs). */
typedef struct {
   void *generator;      /* pointer to MLP or other NN struct */
   void *discriminator;  /* pointer to MLP or other NN struct */
} gan_t;

/* Allocate a new GAN with given generator and discriminator. */
gan_t *gan_alloc(void *generator, void *discriminator);

/* Free a GAN and all associated memory. */
void gan_free(gan_t *gan);

/* Train the GAN on a batch of data. */
void gan_train(gan_t *gan, const double *real_data, size_t batch_size, double learning_rate);

/* Generate a sample from the GAN. */
void gan_generate(const gan_t *gan, double *output);

/* Save/load GAN to/from file. */
int gan_save(const gan_t *gan, const char *filename);
gan_t *gan_load(const char *filename);

#endif /* HOLOS_GAN_H */
