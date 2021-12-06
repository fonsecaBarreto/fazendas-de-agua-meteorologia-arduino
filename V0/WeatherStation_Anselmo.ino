//#include <DHT.h>
//#include <DHT_U.h>

#include <SD.h>
#include<string.h>
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA
RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231
#define DHT_PIN 2        // TEMPERATURA E UMIDADE
#define DHT_TYPE DHT22
#define PIN_VANE A0       // direção do vento
#define PI_NUMBER 3.14159265
#define ANEMOMETER_RADIUS 147
#define ANEMOMETER_PIN 3  // velocidade 
#define PERIOD 5000
#define RAINGAUGE_PIN 4  // pluviometro
// Variaveis Globais //
bool bucketPositionA = false;             // one of the two positions of tipping-bucket               
const double bucketAmount = 0.33;  // mm equivalente a ml 
double dailyRain = 0.0;                   // rain accumulated for the day
double hourlyRain = 0.0;                  // rain accumulated for one hour
double dailyRain_till_LastHour = 0.0;     // rain accumulated for the day till the last hour          
bool first=true;                               // as we want readings of the (MHz) loops only at the 0th moment 

// ADC readings:
#define NUMDIRS 8
unsigned long   adc[NUMDIRS] = {26, 45, 77, 118, 161, 196, 220, 256};
char *strVals[NUMDIRS] = {"W","NW","N","SW","NE","S","SE","E"};
byte dirOffset=0;

//DHT dht(DHT_PIN, DHT_TYPE);

unsigned int anemometerCounter = 0;
int timeOut = 1000;
String data;
unsigned long lastMillis;

//criar um objeto File para manipular um arquivo
File myFile;
float getTemperature() {
  return 32;
}

float getHumidity() {
  return 5; //dht.readHumidity();
}

int RPM() {
  return ((anemometerCounter) * 60) / (PERIOD / 1000);
}

float WindSpeed() {
  return ((4 * PI_NUMBER * ANEMOMETER_RADIUS * RPM()) / 60) / 1000;
}

void addcount() {
  anemometerCounter++;
}
String calcWindDir() 
{
   int val;
   byte x, reading;
   val = analogRead(PIN_VANE);
   val >>=2;                        // Shift to 255 range
   reading = val;
   // Look the reading up in directions table. Find the first value
   // that's >= to what we got.
   for (x=0; x<NUMDIRS; x++) {
      if (adc[x] >= reading)
         break;
   }
   //Serial.println(reading, DEC);
   x = (x + dirOffset) % 8;   // Adjust for orientation
   return(strVals[x]);
}
String convertFloatToString(float value)
{
  char temp[10];
  String tempAsString;
  dtostrf(value, 1, 2, temp);
  tempAsString = String(temp);
  return tempAsString;
}
void estacao()
{
  DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
  // ++++++++++++++++++++++++ Count the bucket tips ++++++++++++++++++++++++++++++++
  //Serial.println(digitalRead(RAINGAUGE_PIN));
  if ((bucketPositionA==false)&&(digitalRead(RAINGAUGE_PIN)==HIGH)){
    bucketPositionA=true;
    dailyRain+=bucketAmount;                               // update the daily rain
  }
  
  if ((bucketPositionA==true)&&(digitalRead(RAINGAUGE_PIN)==LOW)){
    bucketPositionA=false;  
  } 
  if(now.second() != 0) 
    first = true;                     // depois que o primeiro minuto acabar, pronto para a próxima leitura
  
  if(now.second() == 0 && first == true)
  {
 
      hourlyRain = dailyRain - dailyRain_till_LastHour;      // calcula a chuva do último minuto
      dailyRain_till_LastHour = dailyRain;                   // atualize a chuva até a última hora para o próximo cálculo
      writeDataEvent(hourlyRain, dailyRain_till_LastHour);   // realiza a gravação dos dados através da função writeDataEvent de (1 em 1 minuto)
      first = false;                                        // execute cálculos apenas uma vez por minuto e grava no SD
  }
  
  if(now.hour()== 0) 
  {
    dailyRain = 0.0;          // zerando tudo para o dia seguinte                            
    dailyRain_till_LastHour = 0.0;                        
  } 
  //Serial.print(bucketPositionA); 
}              
void writeDataEvent(double chuva_minuto, double chuva_acumulada) 
{
  DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
  ini_sd();
  Serial.println(convertFloatToString(chuva_minuto));
  data = String(now.day())+"/"+String(now.month())+"/"+String(now.year()) + "," + String(now.hour())+":"+String(now.minute())+":"+String(now.second())+"," + convertFloatToString(getTemperature()) + "," + convertFloatToString(getHumidity()) + "," + convertFloatToString(getWindSpeed()) + "," + calcWindDir() + "," + convertFloatToString(chuva_minuto) + "," + convertFloatToString(chuva_acumulada);
  Serial.println(data);
  myFile = SD.open("estacao.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(data);
    // Terminou de escrever, fecha-se o arquivo:
    myFile.close();
  }
  else {
    // Se nao deu certo
    Serial.println("Nao foi possivel abrir o arquivo");
    //acender led vermelho e piscar 5 vezes 
    //apagar led verde
  }

}
float getWindSpeed() {
  WindVelocity();
  return WindSpeed();
  //delay(2000);
}

void WindVelocity() {
  anemometerCounter = 0;
  attachInterrupt(0, addcount, RISING);
  unsigned long millis();
  long startTime = millis();
  while (millis() < startTime + PERIOD) {}
}
int pinoSS = 10; 
void ini_sd()
{
  if (SD.begin()) 
    { // Inicializa o SD Card
      Serial.println("SD Card pronto para uso..."); // Imprime na tela
      //digitalWrite(7, HIGH);
      //digitalWrite(6, LOW);
    }
    else 
    {
      Serial.println("Falha na inicialização do SD Card.");
      //digitalWrite(6, HIGH);
      //digitalWrite(7, LOW);
      return;
    }
}
void setup() 
{
    String rel; 
   //acender led verde
    //pinMode(7, OUTPUT);
    //pinMode(6, OUTPUT);
    Serial.begin(9600); // Define BaundRate
    pinMode(pinoSS, OUTPUT); // Declara pinoSS como saída
    
    ini_sd();
    
    if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
      Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
      //while(1); //SEMPRE ENTRE NO LOOP
    } 
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    Serial.println("Chamei a função");
    if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
      Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
      //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
      //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
      //rtc.adjust(DateTime(2020, 7, 8, 15, 20, 45)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
    } 
    
   // dht.begin(); // Inicializa o DHT
    pinMode(ANEMOMETER_PIN, INPUT);
    digitalWrite(ANEMOMETER_PIN, HIGH);
    pinMode(RAINGAUGE_PIN, INPUT);
    Serial.println("Chamei a função");
    DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
    data = String(now.day())+"/"+String(now.month())+"/"+String(now.year()) + "," + String(now.hour())+":"+String(now.minute())+":"+String(now.second())+"," + "Temperatura: "+ convertFloatToString(getTemperature()) + "," + "Umidade: "+convertFloatToString(getHumidity());
    Serial.println("Executei a função");
    Serial.println(data);
    /* A biblioteca SD tem um metodo para abrir arquivos e esse arquivo aberto
    sera armazenado no objeto myFile, descrito ao inicio desse codigo. Somente 
    um arquivo pode ser aberto por vez.
    */
    myFile = SD.open("estacao.txt", FILE_WRITE); 
    if (myFile) 
    {
        Serial.println("Arquivo aberto com sucesso!!");
    }
    else 
    {
      // Se nao deu certo
      Serial.println("Nao foi possivel abrir o arquivo");
    }
    lastMillis=millis();
}
void loop() 
{
  DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
  //digitalWrite(7, LOW);
  //Serial.println(String(now.hour())+":"+String(now.minute())+":"+String(now.second()));
  estacao();
  
  

}
