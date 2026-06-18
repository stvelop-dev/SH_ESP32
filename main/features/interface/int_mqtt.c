#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "esp_app_desc.h"

#include "int_mqtt.h"
#include "update_ota.h"
#include "io_control.h"
#include "device_manager.h"

static const char *TAG = "Feat_MQTT";

static esp_mqtt_client_handle_t mqtt_client = NULL;

static void mqtt_publish_discovery_firmware(void)
{
    char topic[128];
    char payload[512];

    snprintf(topic, sizeof(topic),
             "homeassistant/sensor/%s_firmware/config",
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);

    snprintf(payload, sizeof(payload),
            "{"
                "\"name\":\"Firmware Version\","
                "\"unique_id\":\"%s_firmware_version\","
                "\"state_topic\":\"smarthome/%s/firmware/version\","
                "\"entity_category\":\"diagnostic\","
                "\"icon\":\"mdi:chip\","
                "\"device\":{"
                    "\"identifiers\":[\"%s\"],"
                    "\"name\":\"%s\","
                    "\"manufacturer\":\"Custom\","
                    "\"model\":\"ESP32 Smart Home\""
                "}"
            "}",
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);

    esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 1);
}

static void mqtt_publish_discovery_binaryOutput(device_t *device)
{
    char topic[128];
    char payload[512];

    snprintf(topic, sizeof(topic),
             "homeassistant/switch/%s_device_%d/config",
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
             device->id);

    snprintf(payload, sizeof(payload),
             "{"
                 "\"name\":\"%s GPIO%d\","
                 "\"unique_id\":\"%s_device_%d\","
                 "\"object_id\":\"%s_gpio%d\","
                 "\"command_topic\":\"smarthome/%s/device/%d/set\","
                 "\"state_topic\":\"smarthome/%s/device/%d/state\","
                 "\"payload_on\":\"1\","
                 "\"payload_off\":\"0\","
                 "\"device\":{"
                     "\"identifiers\":[\"%s\"],"
                     "\"name\":\"%s\","
                     "\"manufacturer\":\"Custom\","
                     "\"model\":\"ESP32 Smart Home\""
                 "}"
             "}",
            device->name,
            device->gpio_pin,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->gpio_pin,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);

    esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 1);
}

static void mqtt_publish_discovery_analogOutput(device_t *device)
{
    char topic[128];
    char payload[512];

    snprintf(topic, sizeof(topic),
             "homeassistant/number/%s_device_%d/config",
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
             device->id);

    snprintf(payload, sizeof(payload),
             "{"
                 "\"name\":\"%s GPIO%d\","
                 "\"unique_id\":\"%s_device_%d\","
                 "\"object_id\":\"%s_gpio%d\","
                 "\"command_topic\":\"smarthome/%s/device/%d/level/set\","
                 "\"state_topic\":\"smarthome/%s/device/%d/level/state\","
                 "\"min\":0,"
                 "\"max\":255,"
                 "\"step\":1,"
                 "\"device\":{"
                     "\"identifiers\":[\"%s\"],"
                     "\"name\":\"%s\","
                     "\"manufacturer\":\"Custom\","
                     "\"model\":\"ESP32 Smart Home\""
                 "}"
             "}",
            device->name,
            device->gpio_pin,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->gpio_pin,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);

    esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 1);
}

static void mqtt_publish_discovery_binaryInput(device_t *device)
{
    char topic[128];
    char payload[512];

    snprintf(topic, sizeof(topic),
             "homeassistant/binary_sensor/%s_device_%d/config",
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
             device->id);

    snprintf(payload, sizeof(payload),
             "{"
                 "\"name\":\"%s GPIO%d\","
                 "\"unique_id\":\"%s_device_%d\","
                 "\"object_id\":\"%s_gpio%d\","
                 "\"state_topic\":\"smarthome/%s/device/%d/state\","
                 "\"payload_on\":\"1\","
                 "\"payload_off\":\"0\","
                 "\"device\":{"
                     "\"identifiers\":[\"%s\"],"
                     "\"name\":\"%s\","
                     "\"manufacturer\":\"Custom\","
                     "\"model\":\"ESP32 Smart Home\""
                 "}"
             "}",
            device->name,
            device->gpio_pin,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->gpio_pin,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);

    esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 1);
}

static void mqtt_publish_discovery_analogInput(device_t *device)
{
    char topic[128];
    char payload[512];

    snprintf(topic, sizeof(topic),
             "homeassistant/sensor/%s_device_%d/config",
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
             device->id);

    snprintf(payload, sizeof(payload),
             "{"
                 "\"name\":\"%s GPIO%d\","
                 "\"unique_id\":\"%s_device_%d\","
                 "\"object_id\":\"%s_gpio%d\","
                 "\"state_topic\":\"smarthome/%s/device/%d/state\","
                 "\"unit_of_measurement\":\"raw\","
                 "\"device\":{"
                     "\"identifiers\":[\"%s\"],"
                     "\"name\":\"%s\","
                     "\"manufacturer\":\"Custom\","
                     "\"model\":\"ESP32 Smart Home\""
                 "}"
             "}",
            device->name,
            device->gpio_pin,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->gpio_pin,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, device->id,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
            CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);

    esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 1);
}

static void mqtt_publish_discovery_device(int id)
{
    device_t *device = deviceManager_getId(id);

    if (device == NULL) {
        return;
    }

    switch (device->type) {

    case COMPONENT_OUTPUT_BINARY:
        mqtt_publish_discovery_binaryOutput(device);
        break;

    case COMPONENT_OUTPUT_ANALOG:
        mqtt_publish_discovery_analogOutput(device);
        break;

    case COMPONENT_INPUT_BINARY:
        mqtt_publish_discovery_binaryInput(device);
        break;

    case COMPONENT_INPUT_ANALOG:
        mqtt_publish_discovery_analogInput(device);
        break;

    default:
        break;
    }
}

static void mqtt_publish_firmware_version(void)
{
    const esp_app_desc_t *app = esp_app_get_description();

    esp_mqtt_client_publish(mqtt_client,
        "smarthome/" CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID "/firmware/version",
        app->version, 0, 1, 1);
}

static void mqtt_publish_device_value(int id, const char *suffix, int value)
{
    char topic[128];
    char payload[32];

    snprintf(topic, sizeof(topic), "smarthome/%s/device/%d/%s", CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, id, suffix);

    snprintf(payload, sizeof(payload), "%d", value);

    esp_mqtt_client_publish( mqtt_client, topic, payload, 0, 1, 1);
}

static void mqtt_ota_task(void *param)
{
    char *url = (char *)param;

    ota_start(url);

    free(url);
    vTaskDelete(NULL);
}

static void mqttInterface_eventHandler(void *handler_args,esp_event_base_t base,int32_t event_id,void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
    {
        char topic[128];

        ESP_LOGI(TAG, "MQTT connected");

        snprintf(topic, sizeof(topic), "smarthome/%s/#", CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);
        esp_mqtt_client_subscribe(mqtt_client, topic, 0);

        mqtt_publish_discovery_firmware();
        mqtt_publish_firmware_version();

        for (int id = 0; id < ioControl_getDeviceCount(); id++) {
            mqtt_publish_discovery_device(id);

            int value = ioControl_getValue(id);

            if (ioControl_getType(id) == COMPONENT_OUTPUT_ANALOG) {
                mqtt_publish_device_value(id, "level/state", value);
            } else {
                mqtt_publish_device_value(id, "state", value);
            }
        }
        break;
    }

    case MQTT_EVENT_DISCONNECTED:
    {
        ESP_LOGW(TAG, "MQTT disconnected");
        break;
    }

    case MQTT_EVENT_DATA:
    {
        char topic[128];
        char data[64];

        char category[32];
        char command[32];
        char subcommand[32];

        int id = -1;

        snprintf(topic, sizeof(topic), "%.*s", event->topic_len, event->topic);
        snprintf(data, sizeof(data), "%.*s", event->data_len, event->data);

        ESP_LOGI(TAG, "Topic: %s", topic);
        ESP_LOGI(TAG, "Data: %s", data);

        if (strcmp(topic, "smarthome/" CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID "/ota/update") == 0)
        {
            if (ota_isRunning()) {
                ESP_LOGW(TAG, "OTA already running");
                break;
            }

            char *url = strdup(data);
            if (url == NULL) {
                ESP_LOGE(TAG, "Could not allocate OTA URL");
                break;
            }

            ESP_LOGI(TAG, "OTA update requested: %s", url);

            xTaskCreate(
                mqtt_ota_task,
                "mqtt_ota_task",
                8192,
                url,
                5,
                NULL
            );

            break;
        }

        if (sscanf(topic, "smarthome/%*[^/]/%31[^/]/%d/%31[^/]/%31[^/]", category, &id, command, subcommand) == 4)
        {
            if (!ioControl_exists(id)) {
                ESP_LOGW(TAG, "Unknown device id: %d", id);
                break;
            }

            if (strcmp(category, "device") == 0 && strcmp(command, "level") == 0 && strcmp(subcommand, "set") == 0)
            {
                int level = atoi(data);

                if (level < 0 || level > 255) {
                    ESP_LOGW(TAG, "Invalid level: %s", data);
                    break;
                }

                ioControl_result_t result = ioControl_setLevel(id, level);

                if (result != IOCONTROL_OK) {
                    ESP_LOGW(TAG, "Could not set device %d level, error %d", id, result);
                } else {
                    mqtt_publish_device_value(id, "level/state", level);
                }
            }

            break;
        }

        if (sscanf(topic, "smarthome/%*[^/]/%31[^/]/%d/%31[^/]", category, &id, command) == 3)
        {
            if (!ioControl_exists(id)) {
                ESP_LOGW(TAG, "Unknown device id: %d", id);
                break;
            }

            if (strcmp(category, "device") == 0 && strcmp(command, "set") == 0)
            {
                int state = atoi(data);

                if (state != 0 && state != 1) {
                    ESP_LOGW(TAG, "Invalid state: %s", data);
                    break;
                }

                ioControl_result_t result = ioControl_setState(id, state);

                if (result != IOCONTROL_OK) {
                    ESP_LOGW(TAG, "Could not set device %d, error %d", id, result);
                } else {
                    mqtt_publish_device_value(id, "state", state);
                }

                break;
            }

            if (strcmp(command, "get") == 0)
            {
                int value = ioControl_getValue(id);

                char response_topic[128];
                char response_data[32];

                snprintf(response_topic, sizeof(response_topic), "smarthome/%s/%s/%d/state", CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, category, id);

                snprintf(response_data, sizeof(response_data), "%d", value);

                esp_mqtt_client_publish(mqtt_client, response_topic, response_data, 0, 0, 0);

                break;
            }
        }

        break;
    }

    default:
        break;
    }
}

void mqttInterface_init(void)
{
#if CONFIG_FEATURE_INTERFACE_MQTT
    ESP_LOGI(TAG, "Initializing MQTT interface");

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_FEATURE_INTERFACE_MQTT_BROKERURI,
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);

    ESP_ERROR_CHECK(esp_mqtt_client_register_event(
        mqtt_client,
        ESP_EVENT_ANY_ID,
        mqttInterface_eventHandler,
        NULL
    ));

    ESP_ERROR_CHECK(esp_mqtt_client_start(mqtt_client));

    ESP_LOGI(TAG, "MQTT interface started");
#endif
}