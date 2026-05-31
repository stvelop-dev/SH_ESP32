#pragma once

typedef struct {
    int hour;
    int minute;
    int second;
} systemTime_t;

void systemTime_init(void);
void systemTime_process(void);
void systemTime_set(int hour, int minute, int second);
systemTime_t systemTime_get(void);