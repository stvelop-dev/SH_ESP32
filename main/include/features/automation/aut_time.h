#pragma once
#include <stdbool.h>

typedef struct {
    bool active;
    int hour;
    int minute;
    int device_id;
    bool turn_on;
} timeRule_t;

typedef enum {
    TIMERULE_ADD_OK,
    TIMERULE_ADD_ERRORINVALIDDEVICE,
    TIMERULE_ADD_ERRORNOTSWITCHABLE,
    TIMERULE_ADD_ERRORNOSLOT,
    TIMERULE_ADD_ERRORINVALIDTIME
} timeRule_addResult_t;

typedef enum {
    TIMERULE_REMOVE_OK,
    TIMERULE_REMOVE_ERRORINVALIDINDEX,
    TIMERULE_REMOVE_ERRORNOTACTIVE
} timeRule_removeResult_t;

void automationTimebased_init(void);
void automationTimebased_process(void);
timeRule_addResult_t automationTimebased_addRule(int hour, int minute, int device_id, bool state);
int automationTimebased_getRuleCount(void);
timeRule_t *automationTimebased_getRule(int index);
timeRule_removeResult_t automationTimebased_removeRule(int index);