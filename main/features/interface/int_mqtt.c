#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_client.h"
#include "esp_log.h"

#include "int_mqtt.h"
#include "update_ota.h"
#include "io_control.h"

static const char *TAG = "Feat_MQTT";

static esp_mqtt_client_handle_t mqtt_client = NULL;

static void mqtt_publish_discovery_device(int id)
{
    char topic[128];
    char payload[512];

    snprintf(topic, sizeof(topic), "homeassistant/switch/%s_device_%d/config",
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, id);

    snprintf(payload, sizeof(payload), 
            "{"
                "\"name\":\"Device %d\","
                "\"unique_id\":\"%s_device_%d\","
                "\"object_id\":\"%s_device_%d\","
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
             id, 
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, id, 
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, id, 
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, id, 
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, id, 
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID,
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);

    esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 1);
}

static void mqtt_publish_device_state(int id, int state)
{
    char topic[128];
    char payload[8];

    snprintf(topic, sizeof(topic), "smarthome/%s/device/%d/state",
             CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, id);

    snprintf(payload, sizeof(payload), "%d", state ? 1 : 0);

    esp_mqtt_client_publish(mqtt_client, topic, payload, 0, 1, 1);
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
        char topic[128];

        ESP_LOGI(TAG, "MQTT connected");

        snprintf(topic, sizeof(topic), "smarthome/%s/#", CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID);
        esp_mqtt_client_subscribe(mqtt_client, topic, 0);

        for (int id = 0; id < ioControl_getDeviceCount(); id++) {
            mqtt_publish_discovery_device(id);
            mqtt_publish_device_state(id, ioControl_getValue(id));
        }
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "MQTT disconnected");
        break;

    case MQTT_EVENT_DATA:
    {
        char topic[128];
        char data[64];
        char category[32];
        char command[32];
        int id = -1;

        snprintf(topic, sizeof(topic), "%.*s", event->topic_len, event->topic);
        snprintf(data, sizeof(data), "%.*s", event->data_len, event->data);

        ESP_LOGI(TAG, "Topic: %s", topic);
        ESP_LOGI(TAG, "Data: %s", data);

        if (sscanf(topic, "smarthome/%*[^/]/%31[^/]/%d/%31[^/]", category, &id, command) == 3)
        {
            if (!ioControl_exists(id)) {
                ESP_LOGW(TAG, "Unknown device id: %d", id);
                break;
            }

            if (strcmp(category, "device") == 0 && strcmp(command, "set") == 0)
            {
                ioControl_result_t result;
                int state = atoi(data);

                if (state == 0 || state == 1) {
                    result = ioControl_setDevice(id, state);
                } else {
                    ESP_LOGW(TAG, "Invalid state: %s", data);
                    break;
                }

                if (result != IOCONTROL_OK) {
                    ESP_LOGW(TAG, "Could not set device %d, error %d", id, result);
                } else {
                    mqtt_publish_device_state(id, state);
                }
            }

            else if (strcmp(command, "get") == 0)
            {
                int value = ioControl_getValue(id);

                char response_topic[128];
                char response_data[32];

                const char *topic_suffix = strcmp(category, "device") == 0 ? "state" : "value";

                snprintf(response_topic, sizeof(response_topic), "smarthome/%s/%s/%d/%s",
                        CONFIG_FEATURE_INTERFACE_MQTT_DEVICEID, category, id, topic_suffix);

                snprintf(response_data, sizeof(response_data), "%d", value);

                esp_mqtt_client_publish(mqtt_client, response_topic, response_data, 0, 0, 0);
            }

            else if (strcmp(category, "ota") == 0 && strcmp(command, "update") == 0)
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