/*
 * Arduino-Http-Requests Library
 * File: HttpResponse.h
 * 
 * Copyright (c) 2025 Dominik Werner
 * https://github.com/dowerner/Arduino-Http-Requests
 *
 * This file is part of the Arduino-Http-Requests library and is licensed
 * under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

enum HttpRequstStatus {
    Sent = 1,
    Completed = 2,
    NoResponse = 3,
    Failed_UnableToConnectToServer = 30,
    Failed_InvalidUrl = 31,
    Failed_UnableToSerializeBody = 32,
    Failed_TooManyConcurrentRequests = 33
};

struct HttpResponse {
    HttpRequstStatus status;
    size_t responseCode;
    String contentType;
    size_t contentLength;
    String server;
    String contentText;

    DeserializationError asJson(JsonDocument &doc) {
        return deserializeJson(doc, contentText);
    }
};