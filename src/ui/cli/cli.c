#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

/* moduli fisici */
#include "core/phys/nuclear/nuclear_decay.h"
#include "core/phys/nuclear/fission_fusion.h"
#include "core/phys/nuclear/nuclear_forces.h"
#include "core/phys/thermo/entropy.h"
#include "core/phys/thermo/heat_transfer.h"
#include "core/phys/thermo/thermodynamics.h"
#include "core/phys/ss/band_structure.h"
#include "core/phys/ss/crystal_lattice.h"
#include "core/phys/ss/phonons.h"

/* Definizione generica di funzione */
typedef void (*phys_func_t)(void);

/* Struttura di registro */
typedef struct {
    const char *name;
    const char *description;
    phys_func_t func;
} phys_entry_t;

/* Stub di esempio: queste sono implementazioni banali per illustrare */
void run_nuclear_decay(void) { printf("Running nuclear_decay...\n"); }
void run_fission_fusion(void) { printf("Running fission_fusion...\n"); }
void run_nuclear_forces(void) { printf("Running nuclear_forces...\n"); }
void run_entropy(void) { printf("Running entropy...\n"); }
void run_heat_transfer(void) { printf("Running heat_transfer...\n"); }
void run_thermodynamics(void) { printf("Running thermodynamics...\n"); }
void run_band_structure(void) { printf("Running band_structure...\n"); }
void run_crystal_lattice(void) { printf("Running crystal_lattice...\n"); }
void run_phonons(void) { printf("Running phonons...\n"); }

/* Registro delle funzioni */
phys_entry_t registry[] = {
    {"nuclear_decay", "Nuclear decay processes", run_nuclear_decay},
    {"fission_fusion", "Nuclear fission and fusion", run_fission_fusion},
    {"nuclear_forces", "Nuclear forces simulation", run_nuclear_forces},
    {"entropy", "Entropy calculations", run_entropy},
    {"heat_transfer", "Heat transfer simulations", run_heat_transfer},
    {"thermodynamics", "General thermodynamics", run_thermodynamics},
    {"band_structure", "Solid-state band structure", run_band_structure},
    {"crystal_lattice", "Crystal lattice modeling", run_crystal_lattice},
    {"phonons", "Phonon dynamics", run_phonons},
    {NULL, NULL, NULL}
};

/* Trova una funzione nel registro */
phys_func_t find_function(const char *name) {
    for (int i = 0; registry[i].name != NULL; i++) {
        if (strcmp(name, registry[i].name) == 0)
            return registry[i].func;
    }
    return NULL;
}

/* Stampa la lista dei comandi */
void print_help(void) {
    printf("Available commands:\n");
    for (int i = 0; registry[i].name != NULL; i++) {
        printf("  %-15s - %s\n", registry[i].name, registry[i].description);
    }
    printf("  help            - Show this help message\n");
    printf("  quit            - Exit the CLI\n");
}

int main(void) {
    char *input;
    printf("Holos Physics CLI (type 'help' for commands)\n");

    while ((input = readline("holos> ")) != NULL) {
        if (*input) add_history(input);

        if (strcmp(input, "quit") == 0) {
            free(input);
            break;
        } else if (strcmp(input, "help") == 0) {
            print_help();
        } else {
            phys_func_t func = find_function(input);
            if (func) {
                func();
            } else {
                printf("Unknown command: %s\n", input);
            }
        }

        free(input);
    }
    return 0;
}