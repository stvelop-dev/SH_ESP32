#pragma once

#include <stdbool.h>
#include "component_types.h"

typedef struct {
    int id;
    const char *name;
    component_type_t type;
    int gpio_pin;
    int pwm_channel;
    int level;
} device_t;

void deviceManager_init(void);

int deviceManager_getCount(void);
int deviceManager_getValue(device_t *device);
device_t *deviceManager_getId(int id);

void deviceManager_setState(device_t *device, bool on);
void deviceManager_setLevel(device_t *device, int level);