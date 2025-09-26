/* tech.h - Technology data structures and functions for HOLOS
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

#ifndef HOLOS_TECH_H
#define HOLOS_TECH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Technology metadata structure, supporting cybersecurity, ethics, circularity, localization, and accessibility. */
struct holos_technology {
	char *id;                                /* Unique technology identifier */
	char *name;                              /* Technology name */
	char *description;                       /* Description of the technology */
	char *category;                          /* Technology category (energy, bio, AI, etc.) */
	char *physics_model;                     /* Reference to physics model used */
	char *creator;                           /* Creator or author */
	int trl;                                 /* Technology Readiness Level (1-9) */
	char **patents;                          /* Array of patent numbers/IDs */
	int n_patents;
	double efficiency;                       /* Calculated efficiency or performance */
	double cost;                             /* Estimated cost */
	double impact;                           /* Environmental or scientific impact */
	double sustainability;                   /* Sustainability index (0-1) */
	char *digital_twin;                      /* Digital twin reference or URI */
	char **tags;                             /* Array of tags/keywords */
	int n_tags;
	char *lifecycle_stage;                   /* Lifecycle stage (R&D, pilot, deployment, EOL, etc.) */
	char **regulatory_certifications;        /* Array of regulatory/certification IDs */
	int n_certifications;
	char **supply_chain_sources;             /* Array of supply chain source names/IDs */
	int n_supply_sources;
	double risk_score;                       /* Risk assessment score (0-1) */
	char *digital_certification;             /* Digital certification reference or URI */
	char *ai_explainability;                 /* AI/ML explainability/interpretability notes */
	double ai_bias_score;                    /* AI/ML bias/fairness score */
	double carbon_footprint;                 /* Environmental carbon footprint (kg CO2e) */
	double water_usage;                      /* Water usage (m^3) */
	char *interoperability;                  /* Interoperability/standards notes */
	char *provenance;                        /* Provenance/traceability record */
	char *version;                           /* Technology version/revision */
	char *cybersecurity;                     /* Cybersecurity/compliance notes */
	double cyber_risk_score;                 /* Cybersecurity risk score (0-1) */
	char *ethics;                            /* Ethics/AI ethics notes */
	double ethics_score;                     /* Ethics/compliance score (0-1) */
	double circularity_index;                /* Circularity/recyclability index (0-1) */
	char *localization;                      /* Localization/region notes */
	char *accessibility;                     /* Accessibility/standards notes */
	double accessibility_score;              /* Accessibility score (0-1) */
	void *extra;                             /* For extensibility */
};
/* Technology diagnostics structure, supporting cyber, ethics, circularity, accessibility, and advanced analytics. */
struct holos_tech_diagnostics {
	double lifecycle_score;                  /* Score for lifecycle maturity */
	double regulatory_score;                 /* Score for regulatory compliance */
	double supply_chain_score;               /* Score for supply chain robustness */
	double risk_score;                       /* Risk assessment score */
	double ai_explainability_score;          /* AI/ML explainability score */
	double ai_bias_score;                    /* AI/ML bias/fairness score */
	double carbon_footprint;                 /* Environmental carbon footprint (kg CO2e) */
	double water_usage;                      /* Water usage (m^3) */
	char *provenance;                        /* Provenance/traceability record */
	char *last_audit_date;                   /* Last audit/check date */
	char *version;                           /* Technology version/revision */
	double cyber_risk_score;                 /* Cybersecurity risk score (0-1) */
	double ethics_score;                     /* Ethics/compliance score (0-1) */
	double circularity_index;                /* Circularity/recyclability index (0-1) */
	double accessibility_score;              /* Accessibility score (0-1) */
	void *extra;
};
/* Compliance, accessibility, and circularity reporting */
void holos_tech_compliance_report(const struct holos_technology *tech);
void holos_tech_accessibility_report(const struct holos_technology *tech);
void holos_tech_circularity_report(const struct holos_technology *tech);
/* Provenance and versioning utilities */
void holos_tech_set_provenance(struct holos_technology *tech, const char *provenance);
void holos_tech_set_version(struct holos_technology *tech, const char *version);
const char *holos_tech_get_provenance(const struct holos_technology *tech);
const char *holos_tech_get_version(const struct holos_technology *tech);
/* Advanced analytics and environmental reporting */
void holos_tech_analytics_report(const struct holos_technology *tech);
void holos_tech_environmental_report(const struct holos_technology *tech);

/* Create a new technology (with extensible fields) */
struct holos_technology *holos_tech_create(const char *name, const char *description, const char *category, const char *physics_model, const char *creator, const char *lifecycle_stage);
/* Attach diagnostics to a technology */
void holos_tech_attach_diagnostics(struct holos_technology *tech, struct holos_tech_diagnostics *diag);
/* Advanced search and reporting utilities */
struct holos_technology *holos_tech_find_by_lifecycle(struct holos_technology **techs, int n, const char *lifecycle_stage);
struct holos_technology *holos_tech_find_by_certification(struct holos_technology **techs, int n, const char *certification);
void holos_tech_report(const struct holos_technology *tech);
void holos_tech_diagnostics_print(const struct holos_tech_diagnostics *diag);

/* Load technology from disk or database */
struct holos_technology *holos_tech_load(const char *id);

/* Save technology to disk or database */
int holos_tech_save(const struct holos_technology *tech);

/* Calculate technology properties using physics model */
int holos_tech_calculate(struct holos_technology *tech);

/* Optimize technology using AI or optimization algorithms */
int holos_tech_optimize(struct holos_technology *tech);

/* Simulate technology in P2P or digital twin environment */
int holos_tech_simulate_p2p(struct holos_technology *tech);

/* Free technology structure */
void holos_tech_free(struct holos_technology *tech);

/* Batch operations and search utilities */
struct holos_technology **holos_tech_batch_create(const char **names, const char **descriptions, const char **categories, const char **physics_models, const char **creators, int n);
void holos_tech_batch_free(struct holos_technology **techs, int n);
struct holos_technology *holos_tech_find_by_tag(struct holos_technology **techs, int n, const char *tag);
struct holos_technology *holos_tech_find_by_category(struct holos_technology **techs, int n, const char *category);

/* Diagnostics and output utilities */
void holos_tech_print(const struct holos_technology *tech);
void holos_tech_print_brief(const struct holos_technology *tech);
void holos_tech_print_all(struct holos_technology **techs, int n);

#ifdef __cplusplus
}
#endif

#endif /* HOLOS_TECH_H */
