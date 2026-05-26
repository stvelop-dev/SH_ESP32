#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "dev_relay.h"
#include "device_manager.h"

static const char *TAG = "Feat_Relay";

static void relayDevice_initGpio(device_t *device)
{
    gpio_reset_pin(device->gpio_pin);
    gpio_set_direction(device->gpio_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(device->gpio_pin, 0);
}

void relayDevice_init(void)
{
#if CONFIG_FEATURE_DEVICE_RELAY
    int start_id = 0;

#if CONFIG_FEATURE_DEVICE_LIGHT
    start_id = CONFIG_LIGHT_COUNT;
#endif

    for (int i = 0; i < CONFIG_RELAY_COUNT; i++) {
        int id = start_id + i;

        device_t *device = device_get_by_id(id);

        if (device == NULL) {
            continue;
        }

        device->id = id;
        device->name = "Relay";
        device->type = DEVICE_TYPE_RELAY;
        device->gpio_pin = CONFIG_RELAY_START_GPIO + i;
        device->is_on = false;
        device->brightness = -1;

        relayDevice_initGpio(device);

        ESP_LOGI(TAG, "Relay created with id %d", id);
    }
#endif
}