# M5PaperS3 Dashboard Controller

A modern IoT dashboard for controlling M5SwitchC6 devices using the M5PaperS3 board.

## Overview

This project provides a complete Arduino sketch for the M5PaperS3 board that creates a web-based dashboard for controlling up to 4 M5SwitchC6 smart switch devices. The dashboard features a modern, responsive interface accessible from any web browser on your local network.

## Features

- 🎛️ **4 Device Control**: Manage up to 4 M5SwitchC6 devices independently
- 📺 **E-Paper Display**: Real-time status display on M5PaperS3 screen showing device states
- 🌐 **Web Interface**: Modern, responsive web UI for remote control
- 📡 **WiFi Connectivity**: Automatic connection with reconnection support
- 📊 **MQTT Support**: Optional MQTT integration for advanced automation
- 🔍 **Status Monitoring**: Real-time status queries for all devices
- 🔌 **REST API**: JSON API endpoint for programmatic access

## Quick Start

1. Navigate to the `M5PaperS3_Dashboard` directory
2. Open `M5PaperS3_Dashboard.ino` in Arduino IDE
3. Install required libraries (M5Unified, M5SwitchC6, PubSubClient)
4. (Optional) Test display first with `examples/DisplayTest/DisplayTest.ino`
5. Update device MAC addresses in the sketch
6. Upload to your M5PaperS3 board
7. Access the web interface at the IP address shown in Serial Monitor and on the e-paper display

## Documentation

- **[User Guide](M5PaperS3_Dashboard/README.md)** - Installation and usage instructions
- **[Technical Documentation](M5PaperS3_Dashboard/DOCUMENTATION.md)** - Architecture and API reference
- **[Configuration](M5PaperS3_Dashboard/config.h)** - Customizable settings

## Default Credentials

**WiFi:**
- SSID: `CJAK2`
- Password: `CJak55998718`

**MQTT:**
- Server: `192.168.0.82`
- Username: `admin`
- Password: `$Orange111`

## Project Structure

```
M5PaperS3_Dashboard/
├── M5PaperS3_Dashboard.ino    # Main sketch
├── config.h                    # Configuration
├── README.md                   # User guide
├── DOCUMENTATION.md            # Technical docs
├── platformio.ini             # PlatformIO config
└── examples/
    ├── SimpleTest/            # Basic functionality test
    └── DisplayTest/           # Display functionality test
```

## Requirements

- M5PaperS3 board
- M5SwitchC6 devices (up to 4)
- WiFi network (2.4GHz)
- Optional: MQTT broker

## License

This project is provided as-is for use with M5Stack products.