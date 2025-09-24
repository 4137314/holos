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

/* Structure to hold CLI options */
struct holos_cli_options {
	int help;
	int version;
	char *config_file;
};

/* Parse command-line arguments using getopt_long */
int holos_parse_options(int argc, char **argv, struct holos_cli_options *opts);

/* Initialize readline for interactive CLI */
void holos_cli_init(void);

/* Run the main CLI loop */
void holos_cli_loop(void);

/* Print help message */
void holos_cli_print_help(void);

/* Print version information */
void holos_cli_print_version(void);

#ifdef __cplusplus
}
#endif

#endif /* HOLOS_CLI_H */
