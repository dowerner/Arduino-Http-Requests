# Http Requests

A simple, HTTP request library for Arduino and compatible boards. The main purpose is to send HTTP GET, POST, PUT, and DELETE requests from within Arduino sketches. It currently provides multiple implementations for Ethernet, WiFiNINA, WiFi101, and ESP WiFi (ESP32/ESP8266) clients.

This library was created for simple IoT / Home automation projects and aims to be easy to use. More complex features and custom request headers are not supported at the moment. This was also a learning project for me so don't expect it to be bug free.

## Features

- Simple asynchronous API with callback support
- Supports JSON bodies (via ArduinoJson)
- Multiple backends for different hardware:
  - Ethernet shields (W5100/W5200/W5500)
  - WiFiNINA devices
  - WiFi101 devices
  - ESP32/ESP8266 boards


## Supported Boards / Network Modules

| Implementation  | Required Library | Compatible Boards / Hardware                                    |
|-----------------|------------------|----------------------------------------------------------------|
| `HttpEthernet`  | Ethernet         | Uno, Mega, Leonardo + Ethernet Shield, Arduino Ethernet Rev3   |
| `HttpWiFi101`   | WiFi101          | MKR1000, WiFi 101 Shield                                      |
| `HttpWiFiNINA`  | WiFiNINA         | MKR WiFi 1010, Uno WiFi Rev2, Nano 33 IoT, Nano RP2040 Connect |
| `HttpWifi`      | WiFi (ESP)       | ESP32, ESP8266, and compatible ESP-based boards                |

*Install the networking library relevant to your hardware using the Arduino Library Manager if not already installed.*


## Getting Started

1. **Include** the relevant HTTP implementation:
    ```cpp
    #include <HttpWiFiNINA.h>   // For WiFiNINA boards (e.g., MKR WiFi 1010)
    // OR
    #include <HttpEthernet.h>   // For Ethernet shields
    // OR
    #include <HttpWiFi101.h>    // For WiFi101 boards
    // OR
    #include <HttpWiFi.h>       // For ESP32/ESP8266
    ```

2. **Example**: GET request with simple text API
    ```cpp
    #include <HttpEthernet.h>

    HttpEthernet http;

    void onResponseReceived(HttpResponse& response) {
      if (response.responseCode != 200) {
        Serial.println("An error has occured.");
        return;
      }
      Serial.println(String("API status: ") + response.contentText);
    }

    void setup() {
      Serial.begin(115200);
      // ... connect your network ...
      http.get("http://api.example.local/status", &onResponseReceived);
    }

    void loop() {
      // call the loop() method to process the pending requests
      http.loop();
    }
    ```

3. **Example**: GET request with API that returns JSON data
    ```cpp
    #include <HttpWiFiNINA.h>
    #include <ArduinoJson.h>

    HttpWiFiNINA http;

    void listMessagesFromApi(HttpResponse& response) {
      if (response.responseCode != 200) {
        Serial.println("An error has occured.");
        return;
      }

      // get as JSON document (uses ArduinoJson library)
      JsonDocument doc;
      response.asJson(doc);
      JsonArray messages = doc.as<JsonArray>();
      Serial.println(response.body);

      for (JsonObject message : messages) {
        String title = message["title"];
        String text = message["text"];
        Serial.println(title + String(": ") + text);
      }
    }

    void setup() {
      Serial.begin(115200);
      // ... connect your network ...
      http.get("http://api.example.local/messages", &listMessagesFromApi);
    }

    void loop() {
      // call the loop() method to process the pending requests
      http.loop();
    }
    ```
4. **Example**: POST request:
    ```cpp
    // use the ArduinoJson library
    JsonDocument newMessage;
    newMessage["title"] = "My new message";
    newMessage["text"] = "This message has been created by my Arduino!";
    http.post("http://api.example.local/messages", newMessage, &onMessageCreated);

    // or use a JSON string
    http.post(
        "http://api.example.local/messages", 
        "{ \"title\": \"My new message\", \"text\": \"This is another message.\" }", 
        &onMessageCreated
    );
    ```

5. **See `examples/examples.ino`** for a complete example sketch.

## Dependencies

- [ArduinoJson](https://arduinojson.org/) for easy JSON parsing
- *One of*:
  - [Ethernet](https://www.arduino.cc/en/Reference/Ethernet)
  - [WiFiNINA](https://www.arduino.cc/en/Reference/WiFiNINA)
  - [WiFi101](https://www.arduino.cc/en/Reference/WiFi101)
  - [WiFi](ESP32/ESP8266 core)

Only the library needed for your network hardware must be installed.


## API

Each specialized `Http*` class provides methods for:
- `get(url, callback)`
- `post(url, body, callback)`
- `put(url, body, callback)`
- `del(url, callback)`

All methods accept a callback that receives an `HttpResponse` object.

All requests are non-blocking and polled by calling `http.loop()` in your main loop.


## License

This library is licensed under the MIT License.  
See [LICENSE](LICENSE) for details.
