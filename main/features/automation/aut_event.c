#include <stddef.h>
#include "esp_log.h"

#include "aut_event.h"
#include "io_control.h"
#include "device_types.h"

#define MAX_RULES 4

static const char *TAG = "Feat_Event";

static eventRule_t rules[MAX_RULES];

void automationEventbased_init(void)
{
     //automationEventbased_addRule(2, 0, true, true);
     //automationEventbased_addRule(2, 0, false, false);
}

eventRule_addResult_t automationEventbased_addRule(int sensor_id, int target_device_id,
    bool trigger_state, bool target_state)
{
    ESP_LOGI(TAG, "Create Eventrule:");

    if (ioControl_getType(sensor_id) != DEVICE_TYPE_SENSOR) {
        return EVENTRULE_ADD_ERRORINVALIDSENSOR;
    }

    int target_type = ioControl_getType(target_device_id);

    if (target_type < 0) {
        return EVENTRULE_ADD_ERRORINVALIDTARGET;
    }

    if (target_type != DEVICE_TYPE_LIGHT &&
        target_type != DEVICE_TYPE_RELAY) {
        return EVENTRULE_ADD_ERRORTARGETNOTSWITCHABLE;
    }

    for (int i = 0; i < MAX_RULES; i++) {
        if (!rules[i].active) {
            rules[i].active = true;
            rules[i].sensor_id = sensor_id;
            rules[i].target_device_id = target_device_id;
            rules[i].trigger_state = trigger_state;
            rules[i].target_state = target_state;
            rules[i].already_triggered = false;
            rules[i].last_value = -1;

            ESP_LOGI(TAG, "Eventrule created at slot %d", i);
            return EVENTRULE_ADD_OK;
        }
    }

    return EVENTRULE_ADD_ERRORNOSLOT;
}

eventRule_removeResult_t automationEventbased_removeRule(int index)
{
    if (index < 0 || index >= MAX_RULES) {
        return EVENTRULE_REMOVE_ERRORINVALIDINDEX;
    }

    if (!rules[index].active) {
        return EVENTRULE_REMOVE_ERRORNOTACTIVE;
    }

    rules[index].active = false;

    ESP_LOGI(TAG, "Eventrule %d removed", index);
    return EVENTRULE_REMOVE_OK;
}

int automationEventbased_getRuleCount(void)
{
    return MAX_RULES;
}

eventRule_t *automationEventbased_getRule(int index)
{
    if (index < 0 || index >= MAX_RULES) {
        return NULL;
    }

    return &rules[index];
}

void automationEventbased_process(void)
{
    for (int i = 0; i < MAX_RULES; i++) {
        if (!rules[i].active) {
            continue;
        }

        int value = ioControl_getValue(rules[i].sensor_id);
        if (value < 0) {
            continue;
        }

        if (value != rules[i].last_value) {
            ESP_LOGI(TAG, "Sensor %d changed: %d -> %d", rules[i].sensor_id, rules[i].last_value, value);
            rules[i].last_value = value;
        }

        if (value == rules[i].trigger_state) {
            if (!rules[i].already_triggered) {
                ioControl_setDevice(rules[i].target_device_id, rules[i].target_state);
                rules[i].already_triggered = true;
            }
        } else {
            rules[i].already_triggered = false;
        }
    }
}