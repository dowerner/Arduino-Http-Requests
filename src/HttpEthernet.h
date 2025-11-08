/*
 * Arduino-Http-Requests Library
 * File: HttpEthernet.h
 * 
 * Copyright (c) 2025 Dominik Werner
 * https://github.com/dowerner/Arduino-Http-Requests
 *
 * This file is part of the Arduino-Http-Requests library and is licensed
 * under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Ethernet.h>
#include "Http.h"

/**
 * Used to perform HTTP requests with:
 *   - Arduino Uno, Mega, Leonardo, or other boards equipped with the official Ethernet shield (W5100, W5200, or W5500)
 *   - Arduino Ethernet Rev3
 *   - Compatible boards using the Arduino Ethernet library
 *
 * This class implements the HTTP client interface using the Ethernet driver and EthernetClient.
 */
class HttpEthernet : public Http<EthernetClient> {
public:
    String getLocalIP() override {
        IPAddress ip = Ethernet.localIP();
        return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
    }
};