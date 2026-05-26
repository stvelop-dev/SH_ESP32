#include "dev_sensor.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "device_manager.h"

static void sensor_initGpio(device_t *device)
{
    gpio_reset_pin(device->gpio_pin);
    gpio_set_direction(device->gpio_pin, GPIO_MODE_INPUT);
}

void sensorDevice_init(void)
{
#if CONFIG_FEATURE_DEVICE_SENSOR
    int start_id = 0;

#if CONFIG_FEATURE_DEVICE_LIGHT
    start_id += CONFIG_LIGHT_COUNT;
#endif

#if CONFIG_FEATURE_DEVICE_RELAY
    start_id += CONFIG_RELAY_COUNT;
#endif

    for (int i = 0; i < CONFIG_SENSOR_COUNT; i++) {
        int id = start_id + i;

        device_t *device = device_get_by_id(id);

        if (device == NULL) {
            continue;
        }

        device->id = id;
        device->name = "Sensor";
        device->type = DEVICE_TYPE_SENSOR;
        device->gpio_pin = CONFIG_SENSOR_START_GPIO + i;
        device->is_on = false;
        device->brightness = -1;

        sensor_initGpio(device);
    }
#endif
}