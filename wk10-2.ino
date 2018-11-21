#include "WiFi.h"
#include "ESPAsyncWebServer.h"

const char* ssid = "rrkkk";
const char* password = "kkkkkkkkkkkk";

AsyncWebServer  server(80);
String website;


void setup(void) {
  Serial.begin(115200);

  pinMode(32,INPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  buildWeb();
  server.on("/", handleRoot);
  server.on("/sensors", handleSensorData);
  server.on("/get",  [](AsyncWebServerRequest *request) {
    request->send(200, "text/html",website);
  });


  server.begin();
  Serial.println("HTTP server started");
}
void loop(void) {
}

void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html",website);
}
void handleSensorData(AsyncWebServerRequest *request){
  int sensor;
  sensor=analogRead(32);
  sensor=map(sensor,0,4095,0,255);
  String json="{\"vr\":";
  json+=sensor;
  json+="}";
  request->send(200, "application/json",json);
}

void buildWeb(){
website+="<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <script src=\"https://code.jquery.com/jquery-2.2.4.min.js\"></script> <script>setInterval(requestData, 500); function requestData(){$.get(\"/sensors\") .done(function(data){if(data){$(\"#resis\").text(data.vr);}else{$(\"#resis\").text(\"?\");}}) .fail(function(){console.log(\"fail\");});}</script> <title>VR Reader</title></head><body> <div class=\"container\"style=\"text-align:center;\"> <p style=\"font-size:36px;\">Variable Resistor=<span id=\"resis\"style=\"font-size:36px;\"></span></p></div></body></html>";
}
