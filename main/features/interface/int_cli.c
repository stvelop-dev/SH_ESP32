#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"

#include "int_cli.h"
#include "io_control.h"
#include "device_manager.h"

static const char *TAG = "Feat_CLI";

static void cliInterface_printHelp(void)
{
    printf("\nAvailable commands:\n");
    printf("  help              Show this help\n");
    printf("  list              List all devices\n");
    printf("  on <id>           Turn device on\n");
    printf("  off <id>          Turn device off\n");
    printf("  bright <id> <val> Set brightness, e.g. bright 0 80\n");
    printf("\n");
}

static void cliInterface_handleCommand(char *line)
{
    char cmd[16];
    int id;
    int value;

    if (sscanf(line, "%15s", cmd) != 1) {
        return;
    }

    if (strcmp(cmd, "on") == 0) {
        if (sscanf(line, "%*s %d", &id) == 1) {
            device_t *dev = deviceManager_getId(id);
            if (dev) {
                ioControl_setDevice(dev->id, true);
                printf("Device %d ON\n", id);
            }
        }
    }

    else if (strcmp(cmd, "off") == 0) {
        if (sscanf(line, "%*s %d", &id) == 1) {
            device_t *dev = deviceManager_getId(id);
            if (dev) {
                ioControl_setDevice(dev->id, false);
                printf("Device %d OFF\n", id);
            }
        }
    }

    else if (strcmp(cmd, "bright") == 0) {
        if (sscanf(line, "%*s %d %d", &id, &value) == 2) {
            device_t *dev = deviceManager_getId(id);
            if (dev) {
                ioControl_setBrightness(dev->id, value);
                printf("Device %d brightness %d\n", id, value);
            }
        }
    }

    else if (strcmp(cmd, "list") == 0) {
        int count = deviceManager_getCount();

        for (int i = 0; i < count; i++) {
            device_t *dev = deviceManager_getId(i);
            if (dev) {
                printf("ID:%d Name:%s On:%d Bright:%d\n",
                       dev->id, dev->name, dev->is_on, dev->brightness);
            }
        }
    }

    else if (strcmp(cmd, "help") == 0) {
        cliInterface_printHelp();
    }

    else {
        printf("Unknown command\n");
    }
}

static void cliInterface_loop(void)
{
    char line[64];
    int pos = 0;

    printf("Type 'help' for commands.\n");
    printf(">> ");
    fflush(stdout);

    while (true) {
        int c = getchar();

        if (c == EOF) {
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;
        }

        if (c == '\n' || c == '\r') {
            putchar('\n');

            line[pos] = '\0';

            if (pos > 0) {
                cliInterface_handleCommand(line);
            }

            pos = 0;
            printf(">> ");
            fflush(stdout);
        }
        else if (pos < sizeof(line) - 1) {
            line[pos++] = (char)c;

            putchar(c);
            fflush(stdout);
        }
    }
}

static void cliInterface_task(void *arg)
{
    (void)arg;
    cliInterface_loop();
}

void cliInterface_init(void)
{
#if CONFIG_FEATURE_INTERFACE_CLI
    ESP_LOGI(TAG, "CLI interface initialized");
    xTaskCreate(
        cliInterface_task,
        "cliInterface_task",
        4096,
        NULL,
        5,
        NULL
    );
#endif
}