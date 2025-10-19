# M5PaperS3 Dashboard Controller

A modern dashboard controller for the M5PaperS3 board that controls 4 M5SwitchC6 devices via MQTT with a web interface.

## Features

- **4 Toggle Switches**: Control 4 M5SwitchC6 devices independently
- **E-Paper Display**: Real-time status display on M5PaperS3 screen showing device states
- **Web Interface**: Modern, responsive web UI for remote control
- **WiFi Connectivity**: Automatic WiFi connection with reconnection support
- **MQTT Support**: MQTT-enabled by default for device communication
- **Status Monitoring**: Real-time status queries for all devices
- **RESTful API**: JSON API endpoint for programmatic access

## Hardware Requirements

- M5PaperS3 board
- Up to 4 M5SwitchC6 devices
- WiFi network
- MQTT broker (optional but enabled by default)

## Required Libraries

Install the following libraries via Arduino Library Manager or manually:

1. **M5Unified** - M5Stack unified library
2. **M5SwitchC6** - Library for M5SwitchC6 device communication
3. **WiFi** - ESP32 WiFi library (built-in)
4. **WebServer** - ESP32 web server library (built-in)
5. **PubSubClient** - MQTT client library

## Default Configuration

### WiFi Credentials
- **SSID**: `CJAK2`
- **Password**: `CJak55998718`

### MQTT Configuration
- **Server**: `192.168.0.82`
- **Port**: `1883`
- **Username**: `admin`
- **Password**: `$Orange111`
- **MQTT Enabled**: Yes (by default)

### Device Configuration

The sketch is configured for 4 M5SwitchC6 devices with the following MAC addresses:

1. **Device 1 (Switch 1)**: `B43A-4581-F070`
2. **Device 2 (Switch 2)**: `B43A-4581-F071`
3. **Device 3 (Switch 3)**: `B43A-4581-F072`
4. **Device 4 (Switch 4)**: `B43A-4581-F073`

**Note**: Update these MAC addresses in the sketch to match your actual M5SwitchC6 devices.

## Installation

1. Open the sketch in Arduino IDE
2. Select **M5Stack-PAPER** as the board (Tools > Board > ESP32 Arduino > M5Stack-PAPER)
3. Install required libraries
4. (Optional) Test the display first using `examples/DisplayTest/DisplayTest.ino`
5. Update device MAC addresses if needed
6. Upload to your M5PaperS3 board

## Usage

### E-Paper Display

The M5PaperS3 built-in e-paper display shows status information with **white text on black background** for optimal visibility and brightness, using **large fonts that fill the entire screen**:
- **Connection Status**: WiFi and MQTT connection indicators
- **IP Address**: Current device IP address
- **Device Status**: Real-time status for all 4 switches with **large graphical toggle indicators**
  - ON state: Toggle filled on the right side with highlight
  - OFF state: Toggle empty on the left side
  - **Touch-enabled**: Tap any toggle switch on the screen to control the device directly
- **MAC Addresses**: Each device's MAC address
- **Last Update Time**: Timestamp of the last status update

The display automatically updates every 2 seconds and immediately after any device state change, ensuring minimal delay between toggle actions and display updates. The graphical toggle switches provide clear visual feedback matching modern UI design patterns.

**Touch Controls**: Simply tap on any of the 4 large toggle switches displayed on the screen to turn devices on or off. No need to use the web interface - full control is available directly from the M5PaperS3 touchscreen!

### Web Interface

Once the device boots up and connects to WiFi, access the web interface at:

```
http://<device-ip-address>
```

The IP address will be displayed in the Serial Monitor and on the e-paper display.

#### Web Interface Features:

- **Status Bar**: Shows WiFi and MQTT connection status, plus device IP
- **Switch Cards**: Each of the 4 devices has a card with:
  - Device name and current state (ON/OFF)
  - ON button - Turn the device on
  - OFF button - Turn the device off
  - Query button - Query the current device status
  - MAC address display
- **Refresh Button**: Reload the page to see updated statuses

### API Endpoints

#### GET /
Returns the HTML dashboard interface

#### GET /toggle?device=X&state=Y
Toggle a device on or off
- `device`: Device index (0-3)
- `state`: 1 for ON, 0 for OFF

Example:
```
http://<device-ip>/toggle?device=0&state=1  # Turn device 0 ON
http://<device-ip>/toggle?device=2&state=0  # Turn device 2 OFF
```

#### GET /query?device=X
Query the status of a specific device
- `device`: Device index (0-3)

Example:
```
http://<device-ip>/query?device=1  # Query device 1 status
```

#### GET /api/status
Returns JSON with all device statuses

Example response:
```json
{
  "devices": [
    {
      "id": 0,
      "name": "Switch 1",
      "mac": "B43A-4581-F070",
      "state": true
    },
    ...
  ],
  "wifi": true,
  "mqtt": true
}
```

## M5SwitchC6 API Calls

The sketch uses the following M5SwitchC6 API calls:

### Turn Device ON
```cpp
switchC6.sendSwitchCommandWithResponse(DEVICE_MAC, true, 10000);
```

### Turn Device OFF
```cpp
switchC6.sendSwitchCommandWithResponse(DEVICE_MAC, false, 10000);
```

### Query Device Status
```cpp
M5SwitchC6::SwitchResponse response;
switchC6.sendStatusQueryWithResponse(DEVICE_MAC, 10000, &response);
```

## Serial Monitor Output

The sketch provides detailed logging via Serial Monitor at 115200 baud:

- WiFi connection status
- MQTT connection status
- Device control commands
- Status query results
- Error messages

## Customization

### Change WiFi Credentials

Edit the following lines in the sketch:
```cpp
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";
```

### Change MQTT Settings

Edit the following lines:
```cpp
const char* MQTT_SERVER = "YOUR_MQTT_SERVER";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "YOUR_USERNAME";
const char* MQTT_PASSWORD = "YOUR_PASSWORD";
```

### Disable MQTT

Set the following variable to `false`:
```cpp
bool mqttEnabled = false;
```

### Change Device Names

Edit the `deviceNames` array:
```cpp
String deviceNames[4] = {
  "Living Room",
  "Bedroom", 
  "Kitchen",
  "Garage"
};
```

### Change Device MAC Addresses

Edit the `DEVICE_MACS` array with your actual device MAC addresses:
```cpp
const String DEVICE_MACS[4] = {
  "YOUR-MAC-1",
  "YOUR-MAC-2",
  "YOUR-MAC-3",
  "YOUR-MAC-4"
};
```

## Troubleshooting

### WiFi Not Connecting
- Verify SSID and password are correct
- Check that the WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- Ensure the WiFi signal is strong enough

### MQTT Not Connecting
- Verify MQTT server IP address is correct
- Check MQTT username and password
- Ensure MQTT broker is running and accessible
- Verify firewall settings allow MQTT traffic (port 1883)

### Device Control Not Working
- Verify device MAC addresses are correct
- Ensure M5SwitchC6 devices are powered on
- Check that devices are in range
- Verify RX pin (16) is correctly connected

### Web Interface Not Loading
- Check Serial Monitor for the IP address
- Ensure you're on the same network as the M5PaperS3
- Try accessing via the IP address directly

### Display Not Working
- Run the `examples/DisplayTest/DisplayTest.ino` sketch to verify display functionality
- Check that M5Unified library is properly installed
- Verify board selection is M5Stack-PAPER
- E-paper displays have slower refresh rates - wait a few seconds for updates

## License

This project is provided as-is for use with M5Stack products.

## Support

For issues and questions:
- Check the Serial Monitor output for error messages
- Verify all library versions are up to date
- Ensure hardware connections are correct
