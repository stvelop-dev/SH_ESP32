#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "sys_time.h"

static const char *TAG = "Serv_SysTime";

static system_time_t current_time;


void systemTime_init(void)
{
    current_time.hour = 0;
    current_time.minute = 0;
    current_time.second = 0;
}

void systemTime_process(void)
{
    static TickType_t last_tick = 0;

    TickType_t now = xTaskGetTickCount();

    if ((now - last_tick) >= pdMS_TO_TICKS(1000)) {

        last_tick = now;

        current_time.second++;

        if (current_time.second >= 60) {
            current_time.second = 0;
            current_time.minute++;
        }

        if (current_time.minute >= 60) {
            current_time.minute = 0;
            current_time.hour++;
        }

        if (current_time.hour >= 24) {
            current_time.hour = 0;
        }
    }
}

void systemTime_set(int hour, int minute, int second)
{
    current_time.hour = hour;
    current_time.minute = minute;
    current_time.second = second;
    
    ESP_LOGI(TAG, "System time updated");
}

system_time_t systemTime_get(void)
{
    return current_time;
}