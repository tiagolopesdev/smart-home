// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include "ESP8266HTTPClient.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>

// ESP8266WebServer server(80);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char* ssid = "F.Lopes";
const char* password = "josefa90";


void startupWifi() {

  Serial.println("\n\n=== WiFi Diagnostic ===");

  // Lista redes disponíveis
  Serial.println("Scanning for networks...");
  int numNetworks = WiFi.scanNetworks();

  Serial.print("Found ");
  Serial.print(numNetworks);
  Serial.println(" networks:");

  for (int i = 0; i < numNetworks; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(" dBm) ");
    Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "Open" : "Protected");
  }

  // Tenta conectar
  Serial.println("\nAttempting to connect to: " + String(ssid));

  WiFi.begin(ssid, password);

  unsigned long startTime = millis();
  bool connected = false;

  while (millis() - startTime < 30000) {  // 30 seconds timeout
    int status = WiFi.status();

    Serial.print("Status: ");
    switch (status) {
      case WL_IDLE_STATUS: Serial.println("IDLE"); break;
      case WL_NO_SSID_AVAIL: Serial.println("SSID not found"); break;
      case WL_SCAN_COMPLETED: Serial.println("Scan completed"); break;
      case WL_CONNECTED:
        Serial.println("CONNECTED!");
        connected = true;
        break;
      case WL_CONNECT_FAILED: Serial.println("Connection failed"); break;
      case WL_CONNECTION_LOST: Serial.println("Connection lost"); break;
      case WL_DISCONNECTED: Serial.println("Disconnected"); break;
      default: Serial.println(status); break;
    }

    if (connected) break;
    delay(2000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // server.on("/", handleRoot);
    // server.begin();

  } else {
    Serial.println("\n❌ Failed to connect");
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(9600);

  startupWifi();
  initWebSocket()

  delay(1000);
}

void loop() {

  // server.handleClient();

  delay(1000);
}
