#include <stdio.h>

#include "update_ota.h"
#include "feature_registry.h"
#include "io_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    //setup
    ota_init();
    ioControl_init();
    featureRegistry_Init();
    
    //loop
    while (true) {
        featureRegistry_Process();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
