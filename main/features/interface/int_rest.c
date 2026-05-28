#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_http_server.h"

#include "int_rest.h"
#include "sdkconfig.h"
#include "io_control.h"
#include "device_manager.h"
#include "aut_time.h"
#include "sys_time.h"

#if CONFIG_FEATURE_INTERFACE_WEBUI
#include "int_webui.h"
#endif

static const char *TAG = "Feat_Rest";

static httpd_handle_t server = NULL;

static esp_err_t restHandler_help(httpd_req_t *req)
{
    const char *html =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>ESP32 SmartHome REST Help</title>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "</head>"
        "<body>"
        "<h1>ESP32 SmartHome REST API</h1>"

        "<h2>Device Control</h2>"
        "<ul>"
        "<li><a href=\"/list\">/list</a> - List all devices</li>"
        "<li>/on?id=0 - Turn device on</li>"
        "<li>/off?id=0 - Turn device off</li>"
        "<li>/bright?id=0&value=80 - Set brightness</li>"
        "</ul>"

        "<h2>Time</h2>"
        "<ul>"
        "<li><a href=\"/time/get\">/time/get</a> - Show current system time</li>"
        "<li>/time/set?hour=18&minute=30&second=0 - Set system time</li>"
        "</ul>"

        "<h2>Schedules</h2>"
        "<ul>"
        "<li><a href=\"/schedule/list\">/schedule/list</a> - List time rules</li>"
        "<li>/schedule/add?hour=18&minute=30&id=0&state=1 - Add time rule</li>"
        "<li>/schedule/remove?index=0 - Remove time rule</li>"
        "</ul>"

        "</body>"
        "</html>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t restHandler_on(httpd_req_t *req)
{
    char query[64];
    char id_str[16];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK &&
        httpd_query_key_value(query, "id", id_str, sizeof(id_str)) == ESP_OK) {

        int id = atoi(id_str);
        device_t *dev = deviceManager_getId(id);

        if (dev) {
            ioControl_result_t result = ioControl_setDevice(dev->id, true);
            if (result == IOCONTROL_ERRORNOTFOUND) {
                httpd_resp_sendstr(req, "Error: Device not found");
                return ESP_OK;
            }

            if (result == IOCONTROL_ERRORNOTSWITCHABLE) {
                httpd_resp_sendstr(req, "Error: Device is not switchable");
                return ESP_OK;
            }

            httpd_resp_sendstr(req, "OK: device on");
            return ESP_OK;
        }
    }

    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid device id");
    return ESP_FAIL;
}

static esp_err_t restHandler_off(httpd_req_t *req)
{
    char query[64];
    char id_str[16];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK &&
        httpd_query_key_value(query, "id", id_str, sizeof(id_str)) == ESP_OK) {

        int id = atoi(id_str);
        device_t *dev = deviceManager_getId(id);

        if (dev) {
            ioControl_result_t result = ioControl_setDevice(dev->id, false);
            if (result == IOCONTROL_ERRORNOTFOUND) {
                httpd_resp_sendstr(req, "Error: Device not found");
                return ESP_OK;
            }

            if (result == IOCONTROL_ERRORNOTSWITCHABLE) {
                httpd_resp_sendstr(req, "Error: Device is not switchable");
                return ESP_OK;
            }
            httpd_resp_sendstr(req, "OK: device off");
            return ESP_OK;
        }
    }

    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid device id");
    return ESP_FAIL;
}

static esp_err_t restHandler_bright(httpd_req_t *req)
{
    char query[64];
    char id_str[16];
    char value_str[16];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK &&
        httpd_query_key_value(query, "id", id_str, sizeof(id_str)) == ESP_OK &&
        httpd_query_key_value(query, "value", value_str, sizeof(value_str)) == ESP_OK) {

        int id = atoi(id_str);
        int value = atoi(value_str);

        device_t *dev = deviceManager_getId(id);

        if (dev) {
            ioControl_setBrightness(dev->id, value);
            httpd_resp_sendstr(req, "OK: brightness set");
            return ESP_OK;
        }
    }

    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Error: Invalid parameters");
    return ESP_FAIL;
}

static esp_err_t restHandler_list(httpd_req_t *req)
{
    char response[512];
    int offset = 0;

    offset += snprintf(response + offset, sizeof(response) - offset, "Devices:<br>\n");

    int count = deviceManager_getCount();

    for (int i = 0; i < count; i++) {
        device_t *dev = deviceManager_getId(i);

        if (dev) {
            offset += snprintf(response + offset, sizeof(response) - offset, "ID:%d Name:%s On:%d Bright:%d<br>\n",
                               dev->id, dev->name, dev->is_on, dev->brightness);
        }
    }

    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t restHandler_scheduleList(httpd_req_t *req)
{
    char response[512];
    int offset = 0;

    offset += snprintf(response + offset, sizeof(response) - offset, "Time Rules:<br>\n");

    int count = automationTimebased_getRuleCount();

    for (int i = 0; i < count; i++) {
        timeRule_t *rule = automationTimebased_getRule(i);

        if (rule == NULL) {
            continue;
        }

        if (!rule->active) {
            continue;
        }
        offset += snprintf(response + offset, sizeof(response) - offset, "Rule %d -> %02d:%02d Device:%d State:%d<br>\n",
                           i, rule->hour, rule->minute, rule->device_id, rule->turn_on);
    }

    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t restHandler_scheduleAdd(httpd_req_t *req)
{
    char query[128];

    char hour_str[16];
    char minute_str[16];
    char id_str[16];
    char state_str[16];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK &&
        httpd_query_key_value(query, "hour", hour_str, sizeof(hour_str)) == ESP_OK &&
        httpd_query_key_value(query, "minute", minute_str, sizeof(minute_str)) == ESP_OK &&
        httpd_query_key_value(query, "id", id_str, sizeof(id_str)) == ESP_OK &&
        httpd_query_key_value(query, "state", state_str, sizeof(state_str)) == ESP_OK) {

        int hour = atoi(hour_str);
        int minute = atoi(minute_str);
        int id = atoi(id_str);
        bool state = atoi(state_str);

        timeRule_addResult_t result = automationTimebased_addRule(hour, minute, id, state);

        if (result == TIMERULE_ADD_ERRORINVALIDTIME) {
            httpd_resp_sendstr(req, "Error: invalid time");
            return ESP_OK;
        }

        if (result == TIMERULE_ADD_ERRORINVALIDDEVICE) {
            httpd_resp_sendstr(req, "Error: device not found");
            return ESP_OK;
        }

        if (result == TIMERULE_ADD_ERRORNOTSWITCHABLE) {
            httpd_resp_sendstr(req, "Error: device is not switchable");
            return ESP_OK;
        }

        if (result == TIMERULE_ADD_ERRORNOSLOT) {
            httpd_resp_sendstr(req, "Error: no free timerule slot");
            return ESP_OK;
        }

        httpd_resp_sendstr(req, "OK: timerule created");
        return ESP_OK;
    }

    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Error: Invalid parameters");
    return ESP_FAIL;
}

static esp_err_t restHandler_scheduleRemove(httpd_req_t *req)
{
    char query[64];
    char index_str[16];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK &&
        httpd_query_key_value(query, "index", index_str, sizeof(index_str)) == ESP_OK) {

        int index = atoi(index_str);

        timeRule_removeResult_t result = automationTimebased_removeRule(index);

        if (result == TIMERULE_REMOVE_ERRORINVALIDINDEX) {
            httpd_resp_sendstr(req, "Error: invalid rule index");
            return ESP_OK;
        }

        if (result == TIMERULE_REMOVE_ERRORNOTACTIVE) {
            httpd_resp_sendstr(req, "Error: rule not active");
            return ESP_OK;
        }

        httpd_resp_sendstr(req, "OK: rule removed");
        return ESP_OK;
    }

    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Error: Invalid index");
    return ESP_FAIL;
}

static esp_err_t restHandler_timeSet(httpd_req_t *req)
{
    char query[128];

    char hour_str[16];
    char minute_str[16];
    char second_str[16];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK &&
        httpd_query_key_value(query, "hour", hour_str, sizeof(hour_str)) == ESP_OK &&
        httpd_query_key_value(query, "minute", minute_str, sizeof(minute_str)) == ESP_OK &&
        httpd_query_key_value(query, "second", second_str, sizeof(second_str)) == ESP_OK) {

        int hour = atoi(hour_str);
        int minute = atoi(minute_str);
        int second = atoi(second_str);

        if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {

            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid time range");
            return ESP_FAIL;
        }

        systemTime_set(hour, minute, second);

        httpd_resp_sendstr(req, "OK: time set");
        return ESP_OK;
    }

    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid time parameters");
    return ESP_FAIL;
}

static esp_err_t restHandler_timeGet(httpd_req_t *req)
{
    char response[64];

    systemTime_t time = systemTime_get();

    snprintf(response, sizeof(response), "%02d:%02d:%02d", time.hour, time.minute, time.second);

    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t restHandler_favicon(httpd_req_t *req)
{
    httpd_resp_set_status(req, "204 No Content");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

void restInterface_init(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    #if CONFIG_FEATURE_INTERFACE_WEBUI
        config.max_uri_handlers = 32;
    #else
        config.max_uri_handlers = 16;
    #endif

    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start REST interface");
        return;
    }

    httpd_uri_t help_uri = {
        .uri = "/help",
        .method = HTTP_GET,
        .handler = restHandler_help,
        .user_ctx = NULL
    };

    httpd_uri_t on_uri = {
        .uri = "/on",
        .method = HTTP_GET,
        .handler = restHandler_on,
        .user_ctx = NULL
    };

    httpd_uri_t off_uri = {
        .uri = "/off",
        .method = HTTP_GET,
        .handler = restHandler_off,
        .user_ctx = NULL
    };

    httpd_uri_t bright_uri = {
        .uri = "/bright",
        .method = HTTP_GET,
        .handler = restHandler_bright,
        .user_ctx = NULL
    };

    httpd_uri_t list_uri = {
        .uri = "/list",
        .method = HTTP_GET,
        .handler = restHandler_list,
        .user_ctx = NULL
    };

    httpd_uri_t schedule_list_uri = {
        .uri = "/schedule/list",
        .method = HTTP_GET,
        .handler = restHandler_scheduleList,
        .user_ctx = NULL
    };

    httpd_uri_t schedule_add_uri = {
        .uri = "/schedule/add",
        .method = HTTP_GET,
        .handler = restHandler_scheduleAdd,
        .user_ctx = NULL
    };

    httpd_uri_t schedule_remove_uri = {
        .uri = "/schedule/remove",
        .method = HTTP_GET,
        .handler = restHandler_scheduleRemove,
        .user_ctx = NULL
    };

    httpd_uri_t time_set_uri = {
        .uri = "/time/set",
        .method = HTTP_GET,
        .handler = restHandler_timeSet,
        .user_ctx = NULL
    };

    httpd_uri_t time_get_uri = {
        .uri = "/time/get",
        .method = HTTP_GET,
        .handler = restHandler_timeGet,
        .user_ctx = NULL
    };

    httpd_uri_t favicon_uri = {
        .uri = "/favicon.ico",
        .method = HTTP_GET,
        .handler = restHandler_favicon,
        .user_ctx = NULL
    };

    httpd_register_uri_handler(server, &help_uri);
    httpd_register_uri_handler(server, &on_uri);
    httpd_register_uri_handler(server, &off_uri);
    httpd_register_uri_handler(server, &bright_uri);
    httpd_register_uri_handler(server, &list_uri);
    httpd_register_uri_handler(server, &schedule_list_uri);
    httpd_register_uri_handler(server, &schedule_add_uri);
    httpd_register_uri_handler(server, &schedule_remove_uri);
    httpd_register_uri_handler(server, &time_set_uri);
    httpd_register_uri_handler(server, &time_get_uri);
    httpd_register_uri_handler(server, &favicon_uri);

    ESP_LOGI(TAG, "REST interface started");

    #if CONFIG_FEATURE_INTERFACE_WEBUI
        webuiInterface_init(server);
    #endif

}