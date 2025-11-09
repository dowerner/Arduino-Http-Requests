/*
 * Arduino-Http-Requests Library
 * File: Http.h
 * 
 * Copyright (c) 2025 Dominik Werner
 * https://github.com/dowerner/Arduino-Http-Requests
 *
 * This file is part of the Arduino-Http-Requests library and is licensed
 * under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "LinkedList.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpCallback.h"
#include "UrlParsing.h"
#include "HttpResponseParsing.h"

#define HTTP_RESPONSE_BUFFER_SIZE 1024
#define RESPONSE_TIMEOUT_MS 60000

// Max client instances for socket-limited boards like WiFiNINA, W5100
#define DEFAULT_MAX_CLIENTS 4

/**
 * Base class for all specific HTTP request implementers.
 */
template <typename TClient>
class Http {
public:

    /**
     * @brief Sends an HTTP GET request to the specified URL.
     *
     * @param url The URL to request.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus get(const char* url, RequestCompletedCallback* onRequestCompleted) {
        return get(String(url), onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP GET request to the specified URL.
     *
     * @param url The URL to request.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus get(const String& url, RequestCompletedCallback* onRequestCompleted) {
        return sendRequest(url, onRequestCompleted, "GET");
    }

    /**
     * @brief Sends an HTTP POST request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be posted.
     * @param body The body of the POST request as a JSON string.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus post(const char* url, const char* body, RequestCompletedCallback* onRequestCompleted) {
        return post(String(url), String(body), onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP POST request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be posted.
     * @param body The body of the POST request as a JSON string.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus post(const char* url, const String& body, RequestCompletedCallback* onRequestCompleted) {
        return post(String(url), body, onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP POST request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be posted.
     * @param body The body of the POST request as a JSON document.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus post(const char* url, const JsonDocument& body, RequestCompletedCallback* onRequestCompleted) {
        return post(String(url), body, onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP POST request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be posted.
     * @param body The body of the POST request as a JSON document.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus post(const String& url, const JsonDocument& body, RequestCompletedCallback* onRequestCompleted) {
        String serializedBody;
        serializeJson(body, serializedBody);
        return post(url, serializedBody, onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP POST request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be posted.
     * @param body The body of the POST request as a JSON string.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus post(const String& url, const String& body, RequestCompletedCallback* onRequestCompleted) {
        String commands[] = { 
            String("Content-Type: application/json"),
            String("Content-Length: ") + String(body.length()),
            String(),
            body
        };
        return sendRequest(url, onRequestCompleted, "POST", commands, 4);
    }

    /**
     * @brief Sends an HTTP PUT request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be put.
     * @param body The body of the PUT request, as a JSON document.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus put(const char* url, const JsonDocument& body, RequestCompletedCallback* onRequestCompleted) {
        return put(String(url), body, onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP PUT request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be put.
     * @param body The body of the PUT request, as a JSON document.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus put(const String& url, const JsonDocument& body, RequestCompletedCallback* onRequestCompleted) {
        String serializedBody;
        serializeJson(body, serializedBody);
        return put(url, serializedBody, onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP PUT request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be put.
     * @param body The body of the PUT request, as a JSON string.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus put(const char* url, const char* body, RequestCompletedCallback* onRequestCompleted) {
        return put(String(url), String(body), onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP PUT request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be put.
     * @param body The body of the PUT request, as a JSON string.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus put(const char* url, const String& body, RequestCompletedCallback* onRequestCompleted) {
        return put(String(url), body, onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP PUT request with a JSON body to the specified URL.
     *
     * @param url The URL to which data will be put.
     * @param body The body of the PUT request, as a JSON string.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus put(const String& url, const String& body, RequestCompletedCallback* onRequestCompleted) {
        String commands[] = { 
            String("Content-Type: application/json"),
            String("Content-Length: ") + String(body.length()),
            String(),
            body
        };
        return sendRequest(url, onRequestCompleted, "PUT", commands, 4);
    }

    /**
     * @brief Sends an HTTP DELETE request to the specified URL.
     *
     * @param url The URL from which a resource will be deleted.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus del(const char* url, RequestCompletedCallback* onRequestCompleted) {
        return del(String(url), onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP DELETE request to the specified URL.
     *
     * @param url The URL from which a resource will be deleted.
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus del(const String& url, RequestCompletedCallback* onRequestCompleted) {
        return sendRequest(url, onRequestCompleted, "DELETE");
    }

    /**
     * @brief Sends an HTTP POST request with a form string body to the specified URL.
     *
     * @param url The URL to which data will be posted.
     * @param formString The form string to be posted (e.g. username=test&password=mypass1234)
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
    HttpRequstStatus postAsForm(const char* url, const char* formString, RequestCompletedCallback* onRequestCompleted) {
        return postAsForm(String(url), formString, onRequestCompleted);
    }

    /**
     * @brief Sends an HTTP POST request with a form string body to the specified URL.
     *
     * @param url The URL to which data will be posted.
     * @param formString The form string to be posted (e.g. username=test&password=mypass1234)
     * @param onRequestCompleted Callback function invoked when the response is received.
     * @return HttpRequstStatus Status indicating the current status of the request.
     */
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

    /**
     * Call this method within your sketche's loop() function to process all the pending requests.
     */
    void loop() {
        size_t requestCount = pendingRequests->getSize();

        if (requestCount == 0) return;

        unsigned long ts = millis();

        for (size_t i = 0; i < requestCount; ++i) {
            HttpRequest<TClient>* request = nullptr;
            if (!pendingRequests->get(i, request) || request == nullptr || !request->client->available()) {
                // check if response timed out
                unsigned long requestDurationMs = ts - request->requestStartTS;
                if (requestDurationMs > RESPONSE_TIMEOUT_MS) {
                    releaseClient(request->client);
                    request->client = nullptr;

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
            releaseClient(request->client);
            request->client = nullptr;

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

    Http<TClient>(int maxClients = DEFAULT_MAX_CLIENTS) {
        pendingRequests = new List<HttpRequest<TClient>*>();
        clientPool = new List<TClient*>();
        this->maxClients = maxClients;

        for (int i = 0; i < maxClients; ++i) {
            clientPool->add(new TClient());
        }        
    }
    
    ~Http<TClient>() {
        // cleanup all pending requests
        HttpRequest<TClient>* request;
        while (pendingRequests->getSize() > 0) {
            if (pendingRequests->get(0, request)) {
                releaseClient(request->client);
                delete request;
                pendingRequests->removeAt(0);
            }
        }

        // cleanup client pool
        for (size_t i = 0; i < clientPool->getSize(); ++i) {
            TClient* client;
            if (clientPool->get(i, client) && client) delete client;
        }
        delete pendingRequests;
        delete clientPool;
    }
    
private:
    List<HttpRequest<TClient>*>* pendingRequests;
    List<TClient*>* clientPool;
    int maxClients;

    TClient* acquireClient() {
        if (clientPool->getSize() == 0) return nullptr;
        TClient* client;
        clientPool->get(0, client);
        clientPool->removeAt(0);
        return client;
    }

    void releaseClient(TClient* client) {
        if (!client) return;
        client->stop();
        clientPool->add(client);
    }

    HttpRequstStatus sendRequest(const String& url, RequestCompletedCallback* onRequestCompleted, const char* method) {
        return sendRequest(url, onRequestCompleted, method, nullptr, 0);
    }

    HttpRequstStatus sendRequest(const String& url, RequestCompletedCallback* onRequestCompleted, const char* method, String clientCommands[], int16_t commandCount) {
        ParsedUrl parsedUrl = UrlParsing::parseUrl(url);
        if (parsedUrl.failed) {
            return HttpRequstStatus::Failed_InvalidUrl;
        }

        TClient* client = acquireClient();
        if (!client) {
            return HttpRequstStatus::Failed_TooManyConcurrentRequests;
        }

        // Create and add request to the list
        HttpRequest<TClient>* request = new HttpRequest<TClient>();
        request->requestStartTS = millis();
        request->client = client;
        request->callback = onRequestCompleted;
        pendingRequests->add(request);
        
        if (!request->client->connect(parsedUrl.host.c_str(), parsedUrl.port)) {
            releaseClient(client);
            return HttpRequstStatus::Failed_UnableToConnectToServer;
        }

        String command = String(method) + String(" ") + parsedUrl.path + String(" HTTP/1.1");
        String hostPart = String("Host: ") + getLocalIP();

        request->client->println(command.c_str());
        request->client->println(hostPart.c_str());
        request->client->println("Connection: close");

        if (clientCommands != nullptr) {
            for (int16_t i = 0; i < commandCount; ++i) {
                request->client->println(clientCommands[i].c_str());
            }
        }
        request->client->println();
        return HttpRequstStatus::Sent;
    }

};
