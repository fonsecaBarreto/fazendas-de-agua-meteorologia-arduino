// Estudante: Lucas Fonseca Barreto
// Titulo: Refatoração e documentação do arquivo WeatherStation.ino
// Data: 06/11/2021
//.........................................................................................................................

#include <SD.h>
File myFile;

void init_SDWriter() {
     if(!SD.begin()) {
          Serial.println("Falha na inicialização do SD Card.");
          return;     
     }
 
     Serial.println("SD Card pronto para uso..."); 
}


void WriteOnSD(MeasurementDTO: dto) {

     String csv_str = measurement.toCsv();
     
     Serial.println('Pronto cara escrever no cartao SD.');
     Serial.println(csv_str);
     
     myFile = SD.open("estacao.csv", FILE_WRITE);
     if(!file) {
          Serial.println("Nao foi possivel abrir o arquivo");
          return;
     }
 
     myFile.println(data);
     myFile.close();
   
     Serial.println('Salvo com sucesso!');
}
