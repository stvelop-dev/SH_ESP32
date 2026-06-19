#include "component_config.h"
#include "component_pins.h"
#include "sdkconfig.h"

/* ---------- IS_ENABLED helper ---------- */

#define __ARG_PLACEHOLDER_1 0,
#define __take_second_arg(_ignored, val, ...) val
#define __is_enabled(value) ___is_enabled(__ARG_PLACEHOLDER_##value)
#define ___is_enabled(arg1_or_junk) __take_second_arg(arg1_or_junk 1, 0)

#define IS_ENABLED(config) __is_enabled(config)

/* ---------- CONFIG name builder ---------- */

#define GPIO_CONFIG(gpio, option) \
    CONFIG_FEATURE_COMPONENT_GPIO##gpio##_##option

#define GPIO_NUM_FROM_PIN(gpio) \
    GPIO_NUM_##gpio

#define GPIO_PWM_TIMER(gpio) \
    (IS_ENABLED(GPIO_CONFIG(gpio, OUTPUT_ANALOG_TIMER_0)) ? 0 : \
    (IS_ENABLED(GPIO_CONFIG(gpio, OUTPUT_ANALOG_TIMER_1)) ? 1 : \
    (IS_ENABLED(GPIO_CONFIG(gpio, OUTPUT_ANALOG_TIMER_2)) ? 2 : 3)))

#define PWM_FREQ(timer) \
    ((timer) == 0 ? CONFIG_FEATURE_COMPONENT_PWM_FREQ_0 : \
    ((timer) == 1 ? CONFIG_FEATURE_COMPONENT_PWM_FREQ_1 : \
    ((timer) == 2 ? CONFIG_FEATURE_COMPONENT_PWM_FREQ_2 : \
                   CONFIG_FEATURE_COMPONENT_PWM_FREQ_3)))

#define GPIO_PULL_MODE(gpio) \
    (IS_ENABLED(GPIO_CONFIG(gpio, INPUT_BINARY_PULLUP)) ? GPIO_PULLUP_ONLY : \
    (IS_ENABLED(GPIO_CONFIG(gpio, INPUT_BINARY_PULLDOWN)) ? GPIO_PULLDOWN_ONLY : GPIO_FLOATING))

/* ---------- Internal tables ---------- */

typedef struct {
    bool enabled;
    component_config_t config;
} component_candidate_t;

typedef struct {
    bool enabled;
    adc_config_t config;
} adc_candidate_t;

/* Only PWM-capable GPIOs */
#define OUTPUT_ANALOG_CANDIDATE_FOR_GPIO(gpio) \
    { IS_ENABLED(GPIO_CONFIG(gpio, OUTPUT_ANALOG)), \
        { GPIO_NUM_FROM_PIN(gpio), COMPONENT_OUTPUT_ANALOG, GPIO_FLOATING, PWM_FREQ(GPIO_PWM_TIMER(gpio)), GPIO_PWM_TIMER(gpio) } },

/* All normal component GPIOs */
#define OTHER_CANDIDATES_FOR_GPIO(gpio) \
    { IS_ENABLED(GPIO_CONFIG(gpio, OUTPUT_BINARY)), \
        { GPIO_NUM_FROM_PIN(gpio), COMPONENT_OUTPUT_BINARY, GPIO_FLOATING, 0, -1 } }, \
    { IS_ENABLED(GPIO_CONFIG(gpio, INPUT_BINARY)), \
        { GPIO_NUM_FROM_PIN(gpio), COMPONENT_INPUT_BINARY, GPIO_PULL_MODE(gpio), 0, -1 } }, \
    { IS_ENABLED(GPIO_CONFIG(gpio, INPUT_ANALOG)), \
        { GPIO_NUM_FROM_PIN(gpio), COMPONENT_INPUT_ANALOG, GPIO_FLOATING, 0, -1 } },

/* Only ADC-capable GPIOs */
#define ADC_CANDIDATE_FOR_GPIO(gpio, unit_id, adc_channel) \
    { IS_ENABLED(GPIO_CONFIG(gpio, INPUT_ANALOG)), \
        { GPIO_NUM_FROM_PIN(gpio), unit_id, adc_channel } },

static const component_candidate_t COMPONENT_CANDIDATES[] = {
    COMPONENT_GPIO_PWM_LIST(OUTPUT_ANALOG_CANDIDATE_FOR_GPIO)
    COMPONENT_GPIO_LIST(OTHER_CANDIDATES_FOR_GPIO)
};

static const adc_candidate_t ADC_CANDIDATES[] = {
    COMPONENT_GPIO_ADC_LIST(ADC_CANDIDATE_FOR_GPIO)
};

#define COMPONENT_CANDIDATE_COUNT \
    (sizeof(COMPONENT_CANDIDATES) / sizeof(COMPONENT_CANDIDATES[0]))

#define ADC_CANDIDATE_COUNT \
    (sizeof(ADC_CANDIDATES) / sizeof(ADC_CANDIDATES[0]))

/* ---------- Public API ---------- */

size_t componentConfig_getCount(void)
{
    size_t count = 0;

    for (size_t i = 0; i < COMPONENT_CANDIDATE_COUNT; i++) {
        if (COMPONENT_CANDIDATES[i].enabled) {
            count++;
        }
    }

    return count;
}

size_t componentConfig_getCountByType(component_type_t type)
{
    size_t count = 0;

    for (size_t i = 0; i < COMPONENT_CANDIDATE_COUNT; i++) {
        if (COMPONENT_CANDIDATES[i].enabled &&
            COMPONENT_CANDIDATES[i].config.type == type) {
            count++;
        }
    }

    return count;
}

bool componentConfig_get(size_t index, component_config_t *config)
{
    size_t current = 0;

    if (config == NULL) {
        return false;
    }

    for (size_t i = 0; i < COMPONENT_CANDIDATE_COUNT; i++) {
        if (!COMPONENT_CANDIDATES[i].enabled) {
            continue;
        }

        if (current == index) {
            *config = COMPONENT_CANDIDATES[i].config;
            return true;
        }

        current++;
    }

    return false;
}

size_t componentConfig_getAdcCount(void)
{
    size_t count = 0;

    for (size_t i = 0; i < ADC_CANDIDATE_COUNT; i++) {
        if (ADC_CANDIDATES[i].enabled) {
            count++;
        }
    }

    return count;
}

bool componentConfig_getAdc(size_t index, adc_config_t *config)
{
    size_t current = 0;

    if (config == NULL) {
        return false;
    }

    for (size_t i = 0; i < ADC_CANDIDATE_COUNT; i++) {
        if (!ADC_CANDIDATES[i].enabled) {
            continue;
        }

        if (current == index) {
            *config = ADC_CANDIDATES[i].config;
            return true;
        }

        current++;
    }

    return false;
}