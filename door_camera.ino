#include <ESP8266WiFi.h> // for wifi functionality
#include <WiFiClient.h> // send and get request to web browser
#include <ESP8266WebServer.h> // handles http protocol (see: https://links2004.github.io/Arduino/d3/d58/class_e_s_p8266_web_server.html)

// Replace with your network credentials
const char* ssid     = "network_name";
const char* password = "password";
char* door_status = "close";
char webpage[256];

// WiFiServer server(80);
ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

void generateWebpage(){
    //Build wepage
  strcpy(webpage,  "<!DOCTYPE html>");
  strcat(webpage,  "<html>");
  strcat(webpage,  "<head>");
  strcat(webpage,  "<body>");
  strcat(webpage,  "<meta charset=\"utf-8\">");
  strcat(webpage,  "<title> Lugano door</title>");
  strcat(webpage, "<h1 style= \"color:blue\"> ESP8266 wifi IP camera to control my home door.</h1>");
  strcat(webpage, "<h1> The door is: ");
  strcat(webpage, door_status);
  strcat(webpage, "</h1>");
  strcat(webpage, "</body>");
  strcat(webpage, "</html>");
 }

void setup() {
  Serial.begin(115200);
  pinMode(D7, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(D4, INPUT);     // Initialize the LED_BUILTIN pin as an output

  generateWebpage();
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("...connecting...");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();

}

// the loop function runs over and over again forever
void loop() {

  server.handleClient();
  
  // Door control
  if (digitalRead(D4) == LOW){
    digitalWrite(D7, HIGH);
    strcpy(door_status, "open");
    Serial.println(door_status);
  }
  else{
    digitalWrite(D7, LOW);
    strcpy(door_status, "close");
  }
  generateWebpage();

}
