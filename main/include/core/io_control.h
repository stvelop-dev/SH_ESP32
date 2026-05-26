#pragma once

void io_control_init(void);

void io_turn_on(int device_id);
void io_turn_off(int device_id);
int io_control_get_device_value(int id);
void io_set_brightness(int device_id, int brightness);