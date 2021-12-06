// Estudante: Lucas Fonseca Barreto
// Titulo: Blibliotecas DHT e RtCLib
// Data: 16/11/2021
//.........................................................................................................................
#include <DHT.h>
#include <DHT_U.h>
#include <RTClib.h>

DHT dht(DHT_PIN, DHT_TYPE);
RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231
DateTime now;
String date[2];

// DHT...............................................................

int init_Dht(){  dht.begin(); } // Inicializa o DHT 
float getTemperature() {  return dht.readTemperature(); }
float getHumidity() { return dht.readHumidity(); }

// RTC ........................................................

void init_Rtc(){
    Serial.print(" -> RTC-DS3231: ");

    if (! rtc.begin()) {
        Serial.println("Não encontrado!");
        Serial.flush();
        while (1) delay(10);
    }
    
    Serial.println("Iniciado com sucesso!");
    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    if(rtc.lostPower()){                            //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA 
      Serial.println("Bateria Fraca.");
      rtc.adjust(DateTime(2021, 11, 12, 15, 48, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)   
      //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    }

}

String* getDate_str(){
  now = rtc.now(); 
  date[0] = String(now.day())+"/"+String(now.month())+"/"+String(now.year());
  date[1] = String(now.hour())+":"+String(now.minute())+":"+String(now.second());
  return date;
}

bool ehOutroDia(){
    if(now.hour() == 0 && now.minute() == 00 && now.second() == 0){
      return true;
    }
    return false;
}

    
