// Estudante: Lucas Fonseca Barreto
// Titulo: Controlador de Chuva e Vento
// Data: 14/11/2021
//.........................................................................................................................

/* Constantes */
#define ANEMOMETER_RADIUS 0.09  // 9 CENTIMETROS
#define NUMDIRS 8
#define VOLUME_PLUVIOMETRO 0.33;                  // Volume do pluviometro em ml
const float CIRCUNFERENCIA = (2 * 3.14 * ANEMOMETER_RADIUS);                        //Equivalente em METROS de uma volta do anemômetro
unsigned long adc[NUMDIRS] = {26, 45, 77, 118, 161, 196, 220, 256};
char *strVals[NUMDIRS] = {"W", "NW", "N", "SW", "NE", "S", "SE", "E"};
byte dirOffset = 0;
unsigned int anemometerCounter = 0;               // Acumulador Anemometro
float chuvaAcumuladaMl[2];                        // Chuva Acumulada ( Intervalo, Dia)
volatile unsigned int chuvaAcumulada[2];          // Chuva Acumulada ( Intervalo, Dia )
volatile byte pino_chuva =0;                      // Estado do pino de chuva
bool posicao_inicial = false;                     // Indica que o o pluviometro esta na posição inicial

//Chuva Controller.........................................................................................................

void watchRain() {
    if ((posicao_inicial==false)&&(pino_chuva==LOW)){
      posicao_inicial= true;
      chuvaAcumulada[0]++; chuvaAcumulada[1]++;
    }
    if ((posicao_inicial==true)&&(pino_chuva==HIGH)){
      posicao_inicial= false;
    }
}

void setRainState() { pino_chuva = digitalRead(RAINGAUGE_PIN); }
void init_rainController() {  
  chuvaAcumulada[0] = 0;
  chuvaAcumulada[1] = 0;
  attachInterrupt(digitalPinToInterrupt(RAINGAUGE_PIN), setRainState, CHANGE); 
}

float* getChuvaAcumulada() {
  chuvaAcumuladaMl[0] = chuvaAcumulada[0] * VOLUME_PLUVIOMETRO;
  chuvaAcumuladaMl[1] = chuvaAcumulada[1] * VOLUME_PLUVIOMETRO;
  chuvaAcumulada[0]=0;
  return chuvaAcumuladaMl;
}

//VENTO CONTROLLER..........................................................................................................

void addcount() { anemometerCounter++; }
void init_windController() { // Adiciona um Genrenciador de eventos - Sempre que houve a alteração no pino do anemometro (3) ele ira acumlar uma nova rotação
  attachInterrupt(digitalPinToInterrupt(ANEMOMETER_PIN), addcount, RISING);
}

float getWindSpeed(float intervalo) {                                     // O intervalo da medição em milisegundos
  float distancia_percorrida = CIRCUNFERENCIA * anemometerCounter ;       // Diametro linear vezes quantidade de vezes em que foi rotacionado
  anemometerCounter = 0 ;                                                 // Zera o acumulador
  return distancia_percorrida / ( intervalo / 1000.0f ) ;                 // Distancia obtida pelo intervalo de tempo desejado (m/s)
}

String getWindDir() {
  int val, x, reading;
  val = analogRead(PIN_VANE);
  val >>= 2;                        // Shift to 255 range
  reading = val;
  for (x = 0; x < NUMDIRS; x++) {
    if (adc[x] >= reading)
      break;
  }
  x = (x + dirOffset) % NUMDIRS;   // Adjust for orientation
  return strVals[x];
}
