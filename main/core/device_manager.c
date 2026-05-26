#include <stddef.h>
#include "esp_log.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "device_manager.h"

#if CONFIG_FEATURE_DEVICE_LIGHT
    #define LIGHT_COUNT CONFIG_LIGHT_COUNT
#else
    #define LIGHT_COUNT 0
#endif

#if CONFIG_FEATURE_DEVICE_RELAY
    #define RELAY_COUNT CONFIG_RELAY_COUNT
#else
    #define RELAY_COUNT 0
#endif

#if CONFIG_FEATURE_DEVICE_SENSOR
    #define SENSOR_COUNT CONFIG_SENSOR_COUNT
#else
    #define SENSOR_COUNT 0
#endif

#define DEVICE_COUNT (LIGHT_COUNT + RELAY_COUNT + SENSOR_COUNT)

static const char *TAG = "Core_DeMa";

static device_t devices[DEVICE_COUNT];

void deviceManager_init(void)
{
    for (int i = 0; i < DEVICE_COUNT; i++) {
        devices[i].id = i;
        devices[i].name = NULL;
        devices[i].type = DEVICE_TYPE_UNKNOWN;
        devices[i].gpio_pin = -1;
        devices[i].is_on = false;
        devices[i].brightness = 0;
    }

    ESP_LOGI(TAG, "%d devices created.", DEVICE_COUNT);
}

int device_get_count(void)
{
    return DEVICE_COUNT;
}

device_t *device_get_by_id(int id)
{
    if (id < 0 || id >= DEVICE_COUNT) {
        return NULL;
    }

    return &devices[id];
}

void device_set_on(device_t *device, bool state)
{
    if (device == NULL) {
        return;
    }

    device->is_on = state;
    gpio_set_level(device->gpio_pin, state ? 1 : 0);
}

int device_get_value(device_t *device)
{
    if (device == NULL) return -1;

    return gpio_get_level(device->gpio_pin);
}
