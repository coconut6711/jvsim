//
//  GlobalConfig.hpp
//  Telemetry Agent
//
//  Created by ABBAS SAKARWALA on 4/27/16.
//  Copyright © 2016 Juniper Networks. All rights reserved.
//

#ifndef GlobalConfig_hpp
#define GlobalConfig_hpp

#include <string>
#include <ostream>

// GRPC related
#define INI_SECTION_TELEMETRY_GRPC      "telemetry-grpc"
#define GRPC_SERVER_IP                  "0.0.0.0"
#define GRPC_SERVER_PORT                50051
#define GRPC_LOG_FILE_PATH              "/var/log"
#define GRPC_LOG_FILE_NAME              "agent_server.log"

// Junos device related
#define INI_SECTION_JUNOS_DEVICE        "junos-device"
#define DEVICE_MGD_UNIX_SOCKET          "/var/run/japi_mgd"
#define DEVICE_MGD_IP                   "localhost"
#define DEVICE_MGD_PORT                 50050
#define DEVICE_USER_NAME                "regress"
#define DEVICE_PASSWORD                 "MaRtInI"

// MQTT related
#define INI_SECTION_MQTT                "mqtt"
#define MQTT_BROKER_IP                  "127.0.0.1"
#define MQTT_BROKER_PORT                1883

// UDP server related
#define INI_SECTION_UDP                 "udp"
#define UDP_SERVER_MODULE               false
#define UDP_SERVER_IP                   "0.0.0.0"
#define UDP_SERVER_PORT                 10000

// OTHER knobs related
#define INI_SECTION_OTHER_KNOBS         "other-knobs"
#define TOPIC_INTERNAL_SUB_ID           "internal-subscription-id"
#define TOPIC_PATH                      "path"
#define SUBSCRIPTION_TOPIC_NAME         TOPIC_PATH

#define SYSTEM_MODE_NULL                "null"
#define SYSTEM_MODE_FILE                "file"
#define SYSTEM_MODE_PROC                "proc"
#define SYSTEM_FILE_PATH                "/var/log"
#define SYSTEM_FILE_NAME                "sim_system.log"

#define RUNNING_MODE_ON_BOX             "on-box"
#define RUNNING_MODE_OFF_BOX            "off-box"

// Global Config object
class GlobalConfig {
public:
    std::string     grpc_server_ip;
    int             grpc_server_port;
    std::string     grpc_log_file_path;
    std::string     grpc_log_file_name;

    std::string     device_mgd_unix_socket;
    std::string     device_mgd_ip;
    int             device_mgd_port;
    std::string     device_user_name;
    std::string     device_password;

    std::string     mqtt_broker_ip;
    int             mqtt_broker_port;

    bool            udp_server_module;
    std::string     udp_server_ip;
    int             udp_server_port;

    std::string     subscribe_topic_name;
    
    std::string     system_mode;
    std::string     system_file_path;
    std::string     system_file_name;
    
    std::string     running_mode;

    GlobalConfig() {
        grpc_server_ip          = GRPC_SERVER_IP;
        grpc_server_port        = GRPC_SERVER_PORT;
        grpc_log_file_path      = GRPC_LOG_FILE_PATH;
        grpc_log_file_name      = GRPC_LOG_FILE_NAME;

        device_mgd_unix_socket  = DEVICE_MGD_UNIX_SOCKET;
        device_mgd_ip           = DEVICE_MGD_IP;
        device_mgd_port         = DEVICE_MGD_PORT;
        device_user_name        = DEVICE_USER_NAME;
        device_password         = DEVICE_PASSWORD;

        mqtt_broker_ip          = MQTT_BROKER_IP;
        mqtt_broker_port        = MQTT_BROKER_PORT;

        udp_server_module       = UDP_SERVER_MODULE;
        udp_server_ip           = UDP_SERVER_IP;
        udp_server_port         = UDP_SERVER_PORT;

        subscribe_topic_name    = SUBSCRIPTION_TOPIC_NAME;
        system_mode             = SYSTEM_MODE_PROC;
        system_file_path        = SYSTEM_FILE_PATH;
        system_file_name        = SYSTEM_FILE_NAME;

        running_mode            = RUNNING_MODE_OFF_BOX;
    }

    static bool is_valid_subscription_topic_name(std::string sub_topic);
    static bool is_valid_system_mode(std::string system_mode);
    static bool is_valid_running_mode(std::string running_mode);
    friend std::ostream& operator<<(std::ostream& os, GlobalConfig& gc);
    static void parse(std::string filename, GlobalConfig &global_config);
};

// Global configuration object
extern GlobalConfig global_config;

#endif /* GlobalConfig_hpp */