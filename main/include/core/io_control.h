#pragma once

void io_control_init(void);

void ioControl_setDeviceOn(int device_id, bool state);
void ioControl_toggleDevice(int device_id);
void io_set_brightness(int device_id, int brightness);

int io_control_get_device_value(int id);