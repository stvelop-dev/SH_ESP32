#pragma once

#include "device_types.h"

void ioControl_init(void);

void ioControl_setDevice(int device_id, bool state);
void ioControl_toggleDevice(int device_id);
void ioControl_setBrightness(int device_id, int brightness);

int ioControl_getValue(int id);
int ioControl_getType(int id);
bool ioControl_exists(int id);