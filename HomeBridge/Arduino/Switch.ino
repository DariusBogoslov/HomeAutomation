#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);


const char* ssid = "Eng. family";
const char* password = "anemissisuirad";

// following for the name of: http://name_esp.local/ 
const char* dns_name = "ESPSwitch";

//int relay_pin = 5; //D1

void setup()
{
 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, OUTPUT);
  Serial.begin(115200);
  Serial.println("ESP Gestartet");
  digitalWrite(LED_BUILTIN, HIGH);  
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Verbindung wird hergestellt ...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Verbunden! IP-Adresse: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(dns_name)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  
    
 

  server.onNotFound([](){
    server.send(404, "text/plain", "Link wurde nicht gefunden!");  
  });

  server.on("/", []() {
    server.send(200, "text/plain", "ESP-Startseite!");
  });

  server.on("/relay_on", []() {
    server.send(200, "text/plain", "relay on");
    relay_on();
  });

  server.on("/relay_off", []() {
    server.send(200, "text/plain", "relay off");
    relay_off();
  });

  server.on("/state", []() {
    server.send(200, "text/plain", String(digitalRead(LED_BUILTIN)));
  });

  server.begin();
  Serial.println("Webserver gestartet.");

  MDNS.addService("http", "tcp", 80);
}

void loop() {
  MDNS.update();
  server.handleClient();  
}

void relay_on(){
  digitalWrite(LED_BUILTIN, HIGH);  
  digitalWrite(0 , HIGH);
}


void relay_off(){
  digitalWrite(LED_BUILTIN, LOW);  
  digitalWrite(0 , LOW);  
}
