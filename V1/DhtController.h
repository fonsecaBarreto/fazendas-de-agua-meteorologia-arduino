// Estudante: Lucas Fonseca Barreto
// Titulo: Refatoração e documentação do arquivo WeatherStation.ino
// Data: 06/11/2021
//.........................................................................................................................

#include <DHT.h>
#include <DHT_U.h>

DHT dht(DHT_PIN, DHT_TYPE);

int init_Dht(){
     dht.begin(); // Inicializa o DHT
}

float getTemperature() {
  return dht.readTemperature();
}

float getHumidity() {
  return dht.readHumidity();
}
