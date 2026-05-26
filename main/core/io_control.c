
#include <stddef.h>

#include "io_control.h"
#include "device_manager.h"

void io_control_init(void)
{
    deviceManager_init();
}

void ioControl_setDeviceOn(int device_id, bool state)
{
    device_t *device = device_get_by_id(device_id);

    if (device == NULL) {
        return;
    }

    device_set_on(device, state);
}

void ioControl_toggleDevice(int device_id)
{
    device_t *device = device_get_by_id(device_id);

    if (device == NULL) {
        return;
    }

    device_set_on(device, !device->is_on);
}

int io_control_get_device_value(int id)
{
    device_t *dev = device_get_by_id(id);

    if (dev == NULL) return -1;

    return device_get_value(dev);
}

void io_set_brightness(int device_id, int brightness)
{
    device_t *device = device_get_by_id(device_id);

    if (device == NULL) {
        return;
    }

    if (device->type != DEVICE_TYPE_LIGHT) {
        return;
    }

    device_set_brightness(device, brightness);
}