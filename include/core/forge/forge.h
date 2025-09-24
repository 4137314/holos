/* forge.h - Project manager and technology forge for HOLOS
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

#ifndef HOLOS_FORGE_H
#define HOLOS_FORGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Project metadata structure */
struct holos_project {
	char *id;                /* Unique project identifier */
	char *name;              /* Project name */
	char *description;       /* Description of the technology */
	char *database_path;     /* Path to project database */
	char *physics_file;      /* Physics calculations file */
	char *ai_file;           /* AI optimization file */
	char *p2p_file;          /* P2P simulation file */
	char *git_repo_path;     /* Path to git repository */
};

/* Create a new project */
struct holos_project *holos_project_create(const char *name, const char *description);

/* Load a project from disk */
struct holos_project *holos_project_load(const char *id);

/* Save project metadata to disk */
int holos_project_save(const struct holos_project *project);

/* Initialize git repository for project */
int holos_project_init_git(struct holos_project *project);

/* Add a file to the project git repository */
int holos_project_git_add_file(struct holos_project *project, const char *filepath);

/* Commit changes to the project git repository */
int holos_project_git_commit(struct holos_project *project, const char *message);

/* Link physics calculations to project */
int holos_project_link_physics(struct holos_project *project, const char *physics_file);

/* Link AI optimizations to project */
int holos_project_link_ai(struct holos_project *project, const char *ai_file);

/* Link P2P simulation to project */
int holos_project_link_p2p(struct holos_project *project, const char *p2p_file);

/* Free project structure */
void holos_project_free(struct holos_project *project);

#ifdef __cplusplus
}
#endif

#endif /* HOLOS_FORGE_H */
