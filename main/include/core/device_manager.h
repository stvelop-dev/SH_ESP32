#pragma once

#include <stdbool.h>
#include "device_types.h"

typedef struct {
    int id;
    const char *name;
    device_type_t type;
    int gpio_pin;
    bool is_on;
    int brightness;
} device_t;

void deviceManager_init(void);

int deviceManager_getCount(void);
int deviceManager_getValue(device_t *device);
device_t *deviceManager_getId(int id);

void deviceManager_setOn(device_t *device, bool on);
void deviceManager_setBrightness(device_t *device, int brightness);