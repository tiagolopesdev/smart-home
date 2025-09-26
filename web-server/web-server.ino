#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ESP8266HTTPClient.h"

ESP8266WebServer server(80);

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

    server.on("/", handleRoot);
    server.begin();

  } else {
    Serial.println("\n❌ Failed to connect");
  }
}

void setup() {
  Serial.begin(9600);

  startupWifi();

  delay(1000);
}

void loop() {

  server.handleClient();

  delay(1000);
}

void handleRoot() {

  Serial.println("Root page requested");

  const char* htmlPage = R"rawliteral(
    <!DOCTYPE HTML>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        body { font-family: Arial; text-align: center; margin: 0 auto; padding: 20px; }
        .button { 
          background-color: %COLOR%; 
          border: none; color: white; 
          padding: 15px 32px; 
          text-decoration: none; 
          font-size: 16px; 
          margin: 4px 2px; 
          cursor: pointer;
          border-radius: 8px;
        }
      </style>
    </head>
    <body>
      <h1>Controle ESP8266</h1>
      <p>Relay 1 - Estado: %STATE%</p>
      <p><button class="button" style="background-color:#008000" onClick={toggleOnButton()}>LIGAR</button></a></p>
      <p><button class="button" style="background-color:#FF0000">DESLIGAR</button></a></p>
    </body>
    </html>
    )rawliteral";


  server.setContentLength(CONTENT_LENGTH_UNKNOWN);

  server.send(200, "text/html", htmlPage);

}  // handleRoot

void toggleOnButton() {
  Serial.println(">>> Active toggleOnButton");
}
