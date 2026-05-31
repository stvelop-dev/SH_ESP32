
#include <stddef.h>

#include "io_control.h"
#include "device_manager.h"
#include "io_control.h"

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

int ioControl_getDeviceCount(void)
{
    return deviceManager_getCount();
}

int ioControl_getValue(int id)
{
    device_t *dev = deviceManager_getId(id);

    if (dev == NULL) return -1;

    return deviceManager_getValue(dev);
}

ioControl_result_t ioControl_setDevice(int device_id, bool state)
{
    device_t *device = deviceManager_getId(device_id);

    if (device == NULL) {
        return IOCONTROL_ERRORNOTFOUND;
    }

    if (device->type == DEVICE_TYPE_SENSOR) {
        return IOCONTROL_ERRORNOTSWITCHABLE;
    }

    deviceManager_setOn(device, state);

    return IOCONTROL_OK;
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