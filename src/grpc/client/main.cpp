//
//  main.cpp
//  agent-jv-client
//
//  Created by NITIN KUMAR on 12/29/15.
//  Copyright © 2015 Juniper Networks. All rights reserved.
//

#include <iostream>
#include "AgentClient.hpp"
#include "AgentClientParser.hpp"

// :-/ globals
extern uint32_t global_id;
AgentParser *parser;

int main(int argc, const char * argv[])
{
    std::string logfile_dir;
    
    // Get the Looging dir
    if (argc > 1) {
        logfile_dir = argv[1];
    } else {
        // TODO ABBAS FIXME
        logfile_dir = std::string("/tmp/");
    }

    // A well known Management Client
    std::string mgmt_client_name(AGENTCLIENT_MGMT);
    AgentClient::create(grpc::CreateChannel("localhost:50051", grpc::InsecureCredentials()),
                        mgmt_client_name,
                        global_id++,
                        logfile_dir);

    // Run a simple command line loop to test all
    parser = new AgentParser(logfile_dir);
    parser->parseLoop();

    return 0;
}
