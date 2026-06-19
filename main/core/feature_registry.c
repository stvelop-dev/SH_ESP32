#include "sdkconfig.h"
#include "feature_registry.h"
#include "component_config.h"

#include "output_analog.h"

#include "output_binary.h"

#include "input_analog.h"

#include "input_binary.h"

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

    analogOutput_init();

    binaryOutput_init();

    analogInput_init();

    binaryInput_init();


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