#pragma once

#include <WiFiNINA.h>
#include "Http.h"

class HttpWiFiNINA : public Http<WiFiClient>{
public:
    String getLocalIP() override {
        IPAddress ip = WiFi.localIP();
        return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
    }
};
