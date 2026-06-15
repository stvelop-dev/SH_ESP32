set(SRC_FILES
    "app_main.c"
    "core/update_ota.c"
    "core/device_manager.c"
    "core/io_control.c"
    "core/feature_registry.c"

    "features/components/output_analog.c"
    "features/components/output_binary.c"
    "features/components/input_analog.c"
    "features/components/input_binary.c"
)

set(INCLUDE_DIRS
    "include/core"
    "include/services"
    "include/features/interface"
    "include/features/components"
)

set(CERTS 
    "core/certs/ca_cert.pem"
)

if(CONFIG_FEATURE_INTERFACE_CLI)
    list(APPEND SRC_FILES "features/interface/int_cli.c")
endif()

if(CONFIG_FEATURE_INTERFACE_MQTT)
    list(APPEND SRC_FILES "features/interface/int_mqtt.c")
endif()


if(CONFIG_SERVICES_WIFIAP)
    list(APPEND SRC_FILES "services/wifi_ap.c")
endif()

if(CONFIG_SERVICES_WIFICLIENT)
    list(APPEND SRC_FILES "services/wifi_client.c")
endif()

if(CONFIG_SERVICES_SYSTIME)
    list(APPEND SRC_FILES "services/sys_time.c")
endif()

idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    EMBED_TXTFILES ${CERTS}
)
