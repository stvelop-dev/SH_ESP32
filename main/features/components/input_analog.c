#include "component_config.h"

#if INPUT_ANALOG_COUNT > 0

#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "input_analog.h"
#include "component_types.h"
#include "device_manager.h"

static const char *TAG = "Feat_InAna";

static void analogInput_initGpio(device_t *device)
{
    gpio_reset_pin(device->gpio_pin);
    gpio_set_direction(device->gpio_pin, GPIO_MODE_INPUT);
}

void analogInput_init(void)
{
    int count = deviceManager_getCount();

    for (int i = 0; i < count; i++) {
        device_t *device = deviceManager_getId(i);

        if (device == NULL) {
            continue;
        }

        if (device->type != COMPONENT_INPUT_ANALOG) {
            continue;
        }

        device->name = "Analog Input";
        device->level = false;
        device->level = -1;

        analogInput_initGpio(device);

        device->level = gpio_get_level(device->gpio_pin);

        ESP_LOGI(TAG, "Analog Input created with id %d on pin %d", device->id, device->gpio_pin);
    }
}

#endif