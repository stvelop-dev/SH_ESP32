#pragma once

#include "esp_err.h"
#include <stdbool.h>

esp_err_t ota_init(void);
esp_err_t ota_start(const char *url);
bool ota_isRunning(void);