/* src/p2p/p2p.c
 *
 * Minimal GNUnet-aware P2P helper for Holos
 *
 * Builds and runs even when GNUnet is not present (graceful fallback).
 *
 * To compile with GNUnet (example):
 *   gcc -DHAVE_GNUNET `pkg-config --cflags libgnunetutil` -o p2p p2p.c `pkg-config --libs libgnunetutil`
 *
 * If you use Autotools, add PKG_CHECK_MODULES for gnunet in configure.ac and add -DHAVE_GNUNET.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef HAVE_GNUNET
/* GNUnet headers (the exact header names depend on installed GNUnet version).
 * You may need to install gnunet-dev and adapt these includes.
 */
#include <gnunet/gnunet_util_lib.h>
#include <gnunet/gnunet_peerinfo_service.h>
#include <gnunet/gnunet_scheduler.h>
#endif

static volatile int keep_running = 1;

static void int_handler(int signo) {
    (void)signo;
    keep_running = 0;
}

/* ---------------- Generic fallback implementations ---------------- */

#ifndef HAVE_GNUNET

/* If GNUnet not available, provide no-op implementations so program still builds. */

int p2p_init(void) {
    fprintf(stderr, "[p2p] GNUnet not available: running in offline stub mode.\n");
    return 0;
}

void p2p_shutdown(void) {
    /* nothing */
}

int p2p_list_peers(void) {
    fprintf(stderr, "[p2p] GNUnet not present: peer listing unavailable.\n");
    return -1;
}

int p2p_publish_message(const char *topic, const char *msg) {
    fprintf(stderr, "[p2p] GNUnet not present: publish '%s' suppressed.\n", topic);
    return -1;
}

int p2p_subscribe_topic(const char *topic) {
    fprintf(stderr, "[p2p] GNUnet not present: subscribe '%s' suppressed.\n", topic);
    return -1;
}

#else /* HAVE_GNUNET */

/* ---------------- GNUnet-enabled implementations ----------------
 *
 * These use typical GNUnet util APIs. The exact function names and usage
 * may change by version: adapt if your installed GNUnet exposes different headers.
 */

/* Global GNUnet objects (simple program-level usage) */
static struct GNUNET_CONFIGURATION_Handle *g_cfg = NULL;
static struct GNUNET_SCHEDULER_Context *g_sched = NULL;

/* Helper: simple logging init */
static void p2p_setup_logging(void) {
    /* GNUNET_log_setup(name, level, options) typical pattern; adapt to your GNUnet version */
    GNUNET_log_setup("holos-p2p", "INFO", NULL);
}

/* Initialize GNUnet configuration and scheduler */
int p2p_init(void) {
    p2p_setup_logging();

    /* Create and load configuration (uses default configuration directory) */
    g_cfg = GNUNET_CONFIGURATION_create();
    if (g_cfg == NULL) {
        fprintf(stderr, "[p2p] Failed to create GNUnet configuration handle.\n");
        return -1;
    }

    if (GNUNET_CONFIGURATION_load(g_cfg, NULL) != GNUNET_OK) {
        fprintf(stderr, "[p2p] Failed to load GNUnet configuration. Is GNUnet installed and configured?\n");
        GNUNET_CONFIGURATION_destroy(g_cfg);
        g_cfg = NULL;
        return -1;
    }

    /* Create a scheduler context for asynchronous callbacks (many GNUnet APIs require this) */
    g_sched = GNUNET_SCHEDULER_create();
    if (g_sched == NULL) {
        fprintf(stderr, "[p2p] Failed to create GNUnet scheduler.\n");
        GNUNET_CONFIGURATION_destroy(g_cfg);
        g_cfg = NULL;
        return -1;
    }

    /* Optional: start GNUnet core or ensure the GNUnet daemon is running.
     * Usually GNUnet runs as system service (gnunet-arm). This code assumes the GNUnet
     * infrastructure/daemons are already running and accessible via IPC.
     */

    fprintf(stderr, "[p2p] GNUnet initialized successfully.\n");
    return 0;
}

/* Shutdown and cleanup */
void p2p_shutdown(void) {
    if (g_sched) {
        GNUNET_SCHEDULER_destroy(g_sched);
        g_sched = NULL;
    }
    if (g_cfg) {
        GNUNET_CONFIGURATION_destroy(g_cfg);
        g_cfg = NULL;
    }
    GNUNET_log_shutdown();
}

/* Example: simple peerinfo callback. This function signature is a sketch: adapt
 * the types to the exact GNUNET_PEERINFO_iterator callback your version expects.
 */
static void peerinfo_callback(void *cls,
        const struct GNUNET_PeerIdentity *peer,
        const struct GNUNET_HELLO_Address *address,
        struct GNUNET_TIME_Absolute expiration,
        const char *err_msg)
{
    (void)cls;
    (void)expiration;
    (void)err_msg;

    if (peer != NULL) {
        char *s = GNUNET_i2s(peer);
        fprintf(stderr, "[p2p] Peer: %s\n", s);
        GNUNET_free(s);
    } else {
        fprintf(stderr, "[p2p] peerinfo: end or null\n");
    }

    /* Optionally print addresses if present */
    if (address) {
        /* The structure and fields depend on GNUnet version. Inspect header for details. */
        fprintf(stderr, "[p2p] address (peer): <address info available>\n");
    }
}

/* List peers (example using peerinfo service) */
int p2p_list_peers(void) {
    if (!g_cfg) {
        fprintf(stderr, "[p2p] not initialized\n");
        return -1;
    }

    /* Example: iterate peers. The exact GNUnet API names and params might differ:
     *
     * GNUNET_PEERINFO_iterate(g_cfg, ... , peerinfo_callback, NULL);
     *
     * Some versions return a context/iterator handle; others use synchronous api.
     *
     * Here we attempt a call and print a note if the exact symbol is unavailable.
     */
#if defined(GNUNET_PEERINFO_ITERATE_AVAILABLE)
    GNUNET_PEERINFO_iterate(g_cfg, NULL, GNUNET_YES, &peerinfo_callback, NULL);
#else
    /* Fallback: try to call the peerinfo service via simple helper (if available) */
    fprintf(stderr, "[p2p] peer iteration API symbol not detected at compile-time; implement using your GNUnet release API.\n");
#endif

    return 0;
}

/* Simplified publish/subscribe placeholders:
 * A full implementation would use GNUNET core messaging services (e.g. GNUNET_MESSAGE_* or a service plugin).
 * Here we show a stub that demonstrates intent and where to call GNUnet APIs.
 */

int p2p_publish_message(const char *topic, const char *msg) {
    if (!g_cfg) return -1;

    fprintf(stderr, "[p2p] publish requested: topic='%s' msg='%s'\n", topic, msg);

    /* TODO: implement using GNUNET core API for messaging or a custom GNUnet service.
     * e.g. GNUNET_MESSAGE_publish(...), or interact with a specific GNUNET plugin.
     */

    return 0;
}

int p2p_subscribe_topic(const char *topic) {
    if (!g_cfg) return -1;

    fprintf(stderr, "[p2p] subscribe requested: topic='%s'\n", topic);

    /* TODO: create subscription using GNUNET APIs, register callbacks to receive messages */

    return 0;
}

#endif /* HAVE_GNUNET */

/* ---------------- small CLI for testing ---------------- */

static void usage(const char *prog) {
    fprintf(stderr,
            "Usage: %s [command]\n"
            "Commands:\n"
            "  init          Initialize P2P subsystem\n"
            "  peers         List peers (best-effort)\n"
            "  publish TOPIC MSG   Publish a message (placeholder)\n"
            "  subscribe TOPIC     Subscribe to a topic (placeholder)\n"
            "  quit\n", prog);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    signal(SIGINT, int_handler);
    signal(SIGTERM, int_handler);

    printf("Holos P2P helper (GNUnet-aware)\n");

    if (p2p_init() != 0) {
        fprintf(stderr, "[p2p] initialization failed (running in degraded mode)\n");
    }

    /* Very small REPL */
    char line[1024];
    while (keep_running) {
        printf("p2p> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        /* strip newline */
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;

        char *cmd = strtok(line, " ");
        if (!cmd) continue;

        if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
            break;
        } else if (strcmp(cmd, "peers") == 0) {
            p2p_list_peers();
        } else if (strcmp(cmd, "publish") == 0) {
            char *topic = strtok(NULL, " ");
            char *msg = strtok(NULL, "");
            if (!topic || !msg) {
                fprintf(stderr, "Usage: publish TOPIC MSG\n");
            } else {
                p2p_publish_message(topic, msg);
            }
        } else if (strcmp(cmd, "subscribe") == 0) {
            char *topic = strtok(NULL, " ");
            if (!topic) {
                fprintf(stderr, "Usage: subscribe TOPIC\n");
            } else {
                p2p_subscribe_topic(topic);
            }
        } else {
            fprintf(stderr, "Unknown command: %s\n", cmd);
            usage(argv[0]);
        }
    }

    p2p_shutdown();
    printf("p2p: exiting\n");
    return 0;
}