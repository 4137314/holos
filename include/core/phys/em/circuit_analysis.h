/* circuit_analysis.h - Circuit analysis and simulation API for HOLOS
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

#ifndef HOLOS_CIRCUIT_ANALYSIS_H
#define HOLOS_CIRCUIT_ANALYSIS_H

#include <stddef.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/* Supported circuit element types */
typedef enum {
    CIRCUIT_RESISTOR,
    CIRCUIT_CAPACITOR,
    CIRCUIT_INDUCTOR,
    CIRCUIT_VOLTAGE_SOURCE,
    CIRCUIT_CURRENT_SOURCE,
    CIRCUIT_DIODE,
    CIRCUIT_BJT,
    CIRCUIT_MOSFET,
    CIRCUIT_OPAMP,
    CIRCUIT_SWITCH,
    CIRCUIT_PROBE,
    CIRCUIT_SUBCIRCUIT,
    CIRCUIT_VCVS,   /* Voltage-controlled voltage source */
    CIRCUIT_VCCS,   /* Voltage-controlled current source */
    CIRCUIT_CCCS,   /* Current-controlled current source */
    CIRCUIT_CCVS    /* Current-controlled voltage source */
} circuit_element_type;

/* Structure for a single circuit element */
typedef struct {
    circuit_element_type type;
    int node1;      /* First node index */
    int node2;      /* Second node index */
    int ctrl1;      /* Control node 1 (for controlled sources) */
    int ctrl2;      /* Control node 2 (for controlled sources) */
    double value;   /* R [Ohm], C [F], L [H], V [V], I [A], etc. */
    double value2;  /* Secondary value (e.g. beta for BJT, gain for opamp) */
    char *model;    /* Model name for semiconductors/opamp */
    char *label;    /* Optional label */
    void *userdata; /* User-defined pointer */
    void *params;   /* Pointer to parameter set (for nonlinear models) */
} circuit_element;

/* Structure for a netlist (circuit description) */
typedef struct circuit_netlist_s circuit_netlist;

typedef struct circuit_subcircuit_s {
    char *name;
    circuit_netlist *netlist;
    int *external_nodes; /* Mapping to parent netlist nodes */
    size_t n_external_nodes;
    void *userdata;
} circuit_subcircuit;

struct circuit_netlist_s {
    size_t n_elements;
    circuit_element *elements;
    size_t n_nodes;
    char *title;
    void *userdata;
    int has_subcircuits;
    circuit_subcircuit *subcircuits;
    size_t n_subcircuits;
    int has_probes;
    circuit_probe *probes;
    size_t n_probes;
    int has_parameters;
    void *parameters; /* Pointer to global parameter set */
    int error_code;   /* Last error code */
    char *error_msg;  /* Last error message */
};
/* Error codes for circuit analysis */
typedef enum {
    CIRCUIT_OK = 0,
    CIRCUIT_ERR_ALLOC = -1,
    CIRCUIT_ERR_INVALID = -2,
    CIRCUIT_ERR_SINGULAR = -3,
    CIRCUIT_ERR_NONLINEAR = -4,
    CIRCUIT_ERR_CONVERGENCE = -5
} circuit_error_code;
/* Set a parameter in the netlist (returns 0 on success) */
int circuit_netlist_set_parameter(circuit_netlist *net, const char *name, double value);

/* Get a parameter from the netlist (returns value, sets *found=1 if found) */
double circuit_netlist_get_parameter(const circuit_netlist *net, const char *name, int *found);

/* Set error code and message in netlist */
void circuit_netlist_set_error(circuit_netlist *net, int code, const char *msg);

/* Get last error code from netlist */
int circuit_netlist_get_error(const circuit_netlist *net);

/* Get last error message from netlist */
const char *circuit_netlist_get_error_msg(const circuit_netlist *net);
/* Nonlinear DC analysis (Newton-Raphson, returns 0 on success) */
int circuit_nonlinear_dc_analysis(const circuit_netlist *net, gsl_vector *node_voltages);

/* Sensitivity analysis: compute dV/d(param) for each node (returns 0 on success) */
int circuit_sensitivity_analysis(const circuit_netlist *net, const char *param, gsl_vector *sensitivities);

/* Small-signal analysis (returns 0 on success) */
int circuit_small_signal_analysis(const circuit_netlist *net, double omega, gsl_vector *node_voltages);

/* Monte Carlo analysis (returns 0 on success) */
int circuit_monte_carlo_analysis(const circuit_netlist *net, size_t n_runs, gsl_matrix *results);
/* Structure for a probe (voltage, current, power, etc.) */
typedef struct {
    char *type;        /* "voltage", "current", "power", etc. */
    int node1;
    int node2;
    char *label;
    void *userdata;
} circuit_probe;

/* Structure for a voltage/current source with time-dependent function */
typedef double (*circuit_source_func)(double t, void *userdata);

typedef struct {
    int node1;
    int node2;
    circuit_source_func func;
    void *userdata;
    char *label;
} circuit_source;
/* Clone a netlist (deep copy) */
circuit_netlist *circuit_netlist_clone(const circuit_netlist *src);

/* Add a probe to the netlist (returns new index or -1 on error) */
int circuit_netlist_add_probe(circuit_netlist *net, const circuit_probe *probe);

/* Find a probe by label (returns pointer or NULL) */
circuit_probe *circuit_netlist_find_probe(circuit_netlist *net, const char *label);

/* Print a probe to file or stdout */
void circuit_probe_print(const circuit_probe *probe, FILE *f);
/* Print analysis results (voltages, currents, etc.) */
void circuit_print_results(const gsl_vector *node_voltages, FILE *f);

/* Compute power dissipated in each element (returns 0 on success) */
int circuit_compute_power(const circuit_netlist *net, const gsl_vector *node_voltages, gsl_vector *element_powers);

/* Compute current through each element (returns 0 on success) */
int circuit_compute_currents(const circuit_netlist *net, const gsl_vector *node_voltages, gsl_vector *element_currents);

/* Utility: get node index by label (returns index or -1) */
int circuit_netlist_find_node(const circuit_netlist *net, const char *label);

/* Allocate a netlist with n elements and n nodes */
circuit_netlist *circuit_netlist_alloc(size_t n_elements, size_t n_nodes);

/* Free a netlist */
void circuit_netlist_free(circuit_netlist *net);

/* Add an element to the netlist (returns new index or -1 on error) */
int circuit_netlist_add_element(circuit_netlist *net, const circuit_element *elem);

/* Remove an element by index (returns 0 on success) */
int circuit_netlist_remove_element(circuit_netlist *net, size_t idx);

/* Find an element by label (returns pointer or NULL) */
circuit_element *circuit_netlist_find_element(circuit_netlist *net, const char *label);

/* Print a netlist to file or stdout */
void circuit_netlist_print(const circuit_netlist *net, FILE *f);

/* DC analysis: solve for node voltages (returns 0 on success) */
int circuit_dc_analysis(const circuit_netlist *net, gsl_vector *node_voltages);

/* AC analysis: solve for node voltages at frequency omega (returns 0 on success) */
int circuit_ac_analysis(const circuit_netlist *net, double omega, gsl_vector *node_voltages);

/* Transient analysis: simulate time evolution (returns 0 on success) */
int circuit_transient_analysis(const circuit_netlist *net, double t0, double t1, double dt, gsl_matrix *node_voltages);

#endif /* HOLOS_CIRCUIT_ANALYSIS_H */
