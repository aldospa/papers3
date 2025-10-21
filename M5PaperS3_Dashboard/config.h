/**
 * Configuration Header for M5PaperS3 Dashboard Controller
 * 
 * This file contains all configurable parameters for the dashboard.
 * Modify these values to customize the behavior of your dashboard.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// WiFi Configuration
// ============================================================================
#define WIFI_SSID "CJAK2"
#define WIFI_PASSWORD "CJak55998718"
#define WIFI_CONNECT_TIMEOUT 20  // Maximum connection attempts

// ============================================================================
// MQTT Configuration
// ============================================================================
#define MQTT_ENABLED true
#define MQTT_SERVER "192.168.0.82"
#define MQTT_PORT 1883
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "$Orange111"
#define MQTT_CLIENT_ID "M5PaperS3_Dashboard"

// ============================================================================
// M5SwitchC6 Configuration
// ============================================================================
#define SWITCH_RX_PIN 16
#define NUM_DEVICES 4
#define COMMAND_TIMEOUT 10000  // 10 seconds timeout for commands

// Device MAC addresses
#define DEVICE1_MAC "B43A-4581-F070"
#define DEVICE2_MAC "B43A-4581-F071"
#define DEVICE3_MAC "B43A-4581-F072"
#define DEVICE4_MAC "B43A-4581-F073"

// Device names (customizable)
#define DEVICE1_NAME "Switch 1"
#define DEVICE2_NAME "Switch 2"
#define DEVICE3_NAME "Switch 3"
#define DEVICE4_NAME "Switch 4"

// ============================================================================
// Web Server Configuration
// ============================================================================
#define WEB_SERVER_PORT 80

// ============================================================================
// Status Update Configuration
// ============================================================================
#define STATUS_UPDATE_INTERVAL 5000  // Update status every 5 seconds
#define QUERY_DELAY_MS 100           // Delay between device queries

// ============================================================================
// Display Configuration
// ============================================================================
#define DISPLAY_UPDATE_INTERVAL 2000  // Update display every 2 seconds
#define DISPLAY_ROTATION 1            // Display rotation (0-3)

// ============================================================================
// Serial Configuration
// ============================================================================
#define SERIAL_BAUD_RATE 115200

#endif // CONFIG_H
