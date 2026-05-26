#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "dev_light.h"
#include "device_types.h"
#include "device_manager.h"

static const char *TAG = "Feat_Light";

static void lightDevice_initGpio(device_t *device)
{
    gpio_reset_pin(device->gpio_pin);
    gpio_set_direction(device->gpio_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(device->gpio_pin, 0);
}

void lightDevice_init(void)
{
#if CONFIG_FEATURE_DEVICE_LIGHT
    for (int i = 0; i < CONFIG_LIGHT_COUNT; i++) {
        device_t *device = deviceManager_getId(i);

        if (device == NULL) {
            continue;
        }

        device->id = i;
        device->name = "Light";
        device->type = DEVICE_TYPE_LIGHT;
        device->gpio_pin = CONFIG_LIGHT_START_GPIO + i;
        device->is_on = false;
        device->brightness = 0;

        lightDevice_initGpio(device);

        ESP_LOGI(TAG, "Light created with id %d on pin %d", i, device->gpio_pin);
    }
#endif
}