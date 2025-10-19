# Display Test Example

This example sketch tests the display functionality of the M5PaperS3 board before deploying the full dashboard.

## Purpose

Test and verify:
- E-paper display initialization
- Text rendering capabilities
- Display updates and refresh
- Status display layout

## What It Does

The sketch performs the following tests:
1. **Display Initialization** - Sets up the e-paper display
2. **Text Rendering** - Displays title and welcome message
3. **Status Information** - Shows connection status (WiFi/MQTT)
4. **Device Status** - Displays 4 mock device states (ON/OFF)
5. **Timestamp** - Shows current running time

## Expected Output

### Serial Monitor (115200 baud)
```
=== M5PaperS3 Display Test ===

Test 1: Display Initialization
✓ Display initialized

Test 2: Basic Text Rendering
✓ Text rendered to display

Test 3: Display Status Information
✓ Status information displayed

Test 4: Display Device Status
✓ Device status displayed

Test 5: Display Update Timestamp
✓ Timestamp displayed

=== All Display Tests Passed ===
```

### E-Paper Display
The display should show (white text on black background for better visibility):
- **Title**: "M5PaperS3 Display Test"
- **Welcome message**: "If you can read this, the display works!"
- **Connection Status**: WiFi and MQTT (shown as Not Connected)
- **Device Status**: 4 switches with **graphical toggle indicators** showing alternating ON/OFF states
  - ON states appear with filled toggles on the right
  - OFF states appear with empty toggles on the left
- **Timestamp**: Running time in seconds (updates every 5 seconds)

## How to Use

1. Open this sketch in Arduino IDE
2. Select **M5Stack-PAPER** as the board
3. Upload to your M5PaperS3 device
4. Open Serial Monitor at 115200 baud
5. Observe the test results in Serial Monitor
6. Check the e-paper display for visual output

## Troubleshooting

### Display Shows Nothing
- Check that M5Unified library is installed
- Verify board selection is correct (M5Stack-PAPER)
- Try pressing the reset button on the device

### Text Appears Garbled
- Check display orientation (rotation setting)
- Verify M5Unified library version is up to date

### Display Doesn't Update
- E-paper displays have slower refresh rates (this is normal)
- Wait 5-10 seconds for the e-paper display to refresh completely
- Check Serial Monitor for update messages

## Next Steps

If all tests pass:
- You can proceed to upload the main M5PaperS3_Dashboard sketch
- The display functionality will work correctly in the full dashboard

If tests fail:
- Check hardware connections
- Verify library installations
- Consult M5Stack documentation
