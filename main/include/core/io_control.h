#pragma once

#include "component_types.h"

typedef enum {
    IOCONTROL_OK,
    IOCONTROL_ERRORNOTFOUND,
    IOCONTROL_ERRORNOTSWITCHABLE
} ioControl_result_t;

void ioControl_init(void);

ioControl_result_t ioControl_setState(int device_id, bool state);
ioControl_result_t ioControl_setLevel(int device_id, int level);

int ioControl_getValue(int id);
int ioControl_getType(int id);
int ioControl_getDeviceCount();
bool ioControl_exists(int id);