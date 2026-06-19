#include <stddef.h>
#include "esp_log.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"

#include "device_manager.h"
#include "component_config.h"

#define DEVICE_MAX_COUNT 32

static const char *TAG = "Core_DeMa";

static device_t devices[DEVICE_MAX_COUNT];
static int device_count = 0;

void deviceManager_init(void)
{
    size_t count = componentConfig_getCount();

    if (count > DEVICE_MAX_COUNT) {
        ESP_LOGE(TAG, "Too many devices: %u > %d", (unsigned)count, DEVICE_MAX_COUNT);
        count = DEVICE_MAX_COUNT;
    }

    device_count = (int)count;

    for (int i = 0; i < device_count; i++) {
        component_config_t config;

        if (!componentConfig_get((size_t)i, &config)) {
            continue;
        }

        devices[i].id = i;
        devices[i].name = NULL;
        devices[i].type = config.type;
        devices[i].gpio_pin = config.gpio;
        devices[i].pull_mode = (int)config.pull_mode;
        devices[i].driver.frequency = config.frequency;
        devices[i].level = 0;
    }

    ESP_LOGI(TAG, "%d devices created.", device_count);
}

int deviceManager_getCount(void)
{
    return device_count;
}

device_t *deviceManager_getId(int id)
{
    if (id < 0 || id >= device_count) {
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
    if (device->type == COMPONENT_INPUT_ANALOG) 
    {
        int raw;

        adc_oneshot_unit_handle_t adc_handle = (adc_oneshot_unit_handle_t)device->driver.handle;

        if (adc_handle == NULL) {
            return -1;
        }

        adc_oneshot_read(adc_handle, (adc_channel_t)device->driver.channel, &raw);

        device->level = raw;
        return device->level;
    }

    device->level = gpio_get_level(device->gpio_pin);
    return device->level;
}

void deviceManager_setLevel(device_t *device, int level) 
{
    if (level < 0) level = 0;
    if (level > 255) level = 255;

    device->level = level;

    ledc_set_duty(LEDC_LOW_SPEED_MODE, device->driver.channel, level);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, device->driver.channel);
}