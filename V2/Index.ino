// Aluno: Lucas Fonseca Barreto
// Titulo: Refatoração e documentação do arquivo WeatherStation.ino
// Data: 16/11/2021
//.........................................................................................................................

/*Pinos*/
#define PIN_VANE A1                                   // Pino biruta
#define RAINGAUGE_PIN 2                               // Pino pluviometro
#define ANEMOMETER_PIN 3                              // Pino anemômetro ( Velocidade do vento )
#define PINO_SD 4                                     // Pino Saida Cartão SD
#define DHT_PIN 6                                     // Pino DHT  
/*Constantes */
#define DHT_TYPE DHT11                                // Tipo do sensor
#define PERIOD 2000                                   // Intervalo de Tempo entre medições
/*Includes*/
#include <string.h>                                   // Inclui biblioteca de String
#include "Libs.h"                                     // Inclui Adaptadores para blibliotecas RTClib e DHT
#include "Controllers.h"                              // Inclui Controlador Pluviometro
#include "View.h"                                    // Inclui Modelo padrão de Medição
#include "Repository.h"                               // Cartão SD Repositorio 

unsigned long tempo_inicial, agora = millis();

void setup(){                                         // Configuração inicial
  Serial.begin(9600);  
  Serial.println("\n > Iniciando Configuração inicial da estação meteorologica...");

  pinMode(PINO_SD, OUTPUT);                           // Declara PINO_SAIDA como saída ( SD )
  pinMode(ANEMOMETER_PIN, INPUT_PULLUP);              // Declara Pino do anemômetro
  pinMode(RAINGAUGE_PIN, INPUT_PULLUP);               // Declara Pino do pluviometro

  init_Rtc();                                         // inicia biblioteca RTC 
  init_SD();                                          // Inicia biblioteca SD 
  init_Dht();                                         // Inicia biblioteca DHT
  init_rainController();                              // Inicia Controlador do pluviometro
  init_windController();                              // Inicio Controlador da biruta e anemômetro

  Serial.println("...................................................................\n");
}

void loop() {                                         // Loop Principal
  agora = millis();                                   // temporizador de execução
  watchRain();                                        // Vigia o estado do pluviometro; Adicionando ao contador em cada alteração.
  
  if((agora - tempo_inicial) < PERIOD) return;        // interrompe a execução caso não tenha passado o intervalo de 1 minuto 
     
  struct MeasurementDTO measurement = MeasurementDTO( // Gera um instância da struct 'Measurement' com a medições obtidas a cima 
    getDate_str(),                                    // RCT - Data / Hora ( Recece um array de string com ambas informações)
    getTemperature(),                                 // DHT - Obtem Temperature 
    getHumidity(),                                    // DHT - Obtem Umidade do ar
    getWindSpeed(PERIOD),                             // Obtem Velocidade do Vento
    getWindDir(),                                     // Obtem do Direção do Vento
    getChuvaAcumulada());                             // Retona chuva acumulada no ultimo minuto
  tempo_inicial = agora;                              // Tara o temporizador;
  
  submit_sd(&measurement);                            // Realiza a gravação dos dados através da função writeDataEvent de (1 em 1 minuto)
  
  if(ehOutroDia()) { init_rainController(); }         // Reinicia o Acumulador de Chuva
 
}
