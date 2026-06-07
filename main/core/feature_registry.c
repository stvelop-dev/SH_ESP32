#include "feature_registry.h"
#include "sdkconfig.h"

#if CONFIG_FEATURE_INTERFACE_CLI
#include "int_cli.h"
#endif

#if CONFIG_FEATURE_INTERFACE_MQTT
#include "int_mqtt.h"
#endif

#if CONFIG_FEATURE_DEVICE_LIGHT
#include "dev_light.h"
#endif

#if CONFIG_FEATURE_DEVICE_RELAY
#include "dev_relay.h"
#endif

#if CONFIG_FEATURE_DEVICE_SENSOR
#include "dev_sensor.h"
#endif

#if CONFIG_FEATURE_SECURITY_NONE
#include "sec_none.h"
#endif

#if CONFIG_SERVICES_WIFIAP
#include "wifi_ap.h"
#endif

#if CONFIG_SERVICES_WIFICLIENT
#include "wifi_client.h"
#endif

#if CONFIG_SERVICES_SYSTIME
#include "sys_time.h"
#endif


void featureRegistry_Init(void)
{

#if CONFIG_SERVICES_WIFIAP
    wifiAp_init();
#endif

#if CONFIG_SERVICES_WIFICLIENT
    wifiClient_init();
#endif

#if CONFIG_SERVICES_SYSTIME
    systemTime_init();
#endif

#if CONFIG_FEATURE_DEVICE_LIGHT
    lightDevice_init();
#endif

#if CONFIG_FEATURE_DEVICE_RELAY
    relayDevice_init();
#endif

#if CONFIG_FEATURE_DEVICE_SENSOR
    sensorDevice_init();
#endif

#if CONFIG_FEATURE_INTERFACE_CLI
    cliInterface_init();
#endif

#if CONFIG_FEATURE_INTERFACE_MQTT
    mqttInterface_init();
#endif

}

void featureRegistry_Process(void)
{
#if CONFIG_SERVICES_SYSTIME
    systemTime_process();
#endif

}