#include <stddef.h>

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

#define DEVICE_COUNT (LIGHT_COUNT + RELAY_COUNT)

static device_t devices[DEVICE_COUNT];

static void device_init_gpio(device_t *device)
{
    gpio_reset_pin(device->gpio_pin);
    gpio_set_direction(device->gpio_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(device->gpio_pin, 0);
}

void device_manager_init(void)
{
    int index = 0;

#if CONFIG_FEATURE_DEVICE_LIGHT
    for (int i = 0; i < LIGHT_COUNT; i++) {
        devices[index].id = index;
        devices[index].name = "Light";
        devices[index].type = DEVICE_TYPE_LIGHT;
        devices[index].gpio_pin = CONFIG_LIGHT_START_GPIO + i;
        devices[index].is_on = false;
        devices[index].brightness = 0;
        device_init_gpio(&devices[index]);
        index++;
    }
#endif

#if CONFIG_FEATURE_DEVICE_RELAY
    for (int i = 0; i < RELAY_COUNT; i++) {
        devices[index].id = index;
        devices[index].name = "Relay";
        devices[index].type = DEVICE_TYPE_RELAY;
        devices[index].gpio_pin = CONFIG_RELAY_START_GPIO + i;
        devices[index].is_on = false;
        devices[index].brightness = -1;
        device_init_gpio(&devices[index]);
        index++;
    }
#endif
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
