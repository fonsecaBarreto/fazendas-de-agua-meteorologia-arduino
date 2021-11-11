// Estudante: Lucas Fonseca Barreto
// Titulo: Refatoração e documentação do arquivo WeatherStation.ino
// Data: 06/11/2021
//.........................................................................................................................

/* PINOS */
#define PIN_VANE A1                                   // direção do vento
#define ANEMOMETER_PIN 3                              // anemômetro 
#define RAINGAUGE_PIN 4                               // Pino pluviometro
#define DHT_PIN 6                                     // Pino DHT  
#define DHT_TYPE DHT11                                // Tipo do sensor
/* Constantes */
#define PINO_SD = 10;                                 // Pino Saida
/* Includes */
#include <string.h>                                   // Inclui biblioteca de String
#include "RtcAdapter.h"                               // Inclui Adapter para bliblioteca RTClib
#include "RainController.h"                           // Inclui Controlador Pluviometro
#include "DhtController.h"                            // Inclui Controlador DHT ( Temperatura , umidade do ar )
#include "View.h"                                     // Inclui Modelo padrão de Medição
#include "Api_integration.h"                          // Inclui Integração com o serviço web

void setup() 
{
  // Define os Pinos ( Nao tenho certeza, Consultar a equipe // Lucas Fonseca )...........................................
  pinMode(PINO_SD, OUTPUT);                           // Declara PINO_SAIDA como saída ( SD )
  pinMode(RAINGAUGE_PIN, INPUT);                      // Declara Pino do pluviometro
  pinMode(ANEMOMETER_PIN, INPUT);                     // Declara Pino do anemômetro
  digitalWrite(ANEMOMETER_PIN, HIGH);                 // ??

  //Iniciar todas as dependencias.........................................................................................
  Serial.begin(9600);                                 // Arduino Serial monitor
  init_Rtc();                                         // inicia biblioteca para captação de data/hora
  init_SDWriter();                                    // Inicio biblioteca para escrever em cartão SD
  init_Dht();                                         // inicia biblioteca Dht para medição da temperatura, e umidade do at
  init_integration();                                 // integração com a aplicação Web 
}

void loop() {
  //Iniciar Loop Principal...............................................................................................
  DateTime now = rtc.now();                           // Captura a data atual
  CalcChuvaAcumulada(now);                            // Calcula a chuva acumulada a cada ciclo 
  if(now.second() != 59) return;                      // enterrompe a execução caso não tenha passado um minuto.

  String* date = getDate(now);                        // RCT - Data / Hora ( Recece um array de string com ambas informações)
  float temperature = getTemperature();               // DHT - Obtem Temperature 
  float airHumidity = getHumidity();                  // DHT - Obtem Umidade do ar
  float windSpeed = getWindSpeed();                   // Obtem Velocidade do Vento
  String windDir = getWindDir();                      // Obtem do Direção do Vento
  double chuva_minuto = getChuva();                   // Retona chuva acumulada no ultimo minuto
  double chuva_acumulada = getChuvaAcumulada();       // Retona chuva acumulada do dia

  /* gera um instância da struct 'Measurement' com a medições obtidas a cima */
  MeasurementDTO measurement = MeasurementDTO(data[0], data[1],temperature, airHumidity, windSpeed, windDir, chuva_minuto, chuva_acumulada);

  enviar_cartaoSD(measurement);                       // realiza a gravação dos dados através da função writeDataEvent de (1 em 1 minuto)
  enviar_webIntergration(measurement);                // Enviar a informações para serem salvas em banco de dados via Internet

  //Fim
}
