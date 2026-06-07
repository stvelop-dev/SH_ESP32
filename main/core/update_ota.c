#include "update_ota.h"

#include "esp_log.h"
#include "esp_https_ota.h"
#include "esp_ota_ops.h"
#include "esp_system.h"

static const char *TAG = "Core_OTA";

static bool s_otaRunning = false;

esp_err_t ota_init(void)
{
    const esp_partition_t *running = esp_ota_get_running_partition();

    ESP_LOGI(TAG, "Running partition: %s at 0x%08" PRIx32,
             running->label,
             running->address);

    return ESP_OK;
}

bool ota_isRunning(void)
{
    return s_otaRunning;
}

esp_err_t ota_start(const char *url)
{
    if (url == NULL || url[0] == '\0') {
        ESP_LOGE(TAG, "OTA URL is empty");
        return ESP_ERR_INVALID_ARG;
    }

    if (s_otaRunning) {
        ESP_LOGW(TAG, "OTA already running");
        return ESP_ERR_INVALID_STATE;
    }

    s_otaRunning = true;

    ESP_LOGI(TAG, "Starting OTA from: %s", url);

    esp_http_client_config_t http_config = {
        .url = url,
        .timeout_ms = 10000,
        .keep_alive_enable = true,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    esp_err_t ret = esp_https_ota(&ota_config);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA successful, restarting...");
        esp_restart();
    } else {
        ESP_LOGE(TAG, "OTA failed: %s", esp_err_to_name(ret));
    }

    s_otaRunning = false;
    return ret;
}