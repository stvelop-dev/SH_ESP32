#pragma once
#include <stdbool.h>

typedef struct {
    bool active;
    int hour;
    int minute;
    int device_id;
    bool turn_on;
} time_rule_t;

void automationTimebased_init(void);
void automationTimebased_process(void);
void automationTimebased_addRule(int hour, int minute, int device_id, bool on);
int automationTimebased_getRuleCount(void);
time_rule_t *automationTimebased_getRule(int index);
void automationTimebased_removeRule(int index);