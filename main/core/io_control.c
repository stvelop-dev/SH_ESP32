
#include <stddef.h>
#include <stdbool.h>

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

int ioControl_getDeviceCount()
{
    return deviceManager_getCount();
}

int ioControl_getValue(int id)
{
    device_t *dev = deviceManager_getId(id);

    if (dev == NULL) return -1;

    return deviceManager_getValue(dev);
}

ioControl_result_t ioControl_setState(int device_id, bool state)
{
    device_t *device = deviceManager_getId(device_id);

    if (device == NULL) {
        return IOCONTROL_ERRORNOTFOUND;
    }

    if (device->type != COMPONENT_OUTPUT_BINARY && device->type != COMPONENT_OUTPUT_ANALOG) {
        return IOCONTROL_ERRORNOTSWITCHABLE;
    }

    deviceManager_setState(device, state);
    return IOCONTROL_OK;
}

ioControl_result_t ioControl_setLevel(int device_id, int level)
{
    device_t *device = deviceManager_getId(device_id);

    if (device == NULL) {
        return IOCONTROL_ERRORNOTFOUND;;
    }

    if (device->type != COMPONENT_OUTPUT_ANALOG) {
        return IOCONTROL_ERRORNOTSWITCHABLE;
    }

    deviceManager_setLevel(device, level);
    return IOCONTROL_OK;
}