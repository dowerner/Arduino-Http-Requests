/*
 * Arduino-Http-Requests Library
 * File: HttpWiFi.h
 * 
 * Copyright (c) 2025 Dominik Werner
 * https://github.com/dowerner/Arduino-Http-Requests
 *
 * This file is part of the Arduino-Http-Requests library and is licensed
 * under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <WiFi.h>
#include "Http.h"

/**
 * Used to perform HTTP requests with:
 *   - ESP32 boards
 *   - ESP8266 boards
 *   - Any board using the ESP Arduino core's WiFi library
 *
 * This class implements the HTTP client interface using the ESP WiFi (WiFi.h) driver and WiFiClient.
 */
class HttpWifi : public Http<WiFiClient> {
public:
    String getLocalIP() override {
        IPAddress ip = WiFi.localIP();
        return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
    }
};