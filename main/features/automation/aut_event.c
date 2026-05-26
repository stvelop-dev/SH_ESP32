#include <stddef.h>
#include "esp_log.h"

#include "aut_event.h"
#include "io_control.h"
#include "device_types.h"

#define MAX_EVENT_RULES 4

static const char *TAG = "Feat_Event";

static event_rule_t rules[MAX_EVENT_RULES];
static int rule_count = 0;

void automationEventbased_init(void)
{
     //automationEventbased_addRule(2, 0, true, true);
     //automationEventbased_addRule(2, 0, false, false);
}

void automationEventbased_addRule(int sensor_id, int target_device_id,
    bool trigger_state, bool target_state)
{
    ESP_LOGI(TAG, "Create Eventrule:");
    if (rule_count >= MAX_EVENT_RULES) return;

    if (ioControl_getType(sensor_id) != DEVICE_TYPE_SENSOR) return;
    ESP_LOGI(TAG, "Sensor %d available", sensor_id);

    int target_type = ioControl_getType(target_device_id);
    if (target_type != DEVICE_TYPE_LIGHT &&
        target_type != DEVICE_TYPE_RELAY) return;
    ESP_LOGI(TAG, "Target %d available", target_device_id);

    rules[rule_count++] = (event_rule_t){
        sensor_id,
        target_device_id,
        trigger_state,
        target_state,
        false,
        -1
    };

    ESP_LOGI(TAG, "Eventrule Created");
}

void automationEventbased_process(void)
{
    for (int i = 0; i < rule_count; i++) {
        int value = ioControl_getValue(rules[i].sensor_id);
        if (value < 0) continue;

        if (value != rules[i].last_value) {
            ESP_LOGI(TAG, "Sensor %d changed: %d -> %d",
                     rules[i].sensor_id,
                     rules[i].last_value,
                     value);

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