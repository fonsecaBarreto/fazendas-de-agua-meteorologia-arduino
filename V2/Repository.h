// Estudante: Lucas Fonseca Barreto
// Titulo: Refatoração e documentação do arquivo WeatherStation.ino
// Data: 01/11/2021
//.........................................................................................................................

#include <SPI.h>
#include <SD.h>       
File myFile;

void init_SD() {
     Serial.print(" -> SD: ");
     
     if(!SD.begin(PINO_SD)) { 
          Serial.println("Falha na inicialização.");
          return;     
     }
     Serial.println("Iniciado com sucesso!"); 
}

void submit_sd(struct MeasurementDTO *dto) {

     String csv_str = dto->toCsv();
     Serial.println(csv_str);
     myFile = SD.open("estacao.csv", FILE_WRITE);
     myFile.println(csv_str);
     myFile.close();
     Serial.println("Salvo com sucesso!");
}
