set(SRC_FILES
    "app_main.c"
    "core/device_manager.c"
    "core/io_control.c"
    "core/feature_registry.c"
)

set(INCLUDE_DIRS
    "include/core"
    "include/features/automation"
    "include/features/interface"
    "include/features/device"
    "include/features/security"
)

if(CONFIG_FEATURE_AUTOMATION_NONE)
    list(APPEND SRC_FILES "features/automation/aut_none.c")
endif()

if(CONFIG_FEATURE_AUTOMATION_TIME)
    list(APPEND SRC_FILES "features/automation/aut_time.c")
endif()

if(CONFIG_FEATURE_AUTOMATION_EVENT)
    list(APPEND SRC_FILES "features/automation/aut_event.c")
endif()

if(CONFIG_FEATURE_INTERFACE_CLI)
    list(APPEND SRC_FILES "features/interface/int_cli.c")
endif()

if(CONFIG_FEATURE_INTERFACE_REST)
    list(APPEND SRC_FILES "features/interface/int_rest.c")
endif()

if(CONFIG_FEATURE_INTERFACE_WEBUI)
    list(APPEND SRC_FILES "features/interface/int_webui.c")
endif()

if(CONFIG_FEATURE_DEVICE_LIGHT)
    list(APPEND SRC_FILES "features/device/dev_light.c")
endif()

if(CONFIG_FEATURE_DEVICE_RELAY)
    list(APPEND SRC_FILES "features/device/dev_relay.c")
endif()

if(CONFIG_FEATURE_DEVICE_SENSOR)
    list(APPEND SRC_FILES "features/device/dev_sensor.c")
endif()

if(CONFIG_FEATURE_SECURITY_NONE)
    list(APPEND SRC_FILES "features/security/sec_none.c")
endif()

if(CONFIG_FEATURE_SECURITY_AUTH)
    list(APPEND SRC_FILES "features/security/sec_auth.c")
endif()

idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS ${INCLUDE_DIRS}
)