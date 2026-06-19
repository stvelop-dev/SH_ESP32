#pragma once

#include "sdkconfig.h"
#include "esp_adc/adc_oneshot.h"

#if defined(CONFIG_IDF_TARGET_ESP32)

#define COMPONENT_GPIO_LIST(X) \
    X(2)  \
    X(4)  \
    X(25) \
    X(26) \
    X(27) \
    X(32) \
    X(33) \
    X(34)

#define COMPONENT_GPIO_PWM_LIST(X) \
    X(2)  \
    X(4)  \
    X(25) \
    X(26) \
    X(27) \
    X(32) \
    X(33)

#define COMPONENT_GPIO_ADC_LIST(X) \
    X(32, ADC_UNIT_1, ADC_CHANNEL_4) \
    X(33, ADC_UNIT_1, ADC_CHANNEL_5) \
    X(34, ADC_UNIT_1, ADC_CHANNEL_6)

#elif defined(CONFIG_IDF_TARGET_ESP32C6)

#define COMPONENT_GPIO_LIST(X) \
    X(0) \
    X(1) \
    X(2) \
    X(3) \
    X(4) \
    X(5) \
    X(6) \
    X(7)

#define COMPONENT_GPIO_PWM_LIST(X) \

#define COMPONENT_GPIO_ADC_LIST(X)

#else

#define COMPONENT_GPIO_LIST(X)
#define COMPONENT_GPIO_PWM_LIST(X)
#define COMPONENT_GPIO_ADC_LIST(X)

#endif