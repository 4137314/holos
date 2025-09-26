/* forge.h - Project manager and technology forge for HOLOS
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

#ifndef HOLOS_FORGE_H
#define HOLOS_FORGE_H

/* Versioning macros for ABI/API compatibility */
#define HOLOS_FORGE_VERSION_MAJOR 1
#define HOLOS_FORGE_VERSION_MINOR 0
#define HOLOS_FORGE_VERSION_PATCH 0
#define HOLOS_FORGE_VERSION_STRING "1.0.0"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * struct holos_project - Advanced project metadata and management structure
 *
 * This structure encapsulates all metadata and resource links for a HOLOS project.
 * All fields are dynamically allocated strings unless otherwise noted.
 *
 * Fields:
 *   id                Unique project identifier (string, required)
 *   name              Project name (string, required)
 *   description       Description of the technology/project (string)
 *   database_path     Path to project database (string)
 *   physics_file      Physics calculations file (string)
 *   ai_file           AI optimization file (string)
 *   p2p_file          P2P simulation file (string)
 *   git_repo_path     Path to git repository (string)
 *   version           Project version string (string)
 *   created_at        ISO 8601 creation timestamp (string)
 *   updated_at        ISO 8601 last update timestamp (string)
 *   owner             Project owner/PI (string)
 *   organization      Affiliated organization (string)
 *   compliance        Compliance/certification string (e.g., ISO, GDPR) (string)
 *   sustainability    Sustainability/circularity notes (string)
 *   provenance        Provenance/traceability string (string)
 *   analytics_path    Path to analytics/reporting file (string)
 *   digital_twin_path Path to digital twin model (string)
 *   diagnostics       Diagnostics/status string (string)
 *   tags              Project tags/keywords (comma-separated, string)
 *   reserved          Reserved for future use (void pointer)
 */
/**
 * holos_project_state - Project lifecycle state enumeration
 */
typedef enum {
	HOLOS_PROJECT_STATE_UNDEFINED = 0,
	HOLOS_PROJECT_STATE_INIT,
	HOLOS_PROJECT_STATE_ACTIVE,
	HOLOS_PROJECT_STATE_PAUSED,
	HOLOS_PROJECT_STATE_COMPLETED,
	HOLOS_PROJECT_STATE_ARCHIVED,
	HOLOS_PROJECT_STATE_ERROR
} holos_project_state;

/**
 * struct holos_project - Extensible project metadata and management structure
 *
 * Now includes lifecycle state, risk, sustainability score, digital twin status, and advanced analytics hooks.
 */
/**
 * struct holos_project - Comprehensive, extensible project metadata and management structure
 *
 * Now includes dependency management, collaboration, audit trail, security, and more reserved fields.
 */
struct holos_project {
	char *id;
	char *name;
	char *description;
	char *database_path;
	char *physics_file;
	char *ai_file;
	char *p2p_file;
	char *git_repo_path;
	char *version;
	char *created_at;
	char *updated_at;
	char *owner;
	char *organization;
	char *compliance;
	char *sustainability;
	char *provenance;
	char *analytics_path;
	char *digital_twin_path;
	char *diagnostics;
	char *tags;
	char *provenance_id;           /**< Unique provenance identifier for traceability */
	char *ai_model_path;            /**< Path to associated AI/ML model */
	char *compliance_report_path;   /**< Path to compliance report */
	char *accessibility_notes;      /**< Accessibility and inclusion notes */
	holos_project_state state;      /**< Project lifecycle state */
	double risk_score;              /**< Project risk assessment score (0-1) */
	double sustainability_score;    /**< Sustainability/circularity score (0-1) */
	int digital_twin_status;        /**< Digital twin sync status (0=none, 1=partial, 2=full) */
	char *advanced_analytics_path;  /**< Path to advanced analytics output */
	char *dependencies;            /**< Comma-separated list of project dependencies */
	char *collaborators;           /**< Comma-separated list of collaborators/emails */
	char *audit_trail_path;        /**< Path to audit trail or log file */
	char *security_profile;        /**< Security/compliance profile string */
	char *future_reserved1; /**< Reserved for future use (string) */
	char *future_reserved2; /**< Reserved for future use (string) */
	char *future_reserved3; /**< Reserved for future use (string) */
	char *future_reserved4; /**< Reserved for future use (string) */
	void *reserved;         /**< Reserved for future binary compatibility */
};

/**
 * holos_project_event_type - Project event/callback types
 */
typedef enum {
	HOLOS_EVENT_NONE = 0,
	HOLOS_EVENT_SAVE,
	HOLOS_EVENT_LOAD,
	HOLOS_EVENT_UPDATE,
	HOLOS_EVENT_DELETE,
	HOLOS_EVENT_DIAGNOSTICS,
	HOLOS_EVENT_COMPLIANCE,
	HOLOS_EVENT_ANALYTICS,
	HOLOS_EVENT_CUSTOM
} holos_project_event_type;

/**
 * holos_project_event_callback - Callback signature for project events
 */
typedef int (*holos_project_event_callback)(struct holos_project *project, holos_project_event_type event, void *user_data);
/**
 * Project dependency and collaboration utilities
 */
int holos_project_add_dependency(struct holos_project *project, const char *dependency_id); /**< Add a dependency. */
int holos_project_remove_dependency(struct holos_project *project, const char *dependency_id); /**< Remove a dependency. */
int holos_project_list_dependencies(const struct holos_project *project, char ***out_list, size_t *out_count); /**< List dependencies. */
int holos_project_add_collaborator(struct holos_project *project, const char *collaborator_email); /**< Add a collaborator. */
int holos_project_remove_collaborator(struct holos_project *project, const char *collaborator_email); /**< Remove a collaborator. */
int holos_project_list_collaborators(const struct holos_project *project, char ***out_list, size_t *out_count); /**< List collaborators. */

/**
 * Audit trail and security utilities
 */
int holos_project_append_audit_event(struct holos_project *project, const char *event); /**< Append event to audit trail. */
int holos_project_check_security_profile(const struct holos_project *project); /**< Check security/compliance profile. */

/**
 * Plugin/extension system
 */
int holos_project_register_event_callback(struct holos_project *project, holos_project_event_callback cb, void *user_data); /**< Register event callback. */
int holos_project_unregister_event_callback(struct holos_project *project, holos_project_event_callback cb); /**< Unregister event callback. */
int holos_project_load_plugin(struct holos_project *project, const char *plugin_path); /**< Load a project plugin/extension. */
};
/**
 * Advanced analytics and sustainability utilities
 */
int holos_project_calculate_risk(struct holos_project *project); /**< Calculate project risk score. */
int holos_project_calculate_sustainability(struct holos_project *project); /**< Calculate sustainability score. */
int holos_project_update_digital_twin_status(struct holos_project *project); /**< Update digital twin status. */
int holos_project_generate_advanced_analytics(struct holos_project *project, const char *output_path); /**< Generate advanced analytics. */
/**
 * Advanced diagnostics and provenance utilities
 */
int holos_project_diagnostics_summary(const struct holos_project *project, char *out_buf, size_t buf_size); /**< Get diagnostics summary. */
int holos_project_provenance_report(const struct holos_project *project, const char *report_path); /**< Generate provenance report. */

/**
 * Batch compliance and analytics utilities
 */
int holos_project_batch_check_compliance(struct holos_project **projects, size_t count); /**< Batch compliance check. */
int holos_project_batch_generate_reports(struct holos_project **projects, size_t count, const char *dir_path); /**< Batch report generation. */

/**
 * Hooks for AI/ML and digital twin integration (future-proof)
 */
int holos_project_link_ai_model(struct holos_project *project, const char *ai_model_path); /**< Link AI/ML model. */
int holos_project_link_digital_twin_model(struct holos_project *project, const char *digital_twin_path); /**< Link digital twin model. */

/**
 * Project management and I/O utilities
 *
 * All functions return 0 on success, negative on error unless otherwise specified.
 * Memory for returned structures must be freed with holos_project_free().
 */
struct holos_project *holos_project_create(const char *name, const char *description); /**< Create a new project. */
struct holos_project *holos_project_load(const char *id); /**< Load a project from disk by ID. */
int holos_project_save(const struct holos_project *project); /**< Save project metadata to disk. */
int holos_project_delete(const char *id); /**< Delete a project by ID. */
int holos_project_clone(const struct holos_project *src, const char *new_id); /**< Clone a project. */

/**
 * Git and version control utilities
 */
int holos_project_init_git(struct holos_project *project); /**< Initialize git repository for project. */
int holos_project_git_add_file(struct holos_project *project, const char *filepath); /**< Add file to git repo. */
int holos_project_git_commit(struct holos_project *project, const char *message); /**< Commit changes to git repo. */
int holos_project_git_status(const struct holos_project *project); /**< Show git status. */
int holos_project_git_log(const struct holos_project *project); /**< Show git log. */

/**
 * File and resource linking
 */
int holos_project_link_physics(struct holos_project *project, const char *physics_file); /**< Link physics file. */
int holos_project_link_ai(struct holos_project *project, const char *ai_file); /**< Link AI file. */
int holos_project_link_p2p(struct holos_project *project, const char *p2p_file); /**< Link P2P file. */
int holos_project_link_analytics(struct holos_project *project, const char *analytics_path); /**< Link analytics file. */
int holos_project_link_digital_twin(struct holos_project *project, const char *digital_twin_path); /**< Link digital twin. */

/**
 * Diagnostics, compliance, and analytics
 */
int holos_project_run_diagnostics(struct holos_project *project); /**< Run diagnostics on project. */
int holos_project_check_compliance(const struct holos_project *project); /**< Check compliance. */
int holos_project_generate_report(const struct holos_project *project, const char *report_path); /**< Generate project report. */
int holos_project_list(const char *filter, struct holos_project ***out_projects, size_t *out_count); /**< List projects by filter. */

/**
 * Batch operations
 */
int holos_project_batch_save(struct holos_project **projects, size_t count); /**< Batch save projects. */
int holos_project_batch_run_diagnostics(struct holos_project **projects, size_t count); /**< Batch diagnostics. */

/**
 * Free project structure
 */
void holos_project_free(struct holos_project *project); /**< Free project structure. */

/**
 * Utilities for extensibility and future-proofing
 */
void holos_project_set_metadata(struct holos_project *project, const char *key, const char *value); /**< Set custom metadata. */
const char *holos_project_get_metadata(const struct holos_project *project, const char *key); /**< Get custom metadata. */


/**
 * GNU-style: All functions return 0 on success, negative on error unless otherwise specified.
 * All structures and functions are designed for extensibility, maintainability, and compliance with modern research workflows.
 *
 * This header is written and documented according to the GNU Coding Standards.
 *
 * - All exported symbols are prefixed with holos_ to avoid namespace pollution.
 * - All public structures and functions are documented for clarity and maintainability.
 * - All memory allocations and deallocations are clearly specified.
 * - All code is written for maximum portability and extensibility.
 */

#ifdef __cplusplus
}
#endif

#endif /* HOLOS_FORGE_H */