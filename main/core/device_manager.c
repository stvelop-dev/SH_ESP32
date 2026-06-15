#include <stddef.h>
#include "esp_log.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "device_manager.h"
#include "component_config.h"

static const char *TAG = "Core_DeMa";

static device_t devices[COMPONENT_COUNT];

void deviceManager_init(void)
{
    for (int i = 0; i < COMPONENT_COUNT; i++) {
        devices[i].id = i;
        devices[i].name = NULL;
        devices[i].type = COMPONENTS[i].type;
        devices[i].gpio_pin = COMPONENTS[i].gpio;
        devices[i].level = false;
        devices[i].level = 0;
    }

    ESP_LOGI(TAG, "%d devices created.", COMPONENT_COUNT);
}

int deviceManager_getCount(void)
{
    return COMPONENT_COUNT;
}

device_t *deviceManager_getId(int id)
{
    if (id < 0 || id >= COMPONENT_COUNT) {
        return NULL;
    }

    return &devices[id];
}

void deviceManager_setState(device_t *device, bool state)
{
    if (device->type == COMPONENT_OUTPUT_ANALOG) {
        if (state) {
            if (device->level == 0) {
                deviceManager_setLevel(device, 255);
            } else {
                deviceManager_setLevel(device, device->level);
            }
        } else {
            deviceManager_setLevel(device, 0);
        }
        return;
    }

    device->level = state;
    gpio_set_level(device->gpio_pin, state ? 1 : 0);
}

int deviceManager_getValue(device_t *device)
{
    switch (device->type) {

    case COMPONENT_INPUT_BINARY:
        device->level = gpio_get_level(device->gpio_pin);
        break;

    case COMPONENT_INPUT_ANALOG:
        //adc_oneshot_read(adc_handle, device->adc_channel, &device->level);
        break;

    default:
        break;
    }

    return device->level;
}

void deviceManager_setLevel(device_t *device, int level) 
{
    if (level < 0) level = 0;
    if (level > 255) level = 255;

    device->level = level;
    device->level = level > 0;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, device->pwm_channel, level);

    ledc_update_duty(LEDC_LOW_SPEED_MODE, device->pwm_channel);
}