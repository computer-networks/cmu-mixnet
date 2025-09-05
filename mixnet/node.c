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
#include "node.h"

#include "connection.h"
#include "packet.h"

#include <stdio.h>
#include <stdlib.h>

void run_node(void *const handle,
              volatile bool *const keep_running,
              const struct mixnet_node_config c) {
                
    (void) c;
    (void) handle;
    while(*keep_running) {}
    // Checking that pushing and pulling is working correctly for this project.
}
