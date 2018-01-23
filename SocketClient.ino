/*
 *
 *  Connects to a listening TCP server
 *
 */

#include <ESP8266WiFi.h>

//const char* ssid     = "******************";
//const char* password = "********";
const char* ssid     = "****";
const char* password = "****";

const char* host = "192.168.0.153";

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  if (WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting...\n");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);

  Serial.print("connected to ");
  Serial.print(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8858;
  Serial.print(" on port ");
  Serial.println(httpPort);
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  client.print("client> Hello from client side!\n");
  Serial.print(client.readString());

  unsigned long timeout = millis();
  // Read all the lines of the reply from server and print them to Serial
  while(client.connected()){
    client.print((int) millis()/10000);
    client.print("\n");
    delay(1000);
    if (millis() - timeout > 15000){
      Serial.println("closing connection");
      client.print("client> closing connection");
      client.stop();
    }
  }
}

