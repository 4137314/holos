/* cli.h - Command-line interface for HOLOS
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

#ifndef HOLOS_CLI_H
#define HOLOS_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>


/* Structure to hold CLI options and state */
struct holos_cli_options {
	int help;
	int version;
	int verbose;
	int interactive;
	char *config_file;
	char *project_file;
	char *subcommand;
	char **args;
	int arg_count;
};

/* Callback type for subcommand handlers */
typedef int (*holos_cli_command_fn)(int argc, char **argv, struct holos_cli_options *opts);

/* Structure for subcommand registration */
struct holos_cli_command {
	const char *name;
	holos_cli_command_fn handler;
	const char *description;
};

/* Register a subcommand handler */
int holos_cli_register_command(const struct holos_cli_command *cmd);

/* Parse command-line arguments using getopt_long */
int holos_parse_options(int argc, char **argv, struct holos_cli_options *opts);

/* Initialize readline for interactive CLI */
void holos_cli_init(void);

/* Run the main CLI loop (interactive or batch) */
void holos_cli_loop(void);

/* Dispatch subcommand */
int holos_cli_dispatch(const char *subcommand, int argc, char **argv, struct holos_cli_options *opts);

/* Print help message */
void holos_cli_print_help(const char *subcommand);

/* Print version information */
void holos_cli_print_version(void);

/* Print error message and exit */
void holos_cli_error(const char *msg);

#ifdef __cplusplus
}
#endif

#endif /* HOLOS_CLI_H */
