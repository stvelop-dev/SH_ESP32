#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "component_types.h"

typedef struct {
    gpio_num_t gpio;
    component_type_t type;
    gpio_pull_mode_t pull_mode;
    uint32_t frequency;
    int timer;
} component_config_t;

typedef struct {
    gpio_num_t gpio;
    adc_unit_t unit;
    adc_channel_t channel;
} adc_config_t;

size_t componentConfig_getCount(void);
size_t componentConfig_getCountByType(component_type_t type);
bool componentConfig_get(size_t index, component_config_t *config);

size_t componentConfig_getAdcCount(void);
bool componentConfig_getAdc(size_t index, adc_config_t *config);