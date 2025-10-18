/**
 * Simple Test Sketch for M5PaperS3 Dashboard
 * 
 * This is a simplified version for testing basic functionality.
 * Use this to verify:
 * - M5PaperS3 board communication
 * - M5SwitchC6 device control
 * - WiFi connectivity
 * 
 * Upload this sketch first to test individual components before
 * deploying the full dashboard.
 */

#include <M5Unified.h>
#include <WiFi.h>
#include <M5SwitchC6.h>

// Test Configuration
const char* WIFI_SSID = "CJAK2";
const char* WIFI_PASSWORD = "CJak55998718";
const String TEST_DEVICE_MAC = "B43A-4581-F070";

// M5SwitchC6 instance
M5SwitchC6 switchC6(16);  // RX pin 16

void setup() {
  // Initialize M5
  M5.begin();
  
  // Initialize Serial
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n=== M5PaperS3 Simple Test ===\n");
  
  // Test 1: M5 Device
  Serial.println("Test 1: M5 Device Initialization");
  Serial.println("✓ M5 device initialized");
  
  // Test 2: M5SwitchC6
  Serial.println("\nTest 2: M5SwitchC6 Initialization");
  switchC6.begin();
  Serial.println("✓ M5SwitchC6 initialized on RX pin 16");
  
  // Test 3: WiFi
  Serial.println("\nTest 3: WiFi Connection");
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n✗ WiFi connection failed!");
  }
  
  // Test 4: Device Control
  Serial.println("\nTest 4: M5SwitchC6 Device Control");
  Serial.print("Testing device: ");
  Serial.println(TEST_DEVICE_MAC);
  
  Serial.println("\nAttempting to turn device ON...");
  bool success = switchC6.sendSwitchCommandWithResponse(TEST_DEVICE_MAC, true, 10000);
  if (success) {
    Serial.println("✓ Device turned ON successfully");
  } else {
    Serial.println("✗ Failed to turn ON device");
  }
  
  delay(2000);
  
  Serial.println("\nAttempting to turn device OFF...");
  success = switchC6.sendSwitchCommandWithResponse(TEST_DEVICE_MAC, false, 10000);
  if (success) {
    Serial.println("✓ Device turned OFF successfully");
  } else {
    Serial.println("✗ Failed to turn OFF device");
  }
  
  delay(2000);
  
  Serial.println("\nQuerying device status...");
  SwitchC6ParsedData_t response;
  success = switchC6.sendStatusQueryWithResponse(TEST_DEVICE_MAC, 10000, &response);
  if (success) {
    Serial.print("✓ Device status: ");
    Serial.println(response.state ? "ON" : "OFF");
  } else {
    Serial.println("✗ Failed to query device status");
  }
  
  Serial.println("\n=== Test Complete ===");
  Serial.println("\nIf all tests passed, you can proceed with the full dashboard sketch.");
  Serial.println("If any test failed, check:");
  Serial.println("- Hardware connections");
  Serial.println("- Library installations");
  Serial.println("- WiFi credentials");
  Serial.println("- Device MAC address");
}

void loop() {
  M5.update();
  delay(100);
}
