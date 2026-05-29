#pragma once

typedef struct {
    bool active;
    int sensor_id;
    int target_device_id;
    bool trigger_state;
    bool target_state;
    bool already_triggered;
    int last_value;
} eventRule_t;

typedef enum {
    EVENTRULE_ADD_OK,
    EVENTRULE_ADD_ERRORNOSLOT,
    EVENTRULE_ADD_ERRORINVALIDSENSOR,
    EVENTRULE_ADD_ERRORINVALIDTARGET,
    EVENTRULE_ADD_ERRORTARGETNOTSWITCHABLE
} eventRule_addResult_t;

typedef enum {
    EVENTRULE_REMOVE_OK,
    EVENTRULE_REMOVE_ERRORINVALIDINDEX,
    EVENTRULE_REMOVE_ERRORNOTACTIVE
} eventRule_removeResult_t;

void automationEventbased_init(void);
void automationEventbased_process(void);
eventRule_addResult_t automationEventbased_addRule(int sensor_id, int target_device_id,
    bool trigger_state, bool target_state);
eventRule_removeResult_t automationEventbased_removeRule(int index);
int automationEventbased_getRuleCount(void);
eventRule_t *automationEventbased_getRule(int index);