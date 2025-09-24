/* p2p.h - Peer-to-peer scientific collaboration for HOLOS
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

#ifndef HOLOS_P2P_H
#define HOLOS_P2P_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Structure for P2P project metadata */
struct holos_p2p_project {
	char *id;                /* Unique project identifier */
	char *name;              /* Project name */
	char *description;       /* Project description */
	char *owner;             /* Owner or creator */
	char *data_path;         /* Path to shared data */
};

/* Initialize P2P subsystem (GNUnet) */
int holos_p2p_init(void);

/* Connect to P2P network */
int holos_p2p_connect(const char *peer_address);

/* Share a project over P2P */
int holos_p2p_share_project(const struct holos_p2p_project *project);

/* Request a project from P2P network */
int holos_p2p_request_project(const char *project_id);

/* Send data to a peer */
int holos_p2p_send_data(const char *peer_address, const void *data, uint64_t size);

/* Receive data from a peer */
int holos_p2p_receive_data(char *peer_address, void *buffer, uint64_t size);

/* Finalize and clean up P2P subsystem */
void holos_p2p_finalize(void);

#ifdef __cplusplus
}
#endif

#endif /* HOLOS_P2P_H */
