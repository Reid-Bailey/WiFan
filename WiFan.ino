//Made by Reid Bailey using https://github.com/esp8266/Arduino/tree/master/doc/esp8266wifi
//
//For use with ESP-01, HW-305 ESP-01 USB Serial Adapter (programmer) and ESP-01S Relay V1.0 (single Relay Module):
//  Ensure IO0 pin is grounded before uploading.
//  Ensure EN pin is pulled up (connected to 3.3V pin) before use.
//  Tools Settings:
//    Board:"Generic ESP8266 Module"
//    Builtin LED: "2"
//    Upload Speed: "115200"
//    CPU Frequency: "80MHz"
//    Crystal Frequency: "26Mhz"
//    Flash Size: "512KB (FS:64KB OTA:~214KB)"
//    Flash Mode: "DIO"
//    Flash Frequency: : "40MHz"
//    Reset Method: "no dtr (aka ck)"
//    Debug Port: "Disabled"
//    Debug Level: "None"
//    IwIP Variant: "v2 Lower Memory"
//    VTables: "Flash"
//    Exceptions: "Legacy (new can return nullptr)
//    Erase Flash: "Only Sketch"
//    Espressif FW: "nonos-sdk 2.2.1+100 (190703)"
//    SSL Support: "All SSL ciphers (most compatible)"
//    Port: "COM 8" ##NOTE: YOURS MAY NOT BE##
//
//  Compiling successfully with Arduino IDE v1.8.11 and esp8266 by ESP8266 community v2.6.3

#include <ESP8266WiFi.h>

const char* WiFi_hostname = "WiFan";
const char* ssid = "DWR-921-07CC";
const char* password = "______";
 
int fanPin = 0;

WiFiServer server(80);
 
void setup() {
  Serial.begin(9600);
  delay(10);
 
  pinMode(fanPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(fanPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.hostname(WiFi_hostname);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = HIGH;
  if (request.indexOf("/FAN=ON") != -1)  {
    digitalWrite(fanPin, HIGH);
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (LOW on ESP-01)
    value = HIGH;
  }
  if (request.indexOf("/FAN=OFF") != -1)  {
    digitalWrite(fanPin, LOW);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off (HIGH on ESP-01)
    value = LOW;
  }
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");
  client.println("body {background-color: black;}");
  client.println("body {color: white;}");
  client.println("body {zoom: 9;}");
  client.println("body {font-family: 'Courier New', Monospace;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
 
  client.print("Fan is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/FAN=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/FAN=OFF\"\"><button>Off </button></a><br />");  
  client.println("</body>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
