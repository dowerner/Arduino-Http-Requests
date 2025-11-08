/*
 * Arduino-Http-Requests Library
 * File: HttpWiFi101.h
 * 
 * Copyright (c) 2025 Dominik Werner
 * https://github.com/dowerner/Arduino-Http-Requests
 *
 * This file is part of the Arduino-Http-Requests library and is licensed
 * under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <WiFi101.h>
#include "Http.h"

/**
 * Used to perform HTTP requests with:
 *   - Arduino MKR1000
 *   - Arduino boards or shields that are compatible with the WiFi101 library
 *
 * This class implements the HTTP client interface using the WiFi101 driver and WiFiClient.
 */
class HttpWiFi101 : public Http<WiFiClient> {
public:
    String getLocalIP() override {
        IPAddress ip = WiFi.localIP();
        return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);        
    }
};