#include "esp_log.h"
#include "esp_http_server.h"

#include "int_webUI.h"

static const char *TAG = "Int_WebUI";

static esp_err_t webuiHandler_getRoot(httpd_req_t *req)
{
    const char *html =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
            "<title>ESP32 SmartHome</title>"
            "<link rel='stylesheet' href='/webui/style.css'>"
        "</head>"
        "<body>"

            "<div class='container'>"
                "<div id='menu'></div>"

                "<div class='content'>"
                    "<div class='box'>"
                        "<h1 id='title'>ESP32 SmartHome</h1>"
                        "<div id='content'>Please select in menu.</div>"
                    "</div>"
                "</div>"
            "</div>"

            "<script src='/webui/script.js'></script>"
        "</body></html>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t webuiHandler_getMenu(httpd_req_t *req)
{
    const char *html =
        "<div class='menu'>"
        "<h2>Menu</h2>"
        "<button onclick=\"loadPage('/webui/status','Status')\">Status</button>"
        "<button onclick=\"loadPage('/webui/devices','Device')\">Device</button>"
        "<button onclick=\"loadPage('/webui/time','Timebased')\">Timebased</button>"
        "<button onclick=\"loadPage('/webui/events','Eventbased')\">Eventbased</button>"
        "</div>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t webuiHandler_getStatus(httpd_req_t *req)
{
    const char *html =
        "<div class='status-card'>"
            "<h2>Systemzeit</h2>"
            "<div id='clock' class='clock'>--:--:--</div>"
        "</div>"

        "<div class='status-card'>"
            "<h3>Zeit setzen</h3>"

            "<div class='form-row'>"
                "<input id='hour' type='number' min='0' max='23' placeholder='Stunde'>"
                "<input id='minute' type='number' min='0' max='59' placeholder='Minute'>"
                "<input id='second' type='number' min='0' max='59' placeholder='Sekunde'>"
            "</div>"

            "<button class='action' onclick='setTime()'>Set time</button>"
            "<pre id='result' class='result'></pre>"
        "</div>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t webuiHandler_getDevices(httpd_req_t *req)
{
    const char *html =

        "<div class='device-card'>"
            "<h2>Devices</h2>"
            "<div id='deviceList'></div>"
        "</div>"

        "<div class='device-card'>"
            "<h3>Device steuern</h3>"
            "<div class='form-row'>"
                "<input id='devId' type='number' min='0' placeholder='Device ID'>"
            "</div>"
            "<button class='action' onclick='deviceOn()'>ON</button>"
            "<button class='action' onclick='deviceOff()'>OFF</button>"

            "<h3>Brightness</h3>"
            "<div class='form-row'>"
                "<input id='brightValue' type='number' min='0' max='100' placeholder='Brightness'>"
            "</div>"
            "<button class='action' onclick='setBrightness()'>Set Brightness</button>"

            "<pre id='result' class='result'></pre>"
        "</div>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t webuiHandler_getTimebased(httpd_req_t *req)
{
    const char *html =
        "<div class='status-card'>"
            "<h2>Timebased Automation</h2>"
            "<div id='scheduleList'></div>"
        "</div>"

        "<div class='status-card'>"
            "<h3>Add rule</h3>"
            "<div class='form-row'>"
                "<input id='ruleHour' type='number' min='0' max='23' placeholder='Stunde'>"
                "<input id='ruleMinute' type='number' min='0' max='59' placeholder='Minute'>"
                "<input id='ruleDeviceId' type='number' min='0' placeholder='Device ID'><br><br>"
                "<select id='ruleState'>"
                    "<option value='1'>ON</option>"
                    "<option value='0'>OFF</option>"
                "</select>"
            "</div>"
            "<button class='action' onclick='addSchedule()'>Add rule</button>"
        "</div>"

        "<div class='status-card'>"
            "<h3>Remove rule</h3>"
            "<div class='form-row'>"
                "<input id='ruleIndex' type='number' min='0' placeholder='Index'>"
            "</div>"
            "<button class='action' onclick='removeSchedule()'>Remove rule</button>"
            "<pre id='result' class='result'></pre>"
        "</div>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t webuiHandler_getEventbased(httpd_req_t *req)
{
    const char *html =
        "<div class='status-card'>"
            "<h2>Eventbased Automation</h2>"
            "<div id='eventRuleList'></div>"
        "</div>"

        "<div class='status-card'>"
            "<h3>Eventregel hinzufuegen</h3>"
            "<div class='form-row'>"
                "<input id='eventSensorId' type='number' min='0' placeholder='Sensor ID'>"
                "<input id='eventTargetId' type='number' min='0' placeholder='Target ID'><br><br>"
                "<select id='eventTriggerState'>"
                    "<option value='1'>Trigger ON</option>"
                    "<option value='0'>Trigger OFF</option>"
                "</select>"
                "<select id='eventTargetState'>"
                    "<option value='1'>Target ON</option>"
                    "<option value='0'>Target OFF</option>"
                "</select>"
            "</div>"
            "<button class='action' onclick='addEventRule()'>Add evenrule</button>"
        "</div>"

        "<div class='status-card'>"
            "<h3>Eventregel entfernen</h3>"
            "<div class='form-row'>"
                "<input id='eventRuleIndex' type='number' min='0' placeholder='Index'>"
            "</div>"
            "<button class='action' onclick='removeEventRule()'>Remove eventrule</button>"
            "<pre id='result' class='result'></pre>"
        "</div>";

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t webuiHandler_getCss(httpd_req_t *req)
{
    const char *css =
        //general
        "body{margin:0;font-family:Arial;background:#222;}"
        ".container{display:flex;height:100vh;}"
        ".content{flex:1;padding:25px;box-sizing:border-box;}"
        ".box{background:white;padding:25px;border-radius:10px;box-shadow:0 0 8px #ccc;}"

        //menu
        ".menu{width:220px;background:#222;color:white;padding:20px;box-sizing:border-box;}"
        ".menu button{width:100%;padding:12px;margin:8px 0;border:none;border-radius:6px;background:#444;color:white;cursor:pointer;font-size:15px;text-align:left;}"
        ".menu button:hover{background:#666;}"

        //status
        ".status-card{background:#f7f7f7;border:1px solid #ddd;border-radius:10px;padding:20px;margin-bottom:20px;}"
        ".clock{font-size:42px;font-weight:bold;background:#222;color:white;padding:20px;border-radius:10px;text-align:center;margin:15px 0;letter-spacing:2px;}"
        ".form-row{margin:15px 0;}"
        ".form-row input{width:90px;padding:10px;border:1px solid #ccc;border-radius:6px;font-size:15px;}"
        ".result{background:#eee;border-radius:8px;padding:12px;margin-top:15px;}"

        //devices
        ".device-card{background:#f7f7f7;border:1px solid #ddd;border-radius:10px;padding:20px;margin-bottom:20px;}"
        ".device-entry{background:white;border:1px solid #ccc;border-radius:8px;padding:12px;margin:10px 0;}";

    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, css, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t webuiHandler_getScript(httpd_req_t *req)
{
    const char *js =
        //load menu
        "fetch('/webui/menu').then(r=>r.text()).then(t=>document.getElementById('menu').innerHTML=t);"

        //load newpage
        "function loadPage(url,title){"
            "document.getElementById('title').innerText=title;"
            "fetch(url).then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('content').innerHTML=t;"

                "if(url=='/webui/status'){"
                    "startClock();"
                "}"

                "if(url=='/webui/devices'){"
                    "loadDevices();"
                "}"

                "if(url=='/webui/time'){"
                    "loadSchedules();"
                "}"

                "if(url=='/webui/events'){"
                    "loadEventRules();"
                "}"
            "});"
        "}"

        //status
        "let currentDate=null;"
        "let clockInterval=null;"

        "function startClock(){"

            "fetch('/time/get').then(r=>r.text()).then(t=>{"

                "currentDate=parseTime(t);"

                "updateClock();"

                "if(clockInterval){"
                    "clearInterval(clockInterval);"
                "}"

                "clockInterval=setInterval(()=>{"
                    "currentDate.setSeconds(currentDate.getSeconds()+1);"
                    "updateClock();"
                "},1000);"

                "setInterval(()=>{"
                    "fetch('/time/get').then(r=>r.text()).then(t=>{currentDate=parseTime(t);});"
                "},10000);"
            "});"
        "}"

        "function parseTime(text){"

            "let parts=text.trim().split(':');"

            "let d=new Date();"

            "d.setHours(parseInt(parts[0]));"
            "d.setMinutes(parseInt(parts[1]));"
            "d.setSeconds(parseInt(parts[2]));"

            "return d;"
        "}"

        "function updateClock(){"

            "let h=String(currentDate.getHours()).padStart(2,'0');"
            "let m=String(currentDate.getMinutes()).padStart(2,'0');"
            "let s=String(currentDate.getSeconds()).padStart(2,'0');"

            "document.getElementById('clock').innerText="
                "h+':'+m+':'+s;"
        "}"

        "function setTime(){"

            "let h=document.getElementById('hour').value;"
            "let m=document.getElementById('minute').value;"
            "let s=document.getElementById('second').value;"

            "fetch('/time/set?hour='+h+'&minute='+m+'&second='+s)"
            ".then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('result').innerText=t;"
                "startClock();"
            "});"
        "}"

        //devices
        "function loadDevices(){"
            "fetch('/list').then(r=>r.text())"
            ".then(t=>{"
                "t=t.replace('Devices:','');"
                "document.getElementById('deviceList').innerHTML=t;"
            "});"
        "}"

        "function deviceOn(){"

            "let id=document.getElementById('devId').value;"

            "fetch('/on?id='+id).then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('result').innerText=t;"
                "loadDevices();"
            "});"
        "}"

        "function deviceOff(){"

            "let id=document.getElementById('devId').value;"

            "fetch('/off?id='+id).then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('result').innerText=t;"
                "loadDevices();"
            "});"
        "}"

        "function setBrightness(){"

            "let id=document.getElementById('devId').value;"
            "let value=document.getElementById('brightValue').value;"

            "fetch('/bright?id='+id+'&value='+value).then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('result').innerText=t;"
                "loadDevices();"
            "});"
        "}"
        
        //timebased
        "function loadSchedules(){"
            "fetch('/schedule/list').then(r=>r.text())"
            ".then(t=>{"
            "t=t.replace('Time Rules:','');"
                "document.getElementById('scheduleList').innerHTML=t;"
            "});"
        "}"

        "function addSchedule(){"
            "let h=document.getElementById('ruleHour').value;"
            "let m=document.getElementById('ruleMinute').value;"
            "let id=document.getElementById('ruleDeviceId').value;"
            "let state=document.getElementById('ruleState').value;"

            "fetch('/schedule/add?hour='+h+'&minute='+m+'&id='+id+'&state='+state).then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('result').innerText=t;"
                "loadSchedules();"
            "});"
        "}"

        "function removeSchedule(){"
            "let index=document.getElementById('ruleIndex').value;"

            "fetch('/schedule/remove?index='+index).then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('result').innerText=t;"
                "loadSchedules();"
            "});"
        "}"
        

        //eventbased
        "function loadEventRules(){"
            "fetch('/event/list').then(r=>r.text())"
            ".then(t=>{"
                "t=t.replace('Eventrules:','');"
                "document.getElementById('eventRuleList').innerHTML=t;"
            "});"
        "}"

        "function addEventRule(){"
            "let sensor=document.getElementById('eventSensorId').value;"
            "let target=document.getElementById('eventTargetId').value;"
            "let trigger=document.getElementById('eventTriggerState').value;"
            "let state=document.getElementById('eventTargetState').value;"

            "fetch('/event/add?sensor='+sensor+'&target='+target+'&trigger='+trigger+'&state='+state)"
            ".then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('result').innerText=t;"
                "loadEventRules();"
            "});"
        "}"

        "function removeEventRule(){"
            "let index=document.getElementById('eventRuleIndex').value;"

            "fetch('/event/remove?index='+index).then(r=>r.text())"
            ".then(t=>{"
                "document.getElementById('result').innerText=t;"
                "loadEventRules();"
            "});"
        "}";

    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, js, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

void webuiInterface_init(httpd_handle_t server)
{
    if (server == NULL) {
        ESP_LOGE(TAG, "HTTP server is NULL");
        return;
    }

    httpd_uri_t root_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = webuiHandler_getRoot
    };

    httpd_uri_t menu_uri = {
        .uri = "/webui/menu",
        .method = HTTP_GET,
        .handler = webuiHandler_getMenu
    };

    httpd_uri_t status_uri = {
        .uri = "/webui/status",
        .method = HTTP_GET,
        .handler = webuiHandler_getStatus
    };

    httpd_uri_t devices_uri = {
        .uri = "/webui/devices",
        .method = HTTP_GET,
        .handler = webuiHandler_getDevices
    };

    httpd_uri_t timebased_uri = {
    .uri = "/webui/time",
    .method = HTTP_GET,
    .handler = webuiHandler_getTimebased
    };

    httpd_uri_t events_uri = {
        .uri = "/webui/events",
        .method = HTTP_GET,
        .handler = webuiHandler_getEventbased
    };

    httpd_uri_t css_uri = {
        .uri = "/webui/style.css",
        .method = HTTP_GET,
        .handler = webuiHandler_getCss
    };

    httpd_uri_t script_uri = {
        .uri = "/webui/script.js",
        .method = HTTP_GET,
        .handler = webuiHandler_getScript
    };

    httpd_register_uri_handler(server, &root_uri);
    httpd_register_uri_handler(server, &menu_uri);
    httpd_register_uri_handler(server, &status_uri);
    httpd_register_uri_handler(server, &devices_uri);
    httpd_register_uri_handler(server, &timebased_uri);
    httpd_register_uri_handler(server, &events_uri);
    httpd_register_uri_handler(server, &css_uri);
    httpd_register_uri_handler(server, &script_uri);

    ESP_LOGI(TAG, "WebUI interface started");
}