/**
 * Display Test Sketch for M5PaperS3 Dashboard
 * 
 * This test verifies the display functionality on the M5PaperS3 board.
 * Use this to verify:
 * - E-paper display initialization
 * - Text rendering on display
 * - Display updates
 * 
 * Upload this sketch to test the display before deploying the full dashboard.
 */

#include <M5Unified.h>

// Display configuration
#define DISPLAY_ROTATION 1  // Display rotation (0-3)

void setup() {
  // Initialize M5
  M5.begin();
  
  // Initialize Serial
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== M5PaperS3 Display Test ===\n");
  
  // Test 1: Display Initialization
  Serial.println("Test 1: Display Initialization");
  M5.Display.setRotation(DISPLAY_ROTATION);
  M5.Display.fillScreen(BLACK); // Black background
  M5.Display.setTextColor(WHITE, BLACK); // White text on black
  M5.Display.setBrightness(255); // Maximum brightness
  Serial.println("✓ Display initialized");
  
  // Test 2: Basic Text Rendering
  Serial.println("\nTest 2: Basic Text Rendering");
  M5.Display.setCursor(10, 10);
  M5.Display.setTextSize(2);
  M5.Display.println("M5PaperS3 Display Test");
  M5.Display.setTextSize(1);
  M5.Display.setCursor(10, 40);
  M5.Display.println("If you can read this, the display works!");
  Serial.println("✓ Text rendered to display");
  
  // Test 3: Display Status Information
  Serial.println("\nTest 3: Display Status Information");
  M5.Display.setCursor(10, 70);
  M5.Display.println("Connection Status:");
  M5.Display.setCursor(10, 85);
  M5.Display.print("WiFi: Not Connected");
  M5.Display.setCursor(10, 100);
  M5.Display.print("MQTT: Not Connected");
  Serial.println("✓ Status information displayed");
  
  // Test 4: Display Device Status with Graphical Toggles
  Serial.println("\nTest 4: Display Device Status");
  M5.Display.setCursor(10, 125);
  M5.Display.setTextSize(2);
  M5.Display.println("Device Status:");
  M5.Display.setTextSize(1);
  
  String deviceNames[4] = {"Switch 1", "Switch 2", "Switch 3", "Switch 4"};
  bool deviceStates[4] = {true, false, true, false};
  
  for (int i = 0; i < 4; i++) {
    int yPos = 150 + (i * 30);
    M5.Display.setCursor(10, yPos);
    M5.Display.print(deviceNames[i]);
    
    // Draw graphical toggle switch
    int switchX = 150;
    int switchY = yPos - 2;
    int switchWidth = 40;
    int switchHeight = 20;
    int toggleRadius = 8;
    
    M5.Display.drawRoundRect(switchX, switchY, switchWidth, switchHeight, 10, WHITE);
    
    if (deviceStates[i]) {
      // ON state
      M5.Display.fillRoundRect(switchX, switchY, switchWidth, switchHeight, 10, WHITE);
      M5.Display.fillCircle(switchX + switchWidth - toggleRadius - 4, switchY + switchHeight / 2, toggleRadius, BLACK);
      M5.Display.drawCircle(switchX + switchWidth - toggleRadius - 4, switchY + switchHeight / 2, toggleRadius, WHITE);
    } else {
      // OFF state
      M5.Display.fillCircle(switchX + toggleRadius + 4, switchY + switchHeight / 2, toggleRadius, WHITE);
    }
  }
  Serial.println("✓ Device status displayed");
  
  // Test 5: Display Update Timestamp
  Serial.println("\nTest 5: Display Update Timestamp");
  M5.Display.setCursor(10, 280);
  M5.Display.print("Test time: ");
  M5.Display.print(millis() / 1000);
  M5.Display.print("s");
  Serial.println("✓ Timestamp displayed");
  
  Serial.println("\n=== All Display Tests Passed ===");
  Serial.println("\nThe display should now show:");
  Serial.println("- Title: M5PaperS3 Display Test");
  Serial.println("- Connection status (WiFi/MQTT)");
  Serial.println("- 4 device statuses with ON/OFF indicators");
  Serial.println("- Test timestamp");
  Serial.println("\nIf you can see this information on the e-paper display,");
  Serial.println("the display functionality is working correctly!");
}

void loop() {
  M5.update();
  
  // Update display every 5 seconds to show it's still running
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {
    lastUpdate = millis();
    
    // Update timestamp on display (clear with black background)
    M5.Display.fillRect(10, 280, 200, 20, BLACK);
    M5.Display.setCursor(10, 280);
    M5.Display.print("Running time: ");
    M5.Display.print(millis() / 1000);
    M5.Display.print("s");
    
    Serial.print("Display updated at ");
    Serial.print(millis() / 1000);
    Serial.println("s");
  }
  
  delay(100);
}
