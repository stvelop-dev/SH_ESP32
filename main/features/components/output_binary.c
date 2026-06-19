#include "component_config.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "output_binary.h"
#include "component_types.h"
#include "device_manager.h"

static const char *TAG = "Feat_OutBin";

static void binaryOutput_initGpio(device_t *device)
{
    gpio_reset_pin(device->gpio_pin);
    gpio_set_direction(device->gpio_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(device->gpio_pin, 0);
}

void binaryOutput_init(void)
{
    int count = deviceManager_getCount();

    for (int i = 0; i < count; i++) {
        device_t *device = deviceManager_getId(i);

        if (device == NULL) {
            continue;
        }

        if (device->type != COMPONENT_OUTPUT_BINARY) {
            continue;
        }

        device->name = "Binary Output";
        device->level = 0;

        binaryOutput_initGpio(device);

        ESP_LOGI(TAG, "Binary Output created with id %d on pin %d", device->id, device->gpio_pin);
    }
}
