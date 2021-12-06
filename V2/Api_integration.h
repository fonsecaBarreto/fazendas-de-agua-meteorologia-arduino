// Estudante: Lucas Fonseca Barreto
// Titulo: Criação do script para integração da estação meteorologia com o serviço web
// Data: 03/11/2021
//.........................................................................................................................

#include<string.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Views.h"

#define WIFI_NAME "Gabriel"
#define WIFI_PASSWORD "2014072276"
#define API_URL "http://fazenda-de-aguas-nodejs.herokuapp.com/api/v1/stations/"
#define ENTRY_POINT "/measurements/json?a="
#define STATION_ID "cf2e1023-87c5-43d7-8bf5-0fdb650de883"
#define AccessToken "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6ImU1MTFkNjgzLTFjNTktNDI3NS1iYzljLWE2MzdiYjIzMjYzOCIsInVzZXJuYW1lIjoidXNlcl9iYXNpYyIsImlhdCI6MTYzNjEyNDc0Mn0.zcCOWZOmnCTPRLPVofR76Tavrh_VrdgCyWx2xEA9pZQ"

int init_integration(){
 
    Serial.println("Conectando ao Wifi");
    
    WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    Serial.print("Connected! IP address:");
    Serial.println(WiFi.localIP());
    
    return 0;
}

int sendMeasurement(MeasurementDTO dto) {

  if (WiFi.status() != WL_CONNECTED) { return 0; }

  WiFiClient client;
  HTTPClient http;

  String json_str = dto.toJson();
  String url = API_URL STATION_ID ENTRY_POINT AccessToken;

  http.begin(client, url); 
  http.addHeader("Content-Type", "application/json");    
           
  Serial.println("\n[HTTP] POST : " + url);
  
  int httpCode = http.POST(json_str);
  Serial.printf("--> code: %d\n\n", httpCode);
  
  if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
  }

  http.end();
 
  return 1;

}
