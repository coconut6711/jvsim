//
//  agent_test_consolidator.cpp
//  agent-jv
//
//  Created by NITIN KUMAR on 2/18/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//


#include <iostream>
#include <functional>
#include <string>
#include "gtest/gtest.h"
#include "agent_test_consolidator.hpp"
#include "AgentServerLog.hpp"

TEST_F(AgentConsolidatorTest, add) {
    AgentConsolidatorHandle *handle;
    SubscriptionRequest request;
    Telemetry::Path *path;

    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");
    path = request.add_path_list();
    path->set_path("port");

    // Add it to the consolidator
    handle = cons->addRequest(61, &request);
    EXPECT_TRUE(handle != NULL);
    EXPECT_EQ(2, cons->getSystemRequestCount());
    EXPECT_EQ(1, cons->getAddCount());
    EXPECT_EQ(0, cons->getErrors());

    // Remove it now
    cons->removeRequest(handle);
    EXPECT_EQ(0, cons->getSystemRequestCount());
    EXPECT_EQ(1, cons->getRemCount());
    EXPECT_EQ(0, cons->getErrors());
}

TEST_F(AgentConsolidatorTest, add_multiple) {
    AgentConsolidatorHandle *handle1, *handle2;
    SubscriptionRequest request;
    Telemetry::Path *path;

    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");
    path = request.add_path_list();
    path->set_path("port");

    // Add it to the consolidator
    handle1 = cons->addRequest(61, &request);
    EXPECT_TRUE(handle1 != NULL);

    // Add it to the consolidator
    handle2 = cons->addRequest(62, &request);
    EXPECT_TRUE(handle2 != NULL);

    // Only two system requests should have been created
    EXPECT_EQ(2, cons->getSystemRequestCount());
    EXPECT_EQ(2, cons->getAddCount());
    EXPECT_EQ(0, cons->getErrors());

    // Remove it now
    cons->removeRequest(handle1);
    EXPECT_EQ(2, cons->getSystemRequestCount());
    EXPECT_EQ(1, cons->getRemCount());
    EXPECT_EQ(0, cons->getErrors());

    cons->removeRequest(handle2);
    EXPECT_EQ(0, cons->getSystemRequestCount());
    EXPECT_EQ(2, cons->getRemCount());
    EXPECT_EQ(0, cons->getErrors());
}

TEST_F(AgentConsolidatorTest, remove_bogus) {
    cons->removeRequest(NULL);
    EXPECT_EQ(1, cons->getErrors());
}

TEST_F(AgentConsolidatorTest, add_multiple_frequency) {
    AgentConsolidatorHandle *handle1, *handle2;
    SubscriptionRequest request1, request2;
    Telemetry::Path *path;

    // Build a request
    path = request1.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(10);

    path = request1.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(100);

    // Add it to the consolidator
    handle1 = cons->addRequest(61, &request1);
    EXPECT_TRUE(handle1 != NULL);
    EXPECT_EQ(2, cons->getSystemRequestCount());

    // Add another one with the same footprint
    path = request2.add_path_list();
    path->set_path("firewall");
    path->set_sample_frequency(10);
    handle2 = cons->addRequest(62, &request2);
    EXPECT_TRUE(handle2 != NULL);
    EXPECT_EQ(2, cons->getSystemRequestCount());

    // Remove the first request
    cons->removeRequest(handle1);
    EXPECT_EQ(1, cons->getSystemRequestCount());

    // Remove the second request
    cons->removeRequest(handle2);
    EXPECT_EQ(0, cons->getSystemRequestCount());
}

TEST_F(AgentConsolidatorTest, add_concurrent) {
    // Create N concurrent threads to create subscriptions
    int                    n = 30;
    pthread_t              tid[30];
    TestArgs               *targs[30];
    
    // Spawn the N subscribers
    for (int i = 0; i < n; i++) {
        targs[i] = new TestArgs(cons, 20+i);
        pthread_create(&tid[i], NULL,
                       AgentConsolidatorTest::create, (void *)(targs[i]));
    }

    // Wait for all of them to finish
    for (int i = 0; i < n; i++) {
        pthread_join(tid[i], NULL);
    }

    // Make sure there were no errrors
    EXPECT_EQ(n, cons->getAddCount());
    EXPECT_EQ(n, cons->getRemCount());
    EXPECT_EQ(0, cons->getSystemRequestCount());
    EXPECT_EQ(0, cons->getErrors());
}

void *
AgentConsolidatorTest::create (void *args)
{
    TestArgs *test_args = (TestArgs *) args;
    AgentConsolidator *cons = test_args->cons;
    AgentConsolidatorHandle *handle;
    SubscriptionRequest request;
    Telemetry::Path *path;

    // Build a request
    path = request.add_path_list();
    path->set_path("firewall");
    path = request.add_path_list();
    path->set_path("port");

    // Add it to the consolidator
    id_idx_t subscription_id = test_args->id;
    handle = cons->addRequest(subscription_id, &request);

    // Sleep randomly between 0 to 4 seconds
    sleep(rand()%5);

    // Now remove it
    cons->removeRequest(handle);

    return NULL;
}