#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "aut_time.h"
#include "io_control.h"
#include "device_types.h"

#define MAX_RULES 4

static const char *TAG = "Feat_Time";

static time_rule_t rules[MAX_RULES];
static int rule_count = 0;

static void fakeTime_get(int *hour, int *minute)
{
    static int fake_hour = 0;
    static int fake_minute = 0;

    fake_minute++;

    if (fake_minute >= 60) {
        fake_minute = 0;
        fake_hour++;

        if (fake_hour >= 24) {
            fake_hour = 0;
        }
    }

    *hour = fake_hour;
    *minute = fake_minute;
}

void automationTimebased_init(void)
{
    //automationTimebased_addRule(0, 5, 0, true);
    //automationTimebased_addRule(0, 10, 0, false);
}

void automationTimebased_addRule(int hour, int minute, int device_id, bool state)
{
    ESP_LOGI(TAG, "Create Timerule:");
    if (rule_count >= MAX_RULES) return;

    int target_type = ioControl_getType(device_id);
    if (target_type != DEVICE_TYPE_LIGHT &&
        target_type != DEVICE_TYPE_RELAY) return;

    ESP_LOGI(TAG, "Device %d available", device_id);

    rules[rule_count++] = (time_rule_t){hour, minute, device_id, state};

    ESP_LOGI(TAG, "Timerule Created");
}

void automationTimebased_process(void)
{
    int hour, minute;
    fakeTime_get(&hour, &minute);

    for (int i = 0; i < rule_count; i++) {
        if (rules[i].hour == hour &&
            rules[i].minute == minute) {

            ioControl_setDevice(rules[i].device_id, rules[i].turn_on);
            ESP_LOGI(TAG, "Timerule %d triggered", i);
        }
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
}