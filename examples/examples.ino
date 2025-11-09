#include <HttpWiFiNINA.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

HttpWiFiNINA http;

int socketCount = 0;

void setup() {
    Serial.begin(115200);
    connectToWifi();

    // interact with an API
  http.get("http://192.168.1.152:5000/messages", &listMessages);

  JsonDocument newMessage;
  newMessage["title"] = "My message";
  newMessage["text"] = "This is a message from my Arduino!";
  http.post("http://192.168.1.152:5000/messages", newMessage, &onMessageCreated);

  JsonDocument update;
  update["title"] = "My message (updated)";
  update["text"] = "This message was updated.";
  http.put("http://192.168.1.152:5000/messages/1", update, &onMessageUpdated);

  http.del("http://192.168.1.152:5000/messages/1", &onMessageDeleted);
}

void loop() {
  // call the loop() method to process the pending requests
  http.loop();
}

void listMessages(const HttpResponse& response) {
  printResponseStatus(response);
  Serial.println(String("Socket: ") + String(++socketCount));

  // print response content as text
  Serial.println(response.contentText);

  // get as JSON document (uses ArduinoJson library)
  JsonDocument doc;
  response.asJson(doc);
  JsonArray messages = doc.as<JsonArray>();


  for (JsonObject message : messages) {
    String title = message["title"];
    String text = message["text"];
    Serial.println(title + String(": ") + text);
  }
}

void onMessageCreated(const HttpResponse& response) {
  printResponseStatus(response);
  if (response.responseCode == 201) {
    Serial.println("Message created.");
  }
  else {
    Serial.println("Message creation failed.");
  }
}

void onMessageUpdated(const HttpResponse& response) {
  printResponseStatus(response);
  if (response.responseCode == 201) {
    Serial.println("Message updated.");
  }
  else {
    Serial.println("Message update failed.");
  }
}

void onMessageDeleted(const HttpResponse& response) {
  printResponseStatus(response);
  if (response.responseCode == 201) {
    Serial.println("Message delted.");
  }
  else {
    Serial.println("Message deletion failed.");
  }
}

void printResponseStatus(const HttpResponse& response) {
  Serial.print("API response received: ");
  Serial.println(String(response.responseCode));
}

void connectToWifi(){
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    Serial.print(".");
    delay(5000);
  }
  Serial.println("connected");
}

