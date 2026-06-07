#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "dev_sensor.h"
#include "device_types.h"
#include "device_manager.h"

static const char *TAG = "Feat_Sensor";

static void sensorDevice_initGpio(device_t *device)
{
    gpio_reset_pin(device->gpio_pin);
    gpio_set_direction(device->gpio_pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(device->gpio_pin, GPIO_PULLUP_ONLY);
}

void sensorDevice_init(void)
{
#if CONFIG_FEATURE_DEVICE_SENSOR
    int start_id = 0;

#if CONFIG_FEATURE_DEVICE_LIGHT
    start_id += CONFIG_FEATURE_DEVICE_LIGHT_COUNT;
#endif

#if CONFIG_FEATURE_DEVICE_RELAY
    start_id += CONFIG_FEATURE_DEVICE_RELAY_COUNT;
#endif

    for (int i = 0; i < CONFIG_FEATURE_DEVICE_SENSOR_COUNT; i++) {
        int id = start_id + i;

        device_t *device = deviceManager_getId(id);

        if (device == NULL) {
            continue;
        }

        device->id = id;
        device->name = "Sensor";
        device->type = DEVICE_TYPE_SENSOR;
        device->gpio_pin = CONFIG_FEATURE_DEVICE_SENSOR_STARTIO + i;
        device->is_on = false;
        device->brightness = -1;

        sensorDevice_initGpio(device);

        ESP_LOGI(TAG, "Sensor created with id %d on pin %d", id, device->gpio_pin);
    }
#endif
}