/**
 * M5PaperS3 Dashboard Controller
 * 
 * This sketch implements a modern dashboard controller for M5PaperS3 board
 * that controls 4 M5SwitchC6 devices via MQTT.
 * 
 * Features:
 * - 4 toggle switches for controlling M5SwitchC6 devices
 * - Web interface for remote control
 * - E-paper display showing real-time device status
 * - WiFi and MQTT connectivity
 * - Status monitoring and display
 * 
 * Default Credentials:
 * - WiFi SSID: "CJAK2_2G"
 * - WiFi Password: "CJak55998718"
 * - MQTT Server: "192.168.0.82"
 * - MQTT Username: "admin"
 * - MQTT Password: "$Orange111"
 */

#include <M5Unified.h>
#include <WiFi.h>
#include <WebServer.h>
#include <M5Switchc6.h>
#include <PubSubClient.h>

// WiFi Configuration
const char* WIFI_SSID = "CJAK2_2G";
const char* WIFI_PASSWORD = "CJak55998718";

// MQTT Configuration
const char* MQTT_SERVER = "192.168.0.82";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "admin";
const char* MQTT_PASSWORD = "$Orange111";
const char* MQTT_CLIENT_ID = "M5PaperS3_Dashboard";

// M5SwitchC6 Configuration
M5SwitchC6 switchC6(16);  // RX pin 16

// Device MAC addresses (4 devices)
const String DEVICE_MACS[4] = {
  "B43A-4581-F070",  // Device 1
  "B43A-4581-F071",  // Device 2
  "B43A-4581-F072",  // Device 3
  "B43A-4581-F073"   // Device 4
};

// Device states
bool deviceStates[4] = {false, false, false, false};
String deviceNames[4] = {"Switch 1", "Switch 2", "Switch 3", "Switch 4"};

// Web Server
WebServer server(80);

// MQTT Client
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Status tracking
bool wifiConnected = false;
bool mqttEnabled = true;
bool mqttConnected = false;
unsigned long lastStatusUpdate = 0;
const unsigned long STATUS_UPDATE_INTERVAL = 5000; // 5 seconds

// Display tracking
unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_UPDATE_INTERVAL = 2000; // 2 seconds

// Display dimensions (will be set based on actual screen size and orientation)
int screenWidth = 0;
int screenHeight = 0;

// Touch areas for toggle switches
struct TouchArea {
  int x, y, width, height;
  int deviceIndex;
};
TouchArea touchAreas[4];

/**
 * Setup WiFi Connection
 */
void setupWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    wifiConnected = false;
    Serial.println("\nWiFi connection failed!");
  }
}

/**
 * Setup MQTT Connection
 */
void setupMQTT() {
  if (!mqttEnabled) return;
  
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
}

/**
 * Connect to MQTT Broker
 */
void connectMQTT() {
  if (!mqttEnabled || !wifiConnected) return;
  
  if (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      mqttConnected = true;
      Serial.println("MQTT connected!");
      
      // Subscribe to relevant topics
      mqttClient.subscribe("m5switch/status/#");
    } else {
      mqttConnected = false;
      Serial.print("MQTT connection failed, rc=");
      Serial.println(mqttClient.state());
    }
  }
}

/**
 * MQTT Callback Handler
 */
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
}

/**
 * Turn device ON
 */
bool turnDeviceOn(int deviceIndex) {
  if (deviceIndex < 0 || deviceIndex >= 4) return false;
  
  Serial.print("Turning ON device ");
  Serial.print(deviceIndex);
  Serial.print(" (");
  Serial.print(DEVICE_MACS[deviceIndex]);
  Serial.println(")");
  
  bool success = switchC6.sendSwitchCommandWithResponse(DEVICE_MACS[deviceIndex], true, 10000);
  
  if (success) {
    deviceStates[deviceIndex] = true;
    Serial.println("Device turned ON successfully");
    displayStatus(); // Update display immediately
  } else {
    Serial.println("Failed to turn ON device");
  }
  
  return success;
}

/**
 * Turn device OFF
 */
bool turnDeviceOff(int deviceIndex) {
  if (deviceIndex < 0 || deviceIndex >= 4) return false;
  
  Serial.print("Turning OFF device ");
  Serial.print(deviceIndex);
  Serial.print(" (");
  Serial.print(DEVICE_MACS[deviceIndex]);
  Serial.println(")");
  
  bool success = switchC6.sendSwitchCommandWithResponse(DEVICE_MACS[deviceIndex], false, 10000);
  
  if (success) {
    deviceStates[deviceIndex] = false;
    Serial.println("Device turned OFF successfully");
    displayStatus(); // Update display immediately
  } else {
    Serial.println("Failed to turn OFF device");
  }
  
  return success;
}

/**
 * Query device status
 */
bool queryDeviceStatus(int deviceIndex) {
  if (deviceIndex < 0 || deviceIndex >= 4) return false;
  
  Serial.print("Querying status for device ");
  Serial.print(deviceIndex);
  Serial.print(" (");
  Serial.print(DEVICE_MACS[deviceIndex]);
  Serial.println(")");
  
  SwitchC6ParsedData_t response;
  bool success = switchC6.sendStatusQueryWithResponse(DEVICE_MACS[deviceIndex], 10000, &response);
  
  if (success) {
    deviceStates[deviceIndex] = response.switchState;
    Serial.print("Device status: ");
    Serial.println(response.switchState ? "ON" : "OFF");
    displayStatus(); // Update display immediately
  } else {
    Serial.println("Failed to query device status");
  }
  
  return success;
}

/**
 * Update all device statuses
 */
void updateAllDeviceStatuses() {
  for (int i = 0; i < 4; i++) {
    queryDeviceStatus(i);
    delay(100); // Small delay between queries
  }
}

/**
 * Display status on M5PaperS3 screen
 * Dynamically scaled to use full screen width based on actual device orientation
 */
void displayStatus() {
  // Get actual screen dimensions based on current rotation
  screenWidth = M5.Display.width();
  screenHeight = M5.Display.height();
  
  // Fill screen with black for white-on-black display
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setBrightness(255); // Set maximum brightness
  
  // Calculate margins and positions dynamically
  int margin = 20;
  int contentWidth = screenWidth - (margin * 2);
  
  // Title - larger font
  M5.Display.setCursor(margin, 20);
  M5.Display.setTextSize(3);
  M5.Display.println("M5PaperS3");
  M5.Display.setCursor(margin, 60);
  M5.Display.println("Dashboard");
  
  // Connection status - medium font
  M5.Display.setTextSize(2);
  M5.Display.setCursor(margin, 120);
  M5.Display.print("WiFi: ");
  M5.Display.println(wifiConnected ? "Connected" : "Disconnected");
  
  M5.Display.setCursor(margin, 150);
  M5.Display.print("MQTT: ");
  M5.Display.println(mqttConnected ? "Connected" : "Disconnected");
  
  if (wifiConnected) {
    M5.Display.setCursor(margin, 180);
    M5.Display.print("IP: ");
    M5.Display.println(WiFi.localIP().toString());
  }
  
  // Device status - larger switches using full width
  M5.Display.setCursor(margin, 240);
  M5.Display.setTextSize(3);
  M5.Display.println("Devices:");
  
  // Calculate available space for switches
  int availableVerticalSpace = screenHeight - 300 - 80; // Space after header, before footer
  int switchSpacing = availableVerticalSpace / 4;
  
  // Draw 4 large toggle switches - each using proportional space
  for (int i = 0; i < 4; i++) {
    int yPos = 300 + (i * switchSpacing);
    
    // Device name - larger font
    M5.Display.setCursor(margin + 10, yPos);
    M5.Display.setTextSize(2);
    M5.Display.print(deviceNames[i]);
    
    // Calculate toggle switch dimensions dynamically
    // Switch should be on the right side, taking ~40% of width
    int switchWidth = min(180, (int)(contentWidth * 0.4));
    int switchHeight = min(80, (int)(switchSpacing - 20));
    int switchX = screenWidth - margin - switchWidth - 10;
    int switchY = yPos - 5;
    int toggleRadius = switchHeight / 3;
    
    // Store touch area for this switch - use full row width for easier tapping
    touchAreas[i].x = 0;
    touchAreas[i].y = yPos - 10;
    touchAreas[i].width = screenWidth;
    touchAreas[i].height = switchSpacing;
    touchAreas[i].deviceIndex = i;
    
    // Draw switch background (rounded rectangle with extra border for touch visibility)
    M5.Display.drawRoundRect(switchX, switchY, switchWidth, switchHeight, 20, WHITE);
    M5.Display.drawRoundRect(switchX + 1, switchY + 1, switchWidth - 2, switchHeight - 2, 19, WHITE);
    M5.Display.drawRoundRect(switchX + 2, switchY + 2, switchWidth - 4, switchHeight - 4, 18, WHITE);
    
    if (deviceStates[i]) {
      // ON state - fill background and draw toggle on right
      M5.Display.fillRoundRect(switchX + 2, switchY + 2, switchWidth - 4, switchHeight - 4, 18, WHITE);
      M5.Display.fillCircle(switchX + switchWidth - toggleRadius - 10, switchY + switchHeight / 2, toggleRadius, BLACK);
      M5.Display.drawCircle(switchX + switchWidth - toggleRadius - 10, switchY + switchHeight / 2, toggleRadius, WHITE);
      M5.Display.drawCircle(switchX + switchWidth - toggleRadius - 10, switchY + switchHeight / 2, toggleRadius - 1, WHITE);
    } else {
      // OFF state - draw toggle on left
      M5.Display.fillCircle(switchX + toggleRadius + 10, switchY + switchHeight / 2, toggleRadius, WHITE);
      M5.Display.fillCircle(switchX + toggleRadius + 10, switchY + switchHeight / 2, toggleRadius - 2, BLACK);
      M5.Display.drawCircle(switchX + toggleRadius + 10, switchY + switchHeight / 2, toggleRadius, WHITE);
    }
    
    // Show MAC address below device name
    M5.Display.setCursor(margin + 10, yPos + 30);
    M5.Display.setTextSize(1);
    M5.Display.print(DEVICE_MACS[i]);
    
    // Show ON/OFF text
    M5.Display.setCursor(margin + 10, yPos + 48);
    M5.Display.setTextSize(2);
    M5.Display.print(deviceStates[i] ? "ON" : "OFF");
  }
  
  // Last update time at bottom
  int footerY = screenHeight - 60;
  M5.Display.setCursor(margin, footerY);
  M5.Display.setTextSize(1);
  M5.Display.print("Updated: ");
  M5.Display.print(millis() / 1000);
  M5.Display.print("s");
  
  // Debug: Display screen info and touch status
  M5.Display.setCursor(margin, footerY + 20);
  M5.Display.print("Screen: ");
  M5.Display.print(screenWidth);
  M5.Display.print("x");
  M5.Display.print(screenHeight);
  M5.Display.print(" | Touch: ");
  M5.Display.print(M5.Touch.isEnabled() ? "Ready" : "N/A");
}

/**
 * Initialize display
 */
void setupDisplay() {
  M5.Display.setRotation(1);
  
  // Get actual screen dimensions after rotation is set
  screenWidth = M5.Display.width();
  screenHeight = M5.Display.height();
  
  M5.Display.fillScreen(BLACK); // Black background for white text
  M5.Display.setTextColor(WHITE, BLACK); // White text on black background
  M5.Display.setBrightness(255); // Set maximum brightness
  M5.Display.setTextSize(1);
  
  // Show initialization message
  M5.Display.setCursor(20, 20);
  M5.Display.setTextSize(3);
  M5.Display.println("Initializing...");
  M5.Display.setTextSize(2);
  M5.Display.setCursor(20, 80);
  M5.Display.println("M5PaperS3 Dashboard");
  M5.Display.setCursor(20, 120);
  M5.Display.setTextSize(1);
  M5.Display.print("Screen: ");
  M5.Display.print(screenWidth);
  M5.Display.print("x");
  M5.Display.println(screenHeight);
  
  Serial.println("Display initialized");
  Serial.print("Screen dimensions: ");
  Serial.print(screenWidth);
  Serial.print("x");
  Serial.println(screenHeight);
}

/**
 * Handle touch input on toggle switches
 */
void handleTouch() {
  // Check if touch is available and update touch state
  if (M5.Touch.isEnabled()) {
    auto t = M5.Touch.getDetail();
    
    // Check for touch press event
    if (t.wasPressed() || t.isPressed()) {
      int touchX = t.x;
      int touchY = t.y;
      
      // Debug output
      Serial.print("Touch detected at: X=");
      Serial.print(touchX);
      Serial.print(", Y=");
      Serial.println(touchY);
      
      // Check if touch is within any toggle switch area
      for (int i = 0; i < 4; i++) {
        if (touchX >= touchAreas[i].x && 
            touchX <= touchAreas[i].x + touchAreas[i].width &&
            touchY >= touchAreas[i].y && 
            touchY <= touchAreas[i].y + touchAreas[i].height) {
          
          // Toggle the device state
          int deviceIndex = touchAreas[i].deviceIndex;
          Serial.print("Touch matched switch ");
          Serial.print(deviceIndex);
          Serial.print(": ");
          Serial.println(deviceNames[deviceIndex]);
          
          // Add debouncing - only toggle if it's a new press
          static unsigned long lastTouchTime = 0;
          static int lastTouchedDevice = -1;
          unsigned long currentTime = millis();
          
          if (t.wasPressed() || (currentTime - lastTouchTime > 500) || (lastTouchedDevice != deviceIndex)) {
            if (deviceStates[deviceIndex]) {
              turnDeviceOff(deviceIndex);
            } else {
              turnDeviceOn(deviceIndex);
            }
            lastTouchTime = currentTime;
            lastTouchedDevice = deviceIndex;
          }
          
          break; // Only handle one touch at a time
        }
      }
    }
  } else {
    // Touch is not enabled, log once
    static bool loggedOnce = false;
    if (!loggedOnce) {
      Serial.println("Touch is not enabled on this device");
      loggedOnce = true;
    }
  }
}

/**
 * Generate HTML page for web interface
 */
String generateWebPage() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>M5PaperS3 Dashboard</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); }";
  html += ".container { max-width: 800px; margin: 0 auto; background: white; border-radius: 10px; padding: 30px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); }";
  html += "h1 { color: #333; text-align: center; margin-bottom: 10px; }";
  html += ".status-bar { background: #f0f0f0; padding: 10px; border-radius: 5px; margin-bottom: 20px; text-align: center; }";
  html += ".status-item { display: inline-block; margin: 0 15px; }";
  html += ".status-dot { display: inline-block; width: 10px; height: 10px; border-radius: 50%; margin-right: 5px; }";
  html += ".status-dot.connected { background: #4CAF50; }";
  html += ".status-dot.disconnected { background: #f44336; }";
  html += ".switch-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin-top: 20px; }";
  html += ".switch-card { background: #f9f9f9; border-radius: 8px; padding: 20px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); transition: transform 0.2s; }";
  html += ".switch-card:hover { transform: translateY(-5px); box-shadow: 0 5px 15px rgba(0,0,0,0.2); }";
  html += ".switch-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 15px; }";
  html += ".switch-name { font-size: 18px; font-weight: bold; color: #333; }";
  html += ".switch-status { padding: 5px 12px; border-radius: 20px; font-size: 12px; font-weight: bold; }";
  html += ".switch-status.on { background: #4CAF50; color: white; }";
  html += ".switch-status.off { background: #ccc; color: #666; }";
  html += ".switch-controls { display: flex; gap: 10px; }";
  html += "button { flex: 1; padding: 12px; border: none; border-radius: 5px; font-size: 14px; font-weight: bold; cursor: pointer; transition: all 0.3s; }";
  html += "button:hover { transform: scale(1.05); }";
  html += ".btn-on { background: #4CAF50; color: white; }";
  html += ".btn-on:hover { background: #45a049; }";
  html += ".btn-off { background: #f44336; color: white; }";
  html += ".btn-off:hover { background: #da190b; }";
  html += ".btn-query { background: #2196F3; color: white; }";
  html += ".btn-query:hover { background: #0b7dda; }";
  html += ".refresh-btn { background: #FF9800; color: white; padding: 12px 30px; border: none; border-radius: 5px; font-size: 16px; cursor: pointer; display: block; margin: 20px auto 0; }";
  html += ".refresh-btn:hover { background: #e68900; }";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<h1>M5PaperS3 Dashboard</h1>";
  
  // Status bar
  html += "<div class='status-bar'>";
  html += "<span class='status-item'><span class='status-dot " + String(wifiConnected ? "connected" : "disconnected") + "'></span>WiFi: " + String(wifiConnected ? "Connected" : "Disconnected") + "</span>";
  html += "<span class='status-item'><span class='status-dot " + String(mqttConnected ? "connected" : "disconnected") + "'></span>MQTT: " + String(mqttConnected ? "Connected" : "Disconnected") + "</span>";
  html += "<span class='status-item'>IP: " + WiFi.localIP().toString() + "</span>";
  html += "</div>";
  
  // Switch cards
  html += "<div class='switch-grid'>";
  for (int i = 0; i < 4; i++) {
    html += "<div class='switch-card'>";
    html += "<div class='switch-header'>";
    html += "<span class='switch-name'>" + deviceNames[i] + "</span>";
    html += "<span class='switch-status " + String(deviceStates[i] ? "on" : "off") + "'>" + String(deviceStates[i] ? "ON" : "OFF") + "</span>";
    html += "</div>";
    html += "<div class='switch-controls'>";
    html += "<button class='btn-on' onclick=\"location.href='/toggle?device=" + String(i) + "&state=1'\">ON</button>";
    html += "<button class='btn-off' onclick=\"location.href='/toggle?device=" + String(i) + "&state=0'\">OFF</button>";
    html += "<button class='btn-query' onclick=\"location.href='/query?device=" + String(i) + "'\">Query</button>";
    html += "</div>";
    html += "<div style='margin-top: 10px; font-size: 12px; color: #666;'>MAC: " + DEVICE_MACS[i] + "</div>";
    html += "</div>";
  }
  html += "</div>";
  
  html += "<button class='refresh-btn' onclick='location.reload()'>Refresh Status</button>";
  html += "</div>";
  html += "</body></html>";
  
  return html;
}

/**
 * Handle root page request
 */
void handleRoot() {
  server.send(200, "text/html", generateWebPage());
}

/**
 * Handle toggle request
 */
void handleToggle() {
  if (server.hasArg("device") && server.hasArg("state")) {
    int device = server.arg("device").toInt();
    int state = server.arg("state").toInt();
    
    bool success;
    if (state == 1) {
      success = turnDeviceOn(device);
    } else {
      success = turnDeviceOff(device);
    }
    
    // Redirect back to main page
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(400, "text/plain", "Missing parameters");
  }
}

/**
 * Handle query request
 */
void handleQuery() {
  if (server.hasArg("device")) {
    int device = server.arg("device").toInt();
    queryDeviceStatus(device);
    
    // Redirect back to main page
    server.sendHeader("Location", "/");
    server.send(303);
  } else {
    server.send(400, "text/plain", "Missing device parameter");
  }
}

/**
 * Handle status API endpoint
 */
void handleStatusAPI() {
  String json = "{\"devices\":[";
  for (int i = 0; i < 4; i++) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"id\":" + String(i) + ",";
    json += "\"name\":\"" + deviceNames[i] + "\",";
    json += "\"mac\":\"" + DEVICE_MACS[i] + "\",";
    json += "\"state\":" + String(deviceStates[i] ? "true" : "false");
    json += "}";
  }
  json += "],";
  json += "\"wifi\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"mqtt\":" + String(mqttConnected ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

/**
 * Setup Web Server routes
 */
void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/query", handleQuery);
  server.on("/api/status", handleStatusAPI);
  
  server.begin();
  Serial.println("Web server started");
}

/**
 * Setup function - runs once at startup
 */
void setup() {
  // Initialize M5 device with touch explicitly enabled
  auto cfg = M5.config();
  cfg.clear_display = true;
  
  // Explicitly enable touch
  M5.begin(cfg);
  
  // Initialize Serial
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== M5PaperS3 Dashboard Controller ===");
  
  // Get and print display info
  Serial.print("Display rotation: ");
  Serial.println(M5.Display.getRotation());
  
  // Initialize display
  setupDisplay();
  
  // Initialize M5SwitchC6
  switchC6.begin();
  Serial.println("M5SwitchC6 initialized");
  
  // Setup WiFi
  setupWiFi();
  
  // Setup MQTT
  if (mqttEnabled) {
    setupMQTT();
    connectMQTT();
  }
  
  // Setup Web Server
  if (wifiConnected) {
    setupWebServer();
    Serial.println("\nWeb interface available at: http://" + WiFi.localIP().toString());
  }
  
  // Query initial device statuses
  Serial.println("\nQuerying initial device statuses...");
  updateAllDeviceStatuses();
  
  // Display initial status
  displayStatus();
  
  // Print touch area information for debugging
  Serial.println("\n=== Touch Areas (Full Row Width) ===");
  for (int i = 0; i < 4; i++) {
    Serial.print("Switch ");
    Serial.print(i);
    Serial.print(" (");
    Serial.print(deviceNames[i]);
    Serial.print("): X=");
    Serial.print(touchAreas[i].x);
    Serial.print("-");
    Serial.print(touchAreas[i].x + touchAreas[i].width);
    Serial.print(" (full width), Y=");
    Serial.print(touchAreas[i].y);
    Serial.print("-");
    Serial.println(touchAreas[i].y + touchAreas[i].height);
  }
  Serial.print("Touch enabled: ");
  Serial.println(M5.Touch.isEnabled() ? "Yes" : "No");
  Serial.print("Touch count: ");
  Serial.println(M5.Touch.getCount());
  
  Serial.println("\n=== Setup Complete ===");
  Serial.println("Dashboard ready for operation");
}

/**
 * Main loop function
 */
void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Handle MQTT
  if (mqttEnabled && wifiConnected) {
    if (!mqttClient.connected()) {
      connectMQTT();
    }
    mqttClient.loop();
  }
  
  // Periodic status updates
  if (millis() - lastStatusUpdate > STATUS_UPDATE_INTERVAL) {
    lastStatusUpdate = millis();
    
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
      wifiConnected = false;
      Serial.println("WiFi disconnected, attempting to reconnect...");
      setupWiFi();
    }
  }
  
  // Periodic display updates
  if (millis() - lastDisplayUpdate > DISPLAY_UPDATE_INTERVAL) {
    lastDisplayUpdate = millis();
    displayStatus();
  }
  
  // Update M5 device and handle touch
  M5.update();
  handleTouch();
  
  delay(10);
}
