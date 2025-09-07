/**
 * Copyright (C) 2023 Carnegie Mellon University
 *
 * This file is part of the Mixnet course project developed for
 * the Computer Networks course (15-441/641) taught at Carnegie
 * Mellon University.
 *
 * No part of the Mixnet project may be copied and/or distributed
 * without the express permission of the 15-441/641 course staff.
 */
#ifndef MIXNET_PACKET_H_
#define MIXNET_PACKET_H_

#include "address.h"

#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Helper macros
#define CHECK_ALIGNMENT_AND_SIZE(typename, size, align)         \
    static_assert(sizeof(typename) == size, "Bad size");        \
    static_assert(alignof(typename) <= align, "Bad alignment")

/**
 * Mixnet packet type.
 */
enum mixnet_packet_type_enum {
         = 0,
    PACKET_TYPE_FLOOD,
    PACKET_TYPE_LSA,
    PACKET_TYPE_DATA,
    PACKET_TYPE_PING,
};
// Shorter type alias for the enum
typedef uint16_t mixnet_packet_type_t;

/**
 * Represents a generic Mixnet packet header.
 */
typedef struct mixnet_packet {
    uint16_t total_size;                // Total packet size (in bytes)
    mixnet_packet_type_t type;          // The type of Mixnet packet
    uint64_t _reserved[1];              // Reserved fields

#ifndef __cplusplus
    char payload[];                     // Variable-size payload
#else
    char *payload() {
        return reinterpret_cast<char*>(this) + sizeof(*this);
    }
    const char *payload() const {
        return reinterpret_cast<const char*>(this) + sizeof(*this);
    }
#endif
}__attribute__((__packed__)) mixnet_packet;
CHECK_ALIGNMENT_AND_SIZE(mixnet_packet, 12, 1);

/**
 * Represents the payload for an STP packet.
 */
typedef struct mixnet_packet_stp {
    mixnet_address root_address;        // Root of the spanning tree
    uint16_t path_length;               // Length of path to the root
    mixnet_address node_address;        // Current node's mixnet address

}__attribute__((packed)) mixnet_packet_stp;
CHECK_ALIGNMENT_AND_SIZE(mixnet_packet_stp, 6, 1);

/**
 * Represents a node's link parameters.
 */
typedef struct mixnet_lsa_link_params {
    mixnet_address neighbor_mixaddr;    // Link partner's mixnet address
    uint16_t cost;                      // Cost of routing on this link

}__attribute__((packed)) mixnet_lsa_link_params;
CHECK_ALIGNMENT_AND_SIZE(mixnet_lsa_link_params, 4, 1);

/**
 * Represents the payload for an LSA packet.
 */
typedef struct mixnet_packet_lsa {
    mixnet_address node_address;        // Advertising node's mixnet address
    uint16_t neighbor_count;            // Advertising node's neighbor count

#ifndef __cplusplus
    mixnet_lsa_link_params links[];     // Variable-size neighbor list
#else
    mixnet_lsa_link_params *links() {
        return reinterpret_cast<mixnet_lsa_link_params*>(
            reinterpret_cast<char*>(this) + sizeof(*this));
    }
#endif
}__attribute__((packed)) mixnet_packet_lsa;
CHECK_ALIGNMENT_AND_SIZE(mixnet_packet_lsa, 4, 1);

/**
 * Represents a Routing Header (RH).
 */
typedef struct mixnet_packet_routing_header {
    mixnet_address src_address;         // Mixnet source address
    mixnet_address dst_address;         // Mixnet destination address
    uint16_t route_length;              // Route length (excluding src, dst)
    uint16_t hop_index;                 // Index of current hop in the route

#ifndef __cplusplus
    mixnet_address route[];             // Var-size route (size is zero if no hops)
#else
    mixnet_address *route() {
        return reinterpret_cast<mixnet_address*>(
            reinterpret_cast<char*>(this) + sizeof(*this));
    }
    const mixnet_address *route() const {
        return reinterpret_cast<const mixnet_address*>(
            reinterpret_cast<const char*>(this) + sizeof(*this));
    }
#endif
}__attribute__((packed)) mixnet_packet_routing_header;
CHECK_ALIGNMENT_AND_SIZE(mixnet_packet_routing_header, 8, 1);

/**
 * Represents the payload for a PING packet (minus RH).
 */
typedef struct mixnet_packet_ping {
    bool is_request;                    // true if request, false otherwise
    uint8_t _pad[1];                    // 1B pad for easier diagramming
    uint64_t send_time;                 // Sender-populated request time

}__attribute__((packed)) mixnet_packet_ping;
CHECK_ALIGNMENT_AND_SIZE(mixnet_packet_ping, 10, 1);

// Constant parameters
static const uint16_t MIN_MIXNET_PACKET_SIZE = sizeof(mixnet_packet);
static const uint16_t MAX_MIXNET_PACKET_SIZE = 4096;
static const uint16_t MAX_MIXNET_ROUTE_LENGTH = 256;

static const uint16_t MAX_MIXNET_DATA_SIZE = (
    MAX_MIXNET_PACKET_SIZE - sizeof(mixnet_packet) -
    (sizeof(mixnet_packet_routing_header) + (sizeof(
        mixnet_address) * MAX_MIXNET_ROUTE_LENGTH)));

// Cleanup
#undef CHECK_ALIGNMENT_AND_SIZE

#ifdef __cplusplus
}
#endif

#endif // MIXNET_PACKET_H_
