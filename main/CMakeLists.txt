set(SRC_FILES
    "app_main.c"
    "core/device_manager.c"
    "core/io_control.c"
    "core/feature_registry.c"
)

set(INCLUDE_DIRS
    "include/core"
    "include/services"
    "include/features/interface"
    "include/features/device"
)

if(CONFIG_FEATURE_INTERFACE_MQTT)
    list(APPEND SRC_FILES "features/interface/int_mqtt.c")
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
)