#include <stdio.h>

#include "feature_registry.h"
#include "io_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    //setup
    ioControl_init();
    featureRegistry_Init();
    
    //loop
    while (true) {
        featureRegistry_Process();
    }
}
