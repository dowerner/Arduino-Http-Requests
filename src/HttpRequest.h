#pragma once

#include "Client.h"
#include "HttpCallback.h"

template<typename T>
struct HttpRequest {
    T* client;
    RequestCompletedCallback* callback;
    unsigned long requestStartTS;
    
    HttpRequest() : client(nullptr), callback(nullptr) {}
    
    ~HttpRequest() {
        if (client != nullptr) {
            delete client;
        }
    }
};