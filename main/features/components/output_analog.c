#include "component_config.h"

#include <stdint.h>
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
        .timer_sel = device->driver.timer,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&channel);
}

static void analogOutput_initPwm(device_t *device)
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = device->driver.timer,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = device->driver.frequency,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&timer);
}

void analogOutput_init(void)
{
    int pwm_channel = 0;
    int pwm_timer = 0;

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
        device->driver.timer = pwm_timer++;

        analogOutput_initPwm(device);
        analogOutput_initGpio(device);

        ESP_LOGI(TAG, "Analog Output created with id %d on pin %d", device->id, device->gpio_pin);
    }
}
