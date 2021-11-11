// Estudante: Lucas Fonseca Barreto
// Titulo: Refatoração e documentação do arquivo WeatherStation.ino
// Data: 06/11/2021
//.........................................................................................................................

/* Constantes */
const double volume_pluviometro = 0.33;           // Volume do pluviometro em ml 
/* Variaveis */
bool bucketPositionA = false;                     // Indica que o o pluviometro esta na posição inicial 
double chuva_acumulada_dia = 0.0;                 // Chuva acumulada dia
double chuva_acumulada_minutoAterior = 0.0;       // chuva acumulada ate o minuto anterior     

void CalcChuvaAcumulada(DateTime now) {

     if(now.hour() == 0){  // zerando tudo para o dia seguinte                                 
          chuva_acumulada_dia = 0.0;                               
          chuva_acumulada_minutoAterior = 0.0;                        
     }

     if ((bucketPositionA==false)&&(digitalRead(RAINGAUGE_PIN)==HIGH)){
          bucketPositionA= true;
          chuva_acumulada_dia+=volume_pluviometro;                               
     }
     
     if ((bucketPositionA==true)&&(digitalRead(RAINGAUGE_PIN)==LOW)){
          bucketPositionA= false;  
     } 
}

double getChuvaMinuto(){
     double chuva_minuto = chuva_acumulada_dia - chuva_acumulada_minutoAterior;   // calcula a chuva do último minuto
     chuva_acumulada_MinutoAterior = chuva_acumulada_dia;
     return chuva_minuto; 
}


double getChuvaAcumulada(){
     return chuva_acumulada_dia;
}
