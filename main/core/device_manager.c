#include <stddef.h>
#include "esp_log.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "device_manager.h"

#if CONFIG_FEATURE_DEVICE_LIGHT
    #define LIGHT_COUNT CONFIG_FEATURE_DEVICE_LIGHT_COUNT
#else
    #define LIGHT_COUNT 0
#endif

#if CONFIG_FEATURE_DEVICE_RELAY
    #define RELAY_COUNT CONFIG_FEATURE_DEVICE_RELAY_COUNT
#else
    #define RELAY_COUNT 0
#endif

#if CONFIG_FEATURE_DEVICE_SENSOR
    #define SENSOR_COUNT CONFIG_FEATURE_DEVICE_SENSOR_COUNT
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

int deviceManager_getCount(void)
{
    return DEVICE_COUNT;
}

device_t *deviceManager_getId(int id)
{
    if (id < 0 || id >= DEVICE_COUNT) {
        return NULL;
    }

    return &devices[id];
}

void deviceManager_setState(device_t *device, bool state)
{
    if (device->type == DEVICE_TYPE_LIGHT) {
        if (state) {
            if (device->brightness == 0) {
                deviceManager_setBrightness(device, 255);
            } else {
                deviceManager_setBrightness(device, device->brightness);
            }
        } else {
            deviceManager_setBrightness(device, 0);
        }
        return;
    }

    device->is_on = state;
    gpio_set_level(device->gpio_pin, state ? 1 : 0);
}

int deviceManager_getValue(device_t *device)
{
    return gpio_get_level(device->gpio_pin);
}

void deviceManager_setBrightness(device_t *device, int brightness) 
{
    if (brightness < 0) brightness = 0;
    if (brightness > 255) brightness = 255;

    device->brightness = brightness;
    device->is_on = brightness > 0;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, device->pwm_channel, brightness);

    ledc_update_duty(LEDC_LOW_SPEED_MODE, device->pwm_channel);
}