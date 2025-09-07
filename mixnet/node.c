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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "node.h"
#include "connection.h"
#include "packet.h"
#include "event_timer.h"

// Tracks all of the node's information on STP/Flooding
typedef struct {
    mixnet_address known_root_address;        // Root of the spanning tree
    uint16_t known_path_length;               // Length of path to the root
    mixnet_address known_node_address;        // Current node's mixnet address
    bool is_converged;
    event_timer_t *root_heart_beat_timer;
    event_timer_t *last_STP_update_timer;
    event_timer_t *relection_timer;
    // Need Root Heart Beat Timer, Last Update Time
} STP_data_t;

typedef struct {
    bool isOpen;
    // Need to determine if the value inside the address is valid
    // or not. Will not be valid upon initialization.
    bool is_neighbor_address_valid;
    mixnet_address neighbor_address;
} port_data_t;

void do_STP(void *const handle, STP_data_t *stp_information, const struct mixnet_node_config c) {
    return;
}

void handle_received_packets(void *const handle, STP_data_t *stp_information, const struct mixnet_node_config c) {
    int incoming_port;
    // Will only receive 1 or 0 packets!
    mixnet_packet *incoming_packet = NULL;
    if (mixnet_recv(handle, &incoming_port, &incoming_packet) < 1) {
        // Did not read any packets, no more work to do so just return.
        return;
    }
    switch(incoming_packet->type) {
        case PACKET_TYPE_STP:
            printf("Handling STP packet\n");
            break;
        case PACKET_TYPE_FLOOD:
            printf("Handling FLOOD packet\n");
            break;
        case PACKET_TYPE_LSA:
            printf("Handling LSA packet\n");
            break;
        case PACKET_TYPE_DATA:
            printf("Handling DATA packet\n");
            break;
        case PACKET_TYPE_PING:
            printf("Handling PING packet\n");
            break;
        default:
            printf("Unknown packet type\n");
            break;
    }
    return;
}

void run_node(void *const handle,
              volatile bool *const keep_running,
              const struct mixnet_node_config c) {
                
    // (void) c;
    // (void) handle;
    
    // Initialize the STP information data structure and the data that will
    // be sent out at first.
    STP_data_t *stp_information = malloc(sizeof(STP_data_t));
    stp_information->known_root_address = c.node_addr;
    stp_information->known_path_length = 0;
    stp_information->known_node_address = c.node_addr;
    stp_information->is_converged = false;
    stp_information->root_heart_beat_timer = event_timer_init()
    stp_information->last_STP_update_timer = event_timer_init()
    stp_information->relection_timer = event_timer_init()

    // Initialize the information known about all of the neighbor ports
    // This is an array where every index corresponds to the port at that
    // that specific port. Should be n - 1 total.

    // DOES NOT INCLUDE THE USER PORT.
    port_data_t *ports_data = malloc(sizeof(port_data_t) * c.num_neighbors);
    for (size_t i = 0; i < c.num_neighbors - 1; i++) {
        // Ports should be closed by default for flood packets. Open them up
        // as you discover your parent and potential children in the Spanning Tree.
        ports_data[i]->isOpen = false;
        ports_data[i]->is_neighbor_address_valid = false;
        ports_data[i]->neighbor_address = 0; 
    }

    // Initialization finished, run the node as a loop.

    while(*keep_running) {
        // Handle all of the STP actions this node must perform this cycle.
        // this includes sending STP packets if convergence hasn't happened yet,
        // sending root heart beat messages if this node is the root,
        // relecting if the last root_heart_beat was too long ago, and 
        // declaring convergence if the last STP in general was too long ago.
        do_STP(handle, stp_information, c);
        handle_received_packets(handle, stp_information, c);
    }
}
