#include "component_config.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "input_analog.h"
#include "component_types.h"
#include "device_manager.h"

static const char *TAG = "Feat_InAna";

static adc_oneshot_unit_handle_t adc1_handle = NULL;

static void analogInput_initAdcUnit(void)
{
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };

    adc_oneshot_new_unit(&init_config, &adc1_handle);
}

static bool analogInput_getAdcConfig(int gpio, adc_config_t *config)
{
    size_t count = componentConfig_getAdcCount();

    for (size_t i = 0; i < count; i++) {
        adc_config_t current;

        if (!componentConfig_getAdc(i, &current)) {
            ESP_LOGW(TAG, "ADC config get failed for index %u", (unsigned)i);
            continue;
        }

        if ((int)current.gpio == gpio) {
            *config = current;
            return true;
        }
    }

    return false;
}

static void analogInput_initAdcChannel(device_t *device)
{
    adc_config_t adc_config;

    if (!analogInput_getAdcConfig(device->gpio_pin, &adc_config)) {
        return;
    }

    device->driver.handle = adc1_handle;
    device->driver.channel = adc_config.channel;

    adc_oneshot_chan_cfg_t channel_config = {.atten = ADC_ATTEN_DB_12, .bitwidth = ADC_BITWIDTH_DEFAULT,};
    adc_oneshot_config_channel(adc1_handle, adc_config.channel, &channel_config);
}

void analogInput_init(void)
{
    if (componentConfig_getCountByType(COMPONENT_INPUT_ANALOG) == 0) {
        return;
    }

    analogInput_initAdcUnit();

    int count = deviceManager_getCount();

    for (int i = 0; i < count; i++) {
        device_t *device = deviceManager_getId(i);

        if (device == NULL) {
            continue;
        }

        if (device->type != COMPONENT_INPUT_ANALOG) {
            continue;
        }

        device->name = "Analog Input";
        device->level = 0;

        analogInput_initAdcChannel(device);
        device->level = deviceManager_getValue(device);

        ESP_LOGI(TAG, "Analog Input created with id %d on pin %d and channel %d", device->id, device->gpio_pin, device->driver.channel);
    }
}