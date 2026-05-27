#include "feature_registry.h"
#include "sdkconfig.h"

#if CONFIG_FEATURE_AUTOMATION_NONE
#include "aut_none.h"
#endif

#if CONFIG_FEATURE_AUTOMATION_TIME
#include "aut_time.h"
#endif

#if CONFIG_FEATURE_AUTOMATION_EVENT
#include "aut_event.h"
#endif

#if CONFIG_FEATURE_INTERFACE_CLI
#include "int_cli.h"
#endif

#if CONFIG_FEATURE_INTERFACE_REST
#include "int_rest.h"
#endif

#if CONFIG_FEATURE_INTERFACE_WEBUI
#include "int_webUI.h"
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

#if CONFIG_FEATURE_SECURITY_AUTH
#include "sec_auth.h"
#endif


void featureRegistry_Init(void)
{
#if CONFIG_FEATURE_DEVICE_LIGHT
    lightDevice_init();
#endif

#if CONFIG_FEATURE_DEVICE_RELAY
    relayDevice_init();
#endif

#if CONFIG_FEATURE_DEVICE_SENSOR
    sensorDevice_init();
#endif

#if CONFIG_FEATURE_AUTOMATION_NONE
    automationNone_init();
#endif

#if CONFIG_FEATURE_AUTOMATION_TIME
    automationTimebased_init();
#endif

#if CONFIG_FEATURE_AUTOMATION_EVENT
    automationEventbased_init();
#endif

#if CONFIG_FEATURE_INTERFACE_CLI
    cliInterface_init();
#endif

#if CONFIG_FEATURE_INTERFACE_REST
    restInterface_init();
#endif

#if CONFIG_FEATURE_INTERFACE_WEBUI
    webuiIntferface_init();
#endif

#if CONFIG_FEATURE_SECURITY_NONE
    noneSecurity_init();
#endif

#if CONFIG_FEATURE_SECURITY_AUTH
    authenticationSecurity_init();
#endif
}

void featureRegistry_Process(void)
{
#if CONFIG_FEATURE_AUTOMATION_NONE
    automationNone_process();
#endif

#if CONFIG_FEATURE_AUTOMATION_TIME
    automationTimebased_process();
#endif

#if CONFIG_FEATURE_AUTOMATION_EVENT
    automationEventbased_process();
#endif
}