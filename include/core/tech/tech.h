/* tech.h - Technology data structures and functions for HOLOS
	 Copyright (C) 2025 4137314 (holos at mail.com)

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

#ifndef HOLOS_TECH_H
#define HOLOS_TECH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Technology metadata structure */
struct holos_technology {
	char *id;                /* Unique technology identifier */
	char *name;              /* Technology name */
	char *description;       /* Description of the technology */
	char *physics_model;     /* Reference to physics model used */
	char *creator;           /* Creator or author */
	double efficiency;       /* Calculated efficiency or performance */
	double cost;             /* Estimated cost */
	double impact;           /* Environmental or scientific impact */
};

/* Create a new technology */
struct holos_technology *holos_tech_create(const char *name, const char *description, const char *physics_model);

/* Load technology from disk */
struct holos_technology *holos_tech_load(const char *id);

/* Save technology to disk */
int holos_tech_save(const struct holos_technology *tech);

/* Calculate technology properties using physics model */
int holos_tech_calculate(struct holos_technology *tech);

/* Optimize technology using AI */
int holos_tech_optimize(struct holos_technology *tech);

/* Simulate technology in P2P environment */
int holos_tech_simulate_p2p(struct holos_technology *tech);

/* Free technology structure */
void holos_tech_free(struct holos_technology *tech);

#ifdef __cplusplus
}
#endif

#endif /* HOLOS_TECH_H */
