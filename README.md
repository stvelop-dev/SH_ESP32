
# ESP32Client

ESP32-based smart home client with MQTT communication, OTA firmware updates, and Home Assistant integration.

Gitlab
```
cd existing_repo
git remote add origin https://gitlab.com/homebase2025/esp32client.git
git branch -M main
git push -uf origin main
```

Github
```
cd existing_repo
git remote add github https://github.com/stvelop-dev/SH_ESP32.git
git push -u github main
```

## Architecture

### Core
- Application initialization
- Device Manager
- IO Control
- OTA Manager
- Feature Registry
- Event Handling

### Features

#### Interfaces
- CLI Interface
- MQTT Interface

#### Devices
- Light
- Relay
- Sensor

### Services
- WiFi Client

## Project Structure

```
main/
├── core/               # Core initialization and startup
├── services/           # WiFi
├── features/interface  # CLI, MQTT
└── features/device     # Lights, devices, sensors
```

## Requirements

- ESP-IDF v5.5.4 or higher
- ESP32 development board
- MQTT broker (e.g. Mosquitto)
- Optional: Home Assistant

## Configuration

Configure the project using `menuconfig`:

- WiFi SSID and password
- MQTT broker address
- MQTT device ID
- OTA server settings

Build and flash:

```bash
idf.py set-target esp32
idf.py menuconfig
idf.py build
idf.py flash monitor
```

## MQTT Topics

### Firmware

| Topic | Description |
|-------|-------------|
| `smarthome/<MQTT_Device_ID>/firmware/version` | Current firmware version |

### Devices

| Topic | Description |
|-------|-------------|
| `smarthome/<MQTT_Device_ID>/device/<deviceID>/set`    | Control a device |
| `smarthome/<MQTT_Device_ID>/device/<deviceID>/state`  | Device state |

Payloads:

```text
0 = OFF
1 = ON
```

## Home Assistant

The ESP32 automatically publishes MQTT Discovery configurations:

```text
homeassistant/sensor/<MQTT_Device_ID>_firmware/config
homeassistant/switch/<MQTT_Device_ID>_device_<deviceID>/config
```

## Changelog

### v0.1.2
- Added Home Assistant MQTT Discovery support
- Implemented device state publishing
- Improved MQTT topic handling
- Fixed local OTA update issue

### v0.1.1
- Added MQTT-based device control
- Extended Device Manager and IO Control modules

### v0.1.0
- Initial OTA implementation
- Added WiFi client service
- Added basic MQTT connectivity