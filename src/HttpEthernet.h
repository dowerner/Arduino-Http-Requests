#pragma once

#include <Ethernet.h>
#include "Http.h"

class HttpEthernet : public Http<EthernetClient> {
public:
    String getLocalIP() override {
        IPAddress ip = Ethernet.localIP();
        return String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]);
    }
};