// Estudante: Lucas Fonseca Barreto
// Titulo: Modelo padrão para as medições da estação meteorologica
// Data: 16/11/2021
//.........................................................................................................................

struct MeasurementDTO {
    String date;
    String date_hour;
    float temperature;
    float airHumidity;
    float windSpeed;
    String windDirection;
    float rainVolume;
    float accRainVolume;

    MeasurementDTO(){}
    MeasurementDTO( String* d, float temp, float air, float ws, String wd, float* rain){
      date = d[0];
      date_hour = d[1];
      temperature = temp;
      airHumidity = air;
      windSpeed = ws;
      windDirection = wd;
      rainVolume = rain[0];
      accRainVolume = rain[1];
    }
    
    String toCsv(){

      String csv = "";
      csv+=String(date)+", ";
      csv+=String(date_hour)+", ";
      csv+=String(temperature)+", ";
      csv+=String(airHumidity)+", ";
      csv+=String(windSpeed)+", ";
      csv+=String(windDirection)+", ";
      csv+=String(rainVolume)+", ";
      csv+=String(accRainVolume);
      return csv;

    }

    String toJson(){
      
      int total_params = 7; 
      String medicao[total_params][2] = {
          {"created_at", date+" "+date_hour}, 
          {"temperature", String(temperature)},
          {"airHumidity", String(airHumidity)},
          {"windSpeed", String(windSpeed)},
          {"windDirection", windDirection},
          {"rainVolume", String(rainVolume)},
          {"accRainVolume", String(accRainVolume)},
      };
    
      String json = "{";
      for( int n = 0 ; n < total_params ; n ++){
        json += "\"" + medicao[n][0]+"\": "+"\""+ medicao[n][1] +"\"" ;
        if(n < total_params - 1){
          json+=", ";
        }
      }
    
      json+= "}";
    
      return json;
    }

};
