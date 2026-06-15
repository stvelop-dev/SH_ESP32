#include "sdkconfig.h"
#include "feature_registry.h"
#include "component_config.h"

#if OUTPUT_ANALOG_COUNT > 0
#include "output_analog.h"
#endif

#if OUTPUT_BINARY_COUNT > 0
#include "output_binary.h"
#endif

#if INPUT_ANALOG_COUNT > 0
#include "input_analog.h"
#endif

#if INPUT_BINARY_COUNT > 0
#include "input_binary.h"
#endif

#if CONFIG_FEATURE_INTERFACE_CLI
#include "int_cli.h"
#endif

#if CONFIG_FEATURE_INTERFACE_MQTT
#include "int_mqtt.h"
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

#if OUTPUT_ANALOG_COUNT > 0
    analogOutput_init();
#endif

#if OUTPUT_BINARY_COUNT > 0
    binaryOutput_init();
#endif

#if INPUT_ANALOG_COUNT > 0
    analogInput_init();
#endif

#if INPUT_BINARY_COUNT > 0
    binaryInput_init();
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