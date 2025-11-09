/*
 * Arduino-Http-Requests Library
 * File: HttpRequest.h
 * 
 * Copyright (c) 2025 Dominik Werner
 * https://github.com/dowerner/Arduino-Http-Requests
 *
 * This file is part of the Arduino-Http-Requests library and is licensed
 * under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Client.h"
#include "HttpCallback.h"

template<typename TClient>
struct HttpRequest {
    TClient* client;
    RequestCompletedCallback* callback;
    unsigned long requestStartTS;
    
    HttpRequest() : client(nullptr), callback(nullptr) {}
    
    ~HttpRequest() {
        client = nullptr;
    }
};