# M5PaperS3 Dashboard - Technical Documentation

## Project Overview

This project implements a modern dashboard controller for the M5PaperS3 board that provides a web-based interface for controlling up to 4 M5SwitchC6 devices via MQTT protocol.

## Architecture

### System Components

```
┌─────────────────────────────────────────────────────────────────┐
│                    M5PaperS3 Board                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐         │
│  │   WiFi       │  │   MQTT       │  │  Web Server  │         │
│  │   Module     │  │   Client     │  │              │         │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘         │
│         │                  │                  │                  │
│  ┌──────┴──────────────────┴──────────────────┴───────┐        │
│  │           M5SwitchC6 Controller                     │        │
│  │           (Serial Communication)                    │        │
│  └──────────────────────┬──────────────────────────────┘        │
│                         │                                        │
│  ┌──────────────────────┴──────────────────────────────┐        │
│  │              E-Paper Display Module                 │        │
│  │         (Real-time Status Visualization)            │        │
│  └─────────────────────────────────────────────────────┘        │
└─────────────────────────┼────────────────────────────────────────┘
                          │
         ┌────────────────┼────────────────┐
         │                │                │
    ┌────▼───┐      ┌────▼───┐      ┌────▼───┐
    │ Switch │      │ Switch │      │ Switch │ ...
    │   C6   │      │   C6   │      │   C6   │
    │ Device │      │ Device │      │ Device │
    └────────┘      └────────┘      └────────┘
```

### Communication Flow

1. **User → Web Interface**: User accesses the web interface via browser
2. **Web Interface → M5PaperS3**: HTTP requests sent to web server
3. **M5PaperS3 → M5SwitchC6**: Commands sent via Serial (RX pin 16)
4. **M5SwitchC6 → Device**: Commands transmitted to physical devices
5. **Device → M5SwitchC6**: Status responses returned
6. **M5SwitchC6 → M5PaperS3**: Responses forwarded via Serial
7. **M5PaperS3 → E-Paper Display**: Status displayed on screen (immediate update on state change)
8. **M5PaperS3 → MQTT Broker**: Status updates published (optional)

## File Structure

```
M5PaperS3_Dashboard/
├── M5PaperS3_Dashboard.ino    # Main sketch file
├── config.h                    # Configuration header
├── README.md                   # User documentation
├── DOCUMENTATION.md            # This file
├── platformio.ini             # PlatformIO configuration
└── examples/
    └── SimpleTest/
        └── SimpleTest.ino     # Basic test sketch
```

## Core Functions

### Initialization Functions

#### `setup()`
Main initialization function that:
- Initializes M5 hardware
- Starts serial communication
- Initializes M5SwitchC6 library
- Connects to WiFi
- Sets up MQTT connection
- Starts web server
- Queries initial device states

#### `setupWiFi()`
Handles WiFi connection:
- Configures WiFi in station mode
- Attempts connection with retry logic
- Updates connection status

#### `setupMQTT()`
Configures MQTT client:
- Sets MQTT server and port
- Registers callback function

#### `setupWebServer()`
Initializes web server:
- Registers route handlers
- Starts HTTP server on port 80

#### `setupDisplay()`
Initializes e-paper display:
- Sets display rotation
- Clears screen with white background
- Sets text color to black
- Shows initialization message

### Device Control Functions

#### `turnDeviceOn(int deviceIndex)`
Turns a specific device ON:
- Parameter: Device index (0-3)
- Returns: Success status (boolean)
- Calls: `switchC6.sendSwitchCommandWithResponse(MAC, true, 10000)`

#### `turnDeviceOff(int deviceIndex)`
Turns a specific device OFF:
- Parameter: Device index (0-3)
- Returns: Success status (boolean)
- Calls: `switchC6.sendSwitchCommandWithResponse(MAC, false, 10000)`

#### `queryDeviceStatus(int deviceIndex)`
Queries current device state:
- Parameter: Device index (0-3)
- Returns: Success status (boolean)
- Updates: `deviceStates[deviceIndex]`
- Calls: `switchC6.sendStatusQueryWithResponse(MAC, 10000, &response)`

#### `updateAllDeviceStatuses()`
Updates status for all devices:
- Loops through all 4 devices
- Calls `queryDeviceStatus()` for each
- Adds delay between queries

### Web Server Functions

#### `handleRoot()`
Serves main dashboard page:
- Generates HTML interface
- Includes current device states
- Shows WiFi/MQTT status

#### `handleToggle()`
Handles device control requests:
- Accepts GET parameters: `device` and `state`
- Calls appropriate turn on/off function
- Redirects back to main page

#### `handleQuery()`
Handles status query requests:
- Accepts GET parameter: `device`
- Queries device status
- Redirects back to main page

#### `handleStatusAPI()`
Returns JSON status data:
- Returns all device states
- Includes WiFi/MQTT status
- Used for programmatic access

#### `generateWebPage()`
Generates HTML for dashboard:
- Creates responsive interface
- Includes CSS styling
- Generates device cards
- Adds control buttons

### Display Functions

#### `displayStatus()`
Updates e-paper display with current status:
- Shows dashboard title
- Displays WiFi and MQTT connection status
- Shows device IP address
- Lists all 4 devices with ON/OFF status
- Displays MAC addresses for each device
- Shows last update timestamp
- Called immediately after state changes for minimal delay

#### `setupDisplay()`
Initializes the display hardware:
- Configures display rotation
- Sets up text rendering
- Shows initialization screen

### Main Loop Function

#### `loop()`
Main execution loop:
- Handles web server requests
- Maintains MQTT connection
- Performs periodic status updates
- Checks WiFi connection
- Updates display every 2 seconds
- Updates M5 device state

## Data Structures

### Device Arrays

```cpp
String DEVICE_MACS[4]       // MAC addresses of devices
bool deviceStates[4]        // Current on/off states
String deviceNames[4]       // Display names for devices
```

### Status Variables

```cpp
bool wifiConnected          // WiFi connection status
bool mqttEnabled            // MQTT enabled flag
bool mqttConnected          // MQTT connection status
unsigned long lastStatusUpdate  // Last status check timestamp
unsigned long lastDisplayUpdate // Last display update timestamp
```

## Web Interface Design

### Dashboard Features

1. **Status Bar**
   - WiFi connection indicator
   - MQTT connection indicator
   - Current IP address

2. **Device Cards** (4 cards)
   - Device name
   - Current state (ON/OFF badge)
   - ON button
   - OFF button
   - Query button
   - MAC address display

3. **Refresh Button**
   - Reloads entire page
   - Updates all device states

### Styling

- Modern gradient background
- Card-based layout
- Responsive grid design
- Hover effects on buttons
- Color-coded status indicators
- Mobile-friendly interface

## API Reference

### M5SwitchC6 Library Calls

#### Send Switch Command
```cpp
bool sendSwitchCommandWithResponse(String mac, bool state, unsigned long timeout)
```
- `mac`: Device MAC address (format: "B43A-4581-F070")
- `state`: true for ON, false for OFF
- `timeout`: Command timeout in milliseconds
- Returns: Success status

#### Query Status
```cpp
bool sendStatusQueryWithResponse(String mac, unsigned long timeout, SwitchResponse* response)
```
- `mac`: Device MAC address
- `timeout`: Query timeout in milliseconds
- `response`: Pointer to response structure
- Returns: Success status

### HTTP Endpoints

#### GET /
- Returns: HTML dashboard page

#### GET /toggle?device=X&state=Y
- `device`: Device index (0-3)
- `state`: 1 for ON, 0 for OFF
- Returns: 303 redirect to /

#### GET /query?device=X
- `device`: Device index (0-3)
- Returns: 303 redirect to /

#### GET /api/status
- Returns: JSON with device and connection status

## Configuration

### WiFi Settings
Modify in sketch or config.h:
```cpp
const char* WIFI_SSID = "CJAK2";
const char* WIFI_PASSWORD = "CJak55998718";
```

### MQTT Settings
Modify in sketch or config.h:
```cpp
const char* MQTT_SERVER = "192.168.0.82";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "admin";
const char* MQTT_PASSWORD = "$Orange111";
```

### Device Configuration
Update MAC addresses to match your devices:
```cpp
const String DEVICE_MACS[4] = {
  "B43A-4581-F070",  // Your actual MAC
  "B43A-4581-F071",  // Your actual MAC
  "B43A-4581-F072",  // Your actual MAC
  "B43A-4581-F073"   // Your actual MAC
};
```

## Error Handling

### WiFi Connection Failures
- Retry mechanism with timeout
- Status flag updated
- Error logged to Serial

### MQTT Connection Failures
- Automatic reconnection in loop
- Connection state tracked
- Error codes logged

### Device Control Failures
- Timeout after 10 seconds
- Error logged to Serial
- State not updated on failure

## Serial Output

All operations are logged to Serial at 115200 baud:
- Initialization status
- WiFi connection progress
- MQTT connection status
- Device control commands
- Query results
- Error messages

## Performance Considerations

### Timing
- 10ms delay in main loop
- 100ms delay between device queries
- 2 second interval for display updates
- 5 second interval for periodic status updates
- 10 second timeout for device commands
- Immediate display update on state changes

### Memory
- Minimal string allocations
- Static arrays for device data
- HTML generated on-demand

### Network
- Single web server instance
- Persistent MQTT connection
- Automatic WiFi reconnection

## Security Considerations

### Current Implementation
- Hardcoded credentials (suitable for private networks)
- No authentication on web interface
- Unencrypted HTTP
- Unencrypted MQTT

### Recommendations for Production
1. Use HTTPS for web interface
2. Implement web authentication
3. Use MQTTS (MQTT over TLS)
4. Store credentials securely
5. Implement API rate limiting

## Troubleshooting

### Common Issues

1. **WiFi not connecting**
   - Check SSID and password
   - Verify 2.4GHz network
   - Check signal strength

2. **MQTT not connecting**
   - Verify broker address
   - Check credentials
   - Ensure broker is running

3. **Devices not responding**
   - Verify MAC addresses
   - Check RX pin connection
   - Ensure devices are powered

4. **Web interface not accessible**
   - Check Serial for IP address
   - Verify same network
   - Check firewall settings

## Future Enhancements

Potential improvements:
- WebSocket for real-time updates
- Device scheduling/timers
- Usage statistics and graphs
- Multi-language support
- Over-the-air (OTA) updates
- Mobile app integration
- Scene/automation support
- Energy monitoring

## License

This project is provided as-is for use with M5Stack products.

## Version History

- v1.0.0 - Initial release with 4-device support
