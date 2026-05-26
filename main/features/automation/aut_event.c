#include <stddef.h>
#include "esp_log.h"

#include "aut_event.h"
#include "io_control.h"
#include "device_manager.h"

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

    device_t *sensor = device_get_by_id(sensor_id);
    if (sensor == NULL) return;
    if (sensor->type != DEVICE_TYPE_SENSOR) return;

    ESP_LOGI(TAG, "Sensor %d available", sensor->id);

    device_t *target = device_get_by_id(target_device_id);
    if (target->type != DEVICE_TYPE_LIGHT &&
        target->type != DEVICE_TYPE_RELAY) return;

    ESP_LOGI(TAG, "Target %d available", target->id);

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
        int value = io_control_get_device_value(rules[i].sensor_id);
        if (value < 0) continue;

        if (value != rules[i].last_value) {
            ESP_LOGI(TAG, "Sensor %d changed: %d -> %d",
                     rules[i].sensor_id,
                     rules[i].last_value,
                     value);

            rules[i].last_value = value;
        }
        
        device_t *sensor = device_get_by_id(rules[i].sensor_id);

        if (sensor == NULL) continue;

        if (value == rules[i].trigger_state) {
            if (!rules[i].already_triggered) {
                ioControl_setDeviceOn(rules[i].target_device_id, rules[i].target_state);
                rules[i].already_triggered = true;
            }
        } else {
            rules[i].already_triggered = false;
        }
    }
}