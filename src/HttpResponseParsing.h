/*
 * Arduino-Http-Requests Library
 * File: HttpResponseParsing.h
 * 
 * Copyright (c) 2025 Dominik Werner
 * https://github.com/dowerner/Arduino-Http-Requests
 *
 * This file is part of the Arduino-Http-Requests library and is licensed
 * under the MIT License. See LICENSE file for details.
 */


#pragma once
#include <Arduino.h>
#include "HttpResponse.h"

class HttpResponseParsing {
    public:
        static HttpResponse parseResponse(String &response) {
            const size_t responseLenght = response.length();
            String line = String();
            char c;
            HttpResponse result;
            result.responseCode = 0;
            result.contentLength = 0;

            for (size_t i = 0; i < responseLenght; ++i) {
                c = response[i];
                if (c != '\n') {
                    line.concat(c);
                    continue;
                }
                line.trim();

                if (line.startsWith("HTTP/")) {
                    result.responseCode = parseResponseCode(line);
                }
                else if (line.startsWith("Content-Type")) {
                    result.contentType = getStringAfterColon(line);
                }
                else if (line.startsWith("Content-Length")) {
                    result.contentLength = getStringAfterColon(line).toInt();
                }
                else if (line.startsWith("Server")) {
                    result.server = getStringAfterColon(line);
                }
                else if (line.length() == 0 && result.contentLength == 0) {
                    // detect empty line (start of response body) and generate content length from what is left of the response
                    result.contentLength = responseLenght - i - 1;
                    break;
                }

                line = String();
            }

            if (result.contentLength > 0) {
                result.contentText = response.substring(responseLenght - result.contentLength);
            }

            return result;
        }

    private:
        static size_t parseResponseCode(String &line) {
            bool inResponse = false;
            size_t lineLength = line.length();
            String codeStr = String();
            char c;
            for (int16_t i = 0; i < lineLength; ++i) {
                c = line[i];
                if (c == ' ') {
                    if (!inResponse) {
                        inResponse = true;
                        continue;
                    }
                    else {
                        break;
                    }
                }
                
                if (inResponse) codeStr.concat(c);
            }
            return codeStr.toInt();            
        }

        static String getStringAfterColon(String &line) {
            size_t colonIndex = line.indexOf(':');
            if (colonIndex == -1) return String();
            String result = line.substring(colonIndex+1);
            result.trim();
            return result;
        }
};