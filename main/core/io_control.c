
#include <stddef.h>

#include "io_control.h"
#include "device_manager.h"

void ioControl_init(void)
{
    deviceManager_init();
}

bool ioControl_exists(int id)
{
    return deviceManager_getId(id) != NULL;
}

int ioControl_getType(int id)
{
    device_t *dev = deviceManager_getId(id);

    if (dev == NULL) return -1;

    return dev->type;
}

int ioControl_getValue(int id)
{
    device_t *dev = deviceManager_getId(id);

    if (dev == NULL) return -1;

    return deviceManager_getValue(dev);
}

void ioControl_setDevice(int device_id, bool state)
{
    device_t *device = deviceManager_getId(device_id);

    if (device == NULL) {
        return;
    }

    deviceManager_setOn(device, state);
}

void ioControl_setBrightness(int device_id, int brightness)
{
    device_t *device = deviceManager_getId(device_id);

    if (device == NULL) {
        return;
    }

    if (device->type != DEVICE_TYPE_LIGHT) {
        return;
    }

    deviceManager_setBrightness(device, brightness);
}