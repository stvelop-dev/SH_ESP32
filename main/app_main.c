#include <stdio.h>

#include "feature_registry.h"
#include "io_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    io_control_init();
    feature_init_all();
    
    while (true) {
        io_turn_on(0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        io_turn_off(0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
