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

const char *ssid = "F.Lopes";
const char *password = "josefa90";

const int relePin = 2;        // Pino D4
const int minSensorPin = 14;  // Pino D5
const int maxSensorPin = 12;  // Pino D6
int state = 0;


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

// Json Variable to Hold Sensor Readings
JSONVar readings;

String getSensorReadings() {
  readings["temperature"] = String(digitalRead(minSensorPin));
  readings["humidity"] = String(digitalRead(maxSensorPin));
  readings["pressure"] = String("000000");
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    //data[len] = 0;
    //String message = (char*)data;
    // Check if the message is "getReadings"
    //if (strcmp((char*)data, "getReadings") == 0) {
    //if it is, send current sensor readings
    String sensorReadings = getSensorReadings();
    Serial.print(sensorReadings);
    notifyClients(sensorReadings);
    //}
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(9600);

  pinMode(minSensorPin, INPUT_PULLUP);  // Usando resistor pull-up interno
  pinMode(maxSensorPin, INPUT_PULLUP);  // Usando resistor pull-up interno

  digitalWrite(relePin, LOW);  // Começa DESLIGADO
  Serial.println("Teste com NO - Deve LIGAR em LOW");
  pinMode(LED_BUILTIN, OUTPUT);

  startupWifi();
  initWebSocket();

  delay(1000);
}

void loop() {

  // server.handleClient();

  delay(1000);
}
