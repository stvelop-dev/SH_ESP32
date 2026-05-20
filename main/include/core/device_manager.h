#pragma once

#include <stdbool.h>

typedef enum {
    DEVICE_TYPE_LIGHT,
    DEVICE_TYPE_RELAY
} device_type_t;

typedef struct {
    int id;
    const char *name;
    device_type_t type;
    int gpio_pin;
    bool is_on;
    int brightness;
} device_t;

void device_manager_init(void);

int device_get_count(void);
device_t *device_get_by_id(int id);

void device_set_on(device_t *device, bool on);
void device_set_brightness(device_t *device, int brightness);