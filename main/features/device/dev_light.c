#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "dev_light.h"
#include "device_types.h"
#include "device_manager.h"

static const char *TAG = "Feat_Light";

static void lightDevice_initGpio(device_t *device)
{
    ledc_channel_config_t channel = {
        .gpio_num = device->gpio_pin,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = device->pwm_channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&channel);
    /*gpio_reset_pin(device->gpio_pin);
    gpio_set_direction(device->gpio_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(device->gpio_pin, 0);*/
}

static void lightDevice_initPwm(void)
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

void lightDevice_init(void)
{
#if CONFIG_FEATURE_DEVICE_LIGHT
    lightDevice_initPwm();
    
    for (int i = 0; i < CONFIG_FEATURE_DEVICE_LIGHT_COUNT; i++) {
        device_t *device = deviceManager_getId(i);

        if (device == NULL) {
            continue;
        }

        device->id = i;
        device->name = "Light";
        device->type = DEVICE_TYPE_LIGHT;
        device->gpio_pin = CONFIG_FEATURE_DEVICE_LIGHT_STARTIO + i;
        device->is_on = false;
        device->brightness = 0;
        device->pwm_channel = i;

        lightDevice_initGpio(device);

        ESP_LOGI(TAG, "Light created with id %d on pin %d PWM channel %d",
                 i, device->gpio_pin, device->pwm_channel);
    }
#endif
}