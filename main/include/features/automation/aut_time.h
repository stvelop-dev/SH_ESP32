#pragma once
#include <stdbool.h>

typedef struct {
    int hour;
    int minute;
    int device_id;
    bool turn_on;
} time_rule_t;

void automationTimebased_init(void);
void automationTimebased_process(void);
void automationTimebased_addRule(int hour, int minute, int device_id, bool on);