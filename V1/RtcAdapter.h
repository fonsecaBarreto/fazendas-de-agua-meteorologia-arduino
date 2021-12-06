// Estudante: Lucas Fonseca Barreto
// Titulo: Refatoração e documentação do arquivo WeatherStation.ino
// Data: 06/11/2021
//.........................................................................................................................

#include "RTClib.h" 
RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231
String date[2];

void init_Rtc(){

     if(! rtc.begin()) { 
          Serial.println("DS3231 não encontrado"); 
          return;
     } 

     //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
     
     if(rtc.lostPower()){    //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA 
          Serial.println("DS3231 OK!");
          //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
          //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
          //rtc.adjust(DateTime(2020, 7, 8, 15, 20, 45)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
     }

     return;
}

String* getDate(DateTime now){

     date[0] = String(now.day())+"/"+String(now.month())+"/"+String(now.year());
     data[1] = String(now.hour())+":"+String(now.minute())+":"+String(now.second());

     return date;

}
    
