#pragma once

typedef struct {
    int sensor_id;
    int target_device_id;
    bool trigger_state;
    bool target_state;
    bool already_triggered;
    int last_value;
} event_rule_t;

void automationEventbased_init(void);
void automationEventbased_process(void);
void automationEventbased_addRule(int sensor_id, int target_device_id,
    bool trigger_state, bool target_state);