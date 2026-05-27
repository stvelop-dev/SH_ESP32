#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "aut_time.h"
#include "io_control.h"
#include "device_types.h"
#include "sys_time.h"

#define MAX_RULES 4

static const char *TAG = "Feat_Time";

static time_rule_t rules[MAX_RULES];

void automationTimebased_init(void)
{
    //automationTimebased_addRule(0, 5, 0, true);
    //automationTimebased_addRule(0, 10, 0, false);
}

void automationTimebased_addRule(int hour, int minute, int device_id, bool state)
{
    ESP_LOGI(TAG, "Create Timerule:");

    int target_type = ioControl_getType(device_id);
    if (target_type != DEVICE_TYPE_LIGHT &&
        target_type != DEVICE_TYPE_RELAY) return;

    ESP_LOGI(TAG, "Device %d available", device_id);

    for (int i = 0; i < MAX_RULES; i++) {
        if (!rules[i].active) {
            rules[i].active = true;
            rules[i].hour = hour;
            rules[i].minute = minute;
            rules[i].device_id = device_id;
            rules[i].turn_on = state;
            ESP_LOGI(TAG, "Timerule created at slot %d", i);
            return;
        }
    }

    ESP_LOGW(TAG, "No free rule slots");
}

int automationTimebased_getRuleCount(void)
{
    return MAX_RULES;
}

time_rule_t *automationTimebased_getRule(int index)
{
    if (index < 0 || index >= MAX_RULES) {
        return NULL;
    }

    return &rules[index];
}

void automationTimebased_removeRule(int index)
{
    if (index < 0 || index >= MAX_RULES) {
        return;
    }

    rules[index].active = false;
    ESP_LOGI(TAG, "Rule %d removed", index);
}

void automationTimebased_process(void)
{
    system_time_t time = systemTime_get();

    for (int i = 0; i < MAX_RULES; i++) {
        if (!rules[i].active) {
            continue;
        }

        if (rules[i].hour == time.hour &&
            rules[i].minute == time.minute &&
            time.second == 0) {

            ioControl_setDevice(rules[i].device_id, rules[i].turn_on);
            ESP_LOGI(TAG, "Timerule %d triggered", i);
        }
    }
}