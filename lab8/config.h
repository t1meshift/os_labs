#pragma once

/**
 * Server port. Also used in client to specify connection port.
 */
#define PORT 8841

/**
 * Maximum number of connections pending.
 */
#define PENDING_CONNS_MAX 32

/**
 * Timeout value in seconds.
 */
#define TIMEOUT_SECS 120

/**
 * Buffer size for client request.
 */
#define BUF_SIZE 4096