#pragma once

#include "LinkedList.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpCallback.h"
#include "UrlParsing.h"
#include "HttpResponseParsing.h"

#define HTTP_RESPONSE_BUFFER_SIZE 1024
#define RESPONSE_TIMEOUT_MS 60000


template <typename T>
class Http {
public:
    HttpRequstStatus get(const char* url, RequestCompletedCallback* onRequestCompleted) {
        return get(String(url), onRequestCompleted);
    }

    HttpRequstStatus get(const String& url, RequestCompletedCallback* onRequestCompleted) {
        return sendRequest(url, onRequestCompleted, "GET");
    }

    HttpRequstStatus post(const char* url, const char* body, RequestCompletedCallback* onRequestCompleted) {
        return post(String(url), String(body), onRequestCompleted);
    }

    HttpRequstStatus post(const char* url, const String& body, RequestCompletedCallback* onRequestCompleted) {
        return post(String(url), body, onRequestCompleted);
    }

    HttpRequstStatus post(const char* url, const JsonDocument& body, RequestCompletedCallback* onRequestCompleted) {
        return post(String(url), body, onRequestCompleted);
    }

    HttpRequstStatus post(const String& url, const JsonDocument& body, RequestCompletedCallback* onRequestCompleted) {
        String serializedBody;
        serializeJson(body, serializedBody);
        return post(url, serializedBody, onRequestCompleted);
    }

    HttpRequstStatus post(const String& url, const String& body, RequestCompletedCallback* onRequestCompleted) {
        String commands[] = { 
            String("Content-Type: application/json"),
            String("Content-Length: ") + String(body.length()),
            String(),
            body
        };
        return sendRequest(url, onRequestCompleted, "POST", commands, 4);
    }

    HttpRequstStatus put(const char* url, const JsonDocument& body, RequestCompletedCallback* onRequestCompleted) {
        return put(String(url), body, onRequestCompleted);
    }

    HttpRequstStatus put(const String& url, const JsonDocument& body, RequestCompletedCallback* onRequestCompleted) {
        String serializedBody;
        serializeJson(body, serializedBody);
        return put(url, serializedBody, onRequestCompleted);
    }

    HttpRequstStatus put(const char* url, const char* body, RequestCompletedCallback* onRequestCompleted) {
        return put(String(url), String(body), onRequestCompleted);
    }

    HttpRequstStatus put(const char* url, const String& body, RequestCompletedCallback* onRequestCompleted) {
        return put(String(url), body, onRequestCompleted);
    }

    HttpRequstStatus put(const String& url, const String& body, RequestCompletedCallback* onRequestCompleted) {
        String commands[] = { 
            String("Content-Type: application/json"),
            String("Content-Length: ") + String(body.length()),
            String(),
            body
        };
        return sendRequest(url, onRequestCompleted, "PUT", commands, 4);
    }

    HttpRequstStatus del(const char* url, RequestCompletedCallback* onRequestCompleted) {
        return del(String(url), onRequestCompleted);
    }

    HttpRequstStatus del(const String& url, RequestCompletedCallback* onRequestCompleted) {
        return sendRequest(url, onRequestCompleted, "DELETE");
    }

    HttpRequstStatus postAsForm(const char* url, const char* formString, RequestCompletedCallback* onRequestCompleted) {
        return postAsForm(String(url), formString, onRequestCompleted);
    }

    HttpRequstStatus postAsForm(const String& url, const char* formString, RequestCompletedCallback* onRequestCompleted) {
        String formData = String(formString);
        String commands[] = { 
            String("Content-Type: application/x-www-form-urlencoded"),
            String("Content-Length: ") + String(formData.length()),
            String(),
            formData
        };
        return sendRequest(url, onRequestCompleted, "POST", commands, 4);
    }

    virtual String getLocalIP() = 0;  // Pure virtual function - must be implemented by derived classes

    void loop() {
        size_t requestCount = pendingRequests->getSize();

        if (requestCount == 0) return;

        unsigned long ts = millis();

        for (size_t i = 0; i < requestCount; ++i) {
            HttpRequest<T>* request = nullptr;
            if (!pendingRequests->get(i, request) || request == nullptr || !request->client->available()) {
                // check if response timed out
                unsigned long requestDurationMs = ts - request->requestStartTS;
                if (requestDurationMs > RESPONSE_TIMEOUT_MS) {
                    if (request->callback != nullptr) {
                        HttpResponse timeoutResponse;
                        timeoutResponse.status = HttpRequstStatus::NoResponse;
                        timeoutResponse.responseCode = 0;
                        request->callback(timeoutResponse);
                    }

                    // Remove and delete the timed out request
                    pendingRequests->removeAt(i);
                    delete request;
                    
                    // Don't increment i since the next item is now at the same index
                    --i;
                    --requestCount;  // Update the count since we removed an item
                }
                continue;
            }

            // Reset state for this request
            size_t bytesRead = 0;
            String responseText = String();
            char localRespBuffer[HTTP_RESPONSE_BUFFER_SIZE];
            char currentChar;

            // Process this request
            while (request->client->available()) {
                currentChar = request->client->read();
                localRespBuffer[bytesRead++] = currentChar;

                if (bytesRead == HTTP_RESPONSE_BUFFER_SIZE) {
                    responseText.concat(localRespBuffer);
                    bytesRead = 0;
                }
            }
            
            // Add remaining bytes
            if (bytesRead > 0) {
                responseText += String(localRespBuffer, bytesRead);
            }
            
            // Serial.println(responseText);
            HttpResponse response = HttpResponseParsing::parseResponse(responseText);
            response.status = HttpRequstStatus::Completed;

            if (request->callback != nullptr) {
                request->callback(response);
            }

            // Remove and delete the completed request
            pendingRequests->removeAt(i);
            delete request;
            
            // Don't increment i since the next item is now at the same index
            --i;
            --requestCount;  // Update the count since we removed an item
        }
    }

    Http<T>() {
        pendingRequests = new List<HttpRequest<T>*>();
    }
    
    ~Http<T>() {
        // Clean up all HttpRequest objects
        HttpRequest<T>* request;
        while (pendingRequests->getSize() > 0) {
            if (pendingRequests->get(0, request)) {
                delete request;  // This will delete the HttpRequest and its client
                pendingRequests->removeAt(0);
            }
        }
        delete pendingRequests;
    }
    
private:
    List<HttpRequest<T>*>* pendingRequests;

    HttpRequstStatus sendRequest(const String& url, RequestCompletedCallback* onRequestCompleted, const char* method) {
        return sendRequest(url, onRequestCompleted, method, nullptr, 0);
    }

    HttpRequstStatus sendRequest(const String& url, RequestCompletedCallback* onRequestCompleted, const char* method, String clientCommands[], int16_t commandCount) {
        ParsedUrl parsedUrl = UrlParsing::parseUrl(url);
        if (parsedUrl.failed) {
            return HttpRequstStatus::Failed_InvalidUrl;
        }

        // Create and add request to the list
        HttpRequest<T>* request = new HttpRequest<T>();
        request->requestStartTS = millis();
        request->client = new T();
        request->callback = onRequestCompleted;
        pendingRequests->add(request);
        
        if (!request->client->connect(parsedUrl.host.c_str(), parsedUrl.port)) {
            return HttpRequstStatus::Failed_UnableToConnectToServer;
        }

        String command = String(method) + String(" ") + parsedUrl.path + String(" HTTP/1.1");
        String hostPart = String("Host: ") + getLocalIP();

        request->client->println(command.c_str());
        request->client->println(hostPart.c_str());
        request->client->println("Connection: close");

        // Serial.println("Custom commands\n");
        if (clientCommands != nullptr) {
            for (int16_t i = 0; i < commandCount; ++i) {
                request->client->println(clientCommands[i].c_str());
                // Serial.println(clientCommands[i]);
            }
        }
        request->client->println();

        return HttpRequstStatus::Sent;
    }

};
