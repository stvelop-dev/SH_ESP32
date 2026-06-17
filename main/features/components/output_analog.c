#include "component_config.h"

#if OUTPUT_ANALOG_COUNT > 0

#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "sdkconfig.h"

#include "output_analog.h"
#include "component_types.h"
#include "device_manager.h"

static const char *TAG = "Feat_OutAna";

static void analogOutput_initGpio(device_t *device)
{
    ledc_channel_config_t channel = {
        .gpio_num = device->gpio_pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = device->driver.channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&channel);
}

static void analogOutput_initPwm(void)
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&timer);
}

void analogOutput_init(void)
{
    analogOutput_initPwm();
    int pwm_channel = 0;

    int count = deviceManager_getCount();

    for (int i = 0; i < count; i++) {
        device_t *device = deviceManager_getId(i);

        if (device == NULL) {
            continue;
        }

        if (device->type != COMPONENT_OUTPUT_ANALOG) {
            continue;
        }

        device->name = "Analog Output";
        device->level = 0;
        device->driver.channel = pwm_channel++;

        analogOutput_initGpio(device);

        ESP_LOGI(TAG, "Analog Output created with id %d on pin %d", device->id, device->gpio_pin);
    }
}

#endif
