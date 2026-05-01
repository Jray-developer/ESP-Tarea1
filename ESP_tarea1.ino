#include <ESP8266WiFi.h>          
#include <ESP8266WebServer.h>     

const char* ssid = "POCO X6";
const char* password = "20202020";

ESP8266WebServer server(80);

const int pinLED = 15;    
const int pinBoton = 2;  
bool estadoLED = false;

String paginaHTML() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>Control LED ESP32</title>";
  html += "<style>";
  html += "body { font-family: Arial; text-align:center; margin-top:50px; }";
  html += "button { padding:20px; font-size:20px; }";
  html += "</style></head><body>";
  html += "<h1>Control del LED</h1>";
  html += String("<p>Estado actual: <b>") + (estadoLED ? "ENCENDIDO" : "APAGADO") + "</b></p>";
  html += "<form action='/toggle'><button>Encender / Apagar</button></form>";
  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", paginaHTML());
}

void handleToggle() {
  estadoLED = !estadoLED;
  digitalWrite(pinLED, estadoLED);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  pinMode(pinBoton, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConectado!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP()); 
  
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
}

void loop() {
  server.handleClient(); 
  if (digitalRead(pinBoton) == LOW) {
    delay(500);
    estadoLED = !estadoLED;

    digitalWrite(pinLED, estadoLED);
    Serial.println("Cambio de estado");
  }
}
