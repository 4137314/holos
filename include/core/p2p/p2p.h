/* p2p.h - Peer-to-peer scientific collaboration for HOLOS
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

#ifndef HOLOS_P2P_H
#define HOLOS_P2P_H

#ifdef __cplusplus
extern "C" {
#endif



#include <stdint.h>
#include <stddef.h>
#include <time.h>

/* Error codes for P2P operations */
enum holos_p2p_error {
	HOLOS_P2P_OK = 0,
	HOLOS_P2P_ERR_GENERIC = -1,
	HOLOS_P2P_ERR_NETWORK = -2,
	HOLOS_P2P_ERR_PEER_NOT_FOUND = -3,
	HOLOS_P2P_ERR_PROJECT_NOT_FOUND = -4,
	HOLOS_P2P_ERR_AUTH = -5,
	HOLOS_P2P_ERR_TIMEOUT = -6
};

/* Supported message types */
enum holos_p2p_message_type {
	HOLOS_P2P_MSG_DATA,
	HOLOS_P2P_MSG_REQUEST,
	HOLOS_P2P_MSG_STATUS,
	HOLOS_P2P_MSG_AUTH,
	HOLOS_P2P_MSG_EVENT
};

/* Structure for P2P peer metadata */
struct holos_p2p_peer {
	char *id;              /* Unique peer identifier */
	char *address;         /* Network address */
	char *name;            /* Peer name */
	int online;            /* Online status */
	time_t last_seen;      /* Last seen timestamp */
	char *public_key;      /* Public key for authentication */
};

/* Structure for P2P project metadata */
struct holos_p2p_project {
	char *id;                /* Unique project identifier */
	char *name;              /* Project name */
	char *description;       /* Project description */
	char *owner;             /* Owner or creator */
	char *data_path;         /* Path to shared data */
	char *access_policy;     /* Access policy (public/private/restricted) */
	time_t created_at;       /* Creation timestamp */
};

/* Structure for P2P message */
struct holos_p2p_message {
	enum holos_p2p_message_type type; /* Message type */
	void *payload;           /* Message payload */
	size_t size;             /* Payload size */
	char *sender_id;         /* Sender peer ID */
	char *receiver_id;       /* Receiver peer ID */
	time_t timestamp;        /* Message timestamp */
};

/* Structure for P2P event */
struct holos_p2p_event {
	char *event_type;        /* Event type (e.g., "connect", "disconnect", "data_received") */
	void *event_data;        /* Event-specific data */
	time_t timestamp;        /* Event timestamp */
};

/* Callback type for event hooks */
typedef void (*holos_p2p_event_callback)(const struct holos_p2p_event *event);

/* Register an event callback */
int holos_p2p_register_event_callback(holos_p2p_event_callback cb);

/* Discover peers on the network */
int holos_p2p_discover_peers(struct holos_p2p_peer **peers, size_t *count);

/* Get peer info by ID */
int holos_p2p_get_peer(const char *peer_id, struct holos_p2p_peer *peer);

/* Authenticate with a peer */
int holos_p2p_authenticate_peer(const char *peer_id, const char *public_key);

/* Error handling */
void holos_p2p_error(const char *msg, enum holos_p2p_error code);

/* Initialize P2P subsystem (GNUnet) */
int holos_p2p_init(void);

/* Connect to P2P network or peer */
int holos_p2p_connect(const char *peer_address);

/* Disconnect from P2P network */
int holos_p2p_disconnect(const char *peer_address);

/* Share a project over P2P */
int holos_p2p_share_project(const struct holos_p2p_project *project);

/* Update project metadata on the network */
int holos_p2p_update_project(const struct holos_p2p_project *project);

/* Request a project from P2P network */
int holos_p2p_request_project(const char *project_id);

/* Send a message to a peer */
int holos_p2p_send_message(const char *peer_address, const struct holos_p2p_message *msg);

/* Receive a message from a peer */
int holos_p2p_receive_message(char *peer_address, struct holos_p2p_message *msg);

/* Broadcast a message to all peers */
int holos_p2p_broadcast_message(const struct holos_p2p_message *msg);

/* Send raw data to a peer (legacy) */
int holos_p2p_send_data(const char *peer_address, const void *data, uint64_t size);

/* Receive raw data from a peer (legacy) */
int holos_p2p_receive_data(char *peer_address, void *buffer, uint64_t size);

/* Finalize and clean up P2P subsystem */
void holos_p2p_finalize(void);

/* Utility: Free peer structure */
void holos_p2p_free_peer(struct holos_p2p_peer *peer);

/* Utility: Free project structure */
void holos_p2p_free_project(struct holos_p2p_project *project);

/* Utility: Free message structure */
void holos_p2p_free_message(struct holos_p2p_message *msg);

/* Utility: Free event structure */
void holos_p2p_free_event(struct holos_p2p_event *event);

/* Structure for P2P peer metadata */
struct holos_p2p_peer {
	char *id;              /* Unique peer identifier */
	char *address;         /* Network address */
	char *name;            /* Peer name */
	int online;            /* Online status */
};

/* Structure for P2P message */
struct holos_p2p_message {
	char *type;            /* Message type (e.g., "data", "request", "status") */
	void *payload;         /* Message payload */
	size_t size;           /* Payload size */
};

/* Structure for P2P event */
struct holos_p2p_event {
	char *event_type;      /* Event type (e.g., "connect", "disconnect", "data_received") */
	void *event_data;      /* Event-specific data */
};

/* Callback type for event hooks */
typedef void (*holos_p2p_event_callback)(const struct holos_p2p_event *event);

/* Register an event callback */
int holos_p2p_register_event_callback(holos_p2p_event_callback cb);

/* Discover peers on the network */
int holos_p2p_discover_peers(struct holos_p2p_peer **peers, size_t *count);

/* Get peer info by ID */
int holos_p2p_get_peer(const char *peer_id, struct holos_p2p_peer *peer);

/* Error handling */
void holos_p2p_error(const char *msg);

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

/* Connect to P2P network or peer */
int holos_p2p_connect(const char *peer_address);

/* Disconnect from P2P network */
int holos_p2p_disconnect(const char *peer_address);

/* Share a project over P2P */
int holos_p2p_share_project(const struct holos_p2p_project *project);

/* Request a project from P2P network */
int holos_p2p_request_project(const char *project_id);

/* Send a message to a peer */
int holos_p2p_send_message(const char *peer_address, const struct holos_p2p_message *msg);

/* Receive a message from a peer */
int holos_p2p_receive_message(char *peer_address, struct holos_p2p_message *msg);

/* Send raw data to a peer (legacy) */
int holos_p2p_send_data(const char *peer_address, const void *data, uint64_t size);

/* Receive raw data from a peer (legacy) */
int holos_p2p_receive_data(char *peer_address, void *buffer, uint64_t size);

/* Finalize and clean up P2P subsystem */
void holos_p2p_finalize(void);

#ifdef __cplusplus
}
#endif

#endif /* HOLOS_P2P_H */
