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
#include "common/testing.h"

/**
 * This test-case exercises the degenerate single-node topology. The
 * lone node is its own STP root and has no neighbors, so flooding a
 * packet from it should not produce any output. This checks that the
 * node implementation doesn't crash or fabricate packets when the set
 * of neighbors is empty.
 */
class testcase_singleton final : public testcase {
public:
    explicit testcase_singleton() :
        testcase("testcase_singleton") {}

    virtual void pcap(const uint16_t, const mixnet_packet
                                *const packet) override {
        if (packet->type == PACKET_TYPE_FLOOD) {
            pcap_count_++;
        }
    }

    virtual void setup() override {
        init_graph(1);
        // A single node with no links
        graph_->set_mixaddrs({42});
    }

    virtual error_code run(orchestrator& o) override {
        await_convergence(); // Await STP convergence

        // Subscribe to packets from the lone node
        DIE_ON_ERROR(o.pcap_change_subscription(0, true));

        // Flood a few times from the only node; with no neighbors,
        // none of these packets should ever appear on user output.
        for (uint16_t i = 0; i < 3; i++) {
            DIE_ON_ERROR(o.send_packet(0, 0, PACKET_TYPE_FLOOD));
        }
        await_packet_propagation();
        return error_code::NONE;
    }

    virtual void teardown() override {
        pass_teardown_ = (pcap_count_ == 0);
    }
};

int main(int argc, char **argv) {
    testcase_singleton tc; // Run testcase
    return testcase::run_testcase(tc, argc, argv);
}
