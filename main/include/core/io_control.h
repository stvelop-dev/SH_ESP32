#pragma once

#include "device_types.h"

typedef enum {
    IOCONTROL_OK,
    IOCONTROL_ERRORNOTFOUND,
    IOCONTROL_ERRORNOTSWITCHABLE
} ioControl_result_t;

void ioControl_init(void);

ioControl_result_t ioControl_setDevice(int device_id, bool state);
void ioControl_setBrightness(int device_id, int brightness);

int ioControl_getValue(int id);
int ioControl_getType(int id);
bool ioControl_exists(int id);