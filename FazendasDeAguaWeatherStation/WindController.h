// Estudante: Lucas Fonseca Barreto
// Titulo: Refatoração e documentação do arquivo WeatherStation.ino
// Data: 06/11/2021
//.........................................................................................................................

/* Constantes */
#define NUMDIRS 8
#define ANEMOMETER_RADIUS 147
#define PI_NUMBER 3.14159265
#define PERIOD 5000
unsigned long adc[NUMDIRS] = {26, 45, 77, 118, 161, 196, 220, 256};
char *strVals[NUMDIRS] = {"W","NW","N","SW","NE","S","SE","E"};

/* Variaveis */
byte dirOffset=0;
unsigned int anemometerCounter = 0;

/* Computações */
int RPM() {
  return ((anemometerCounter) * 60) / (PERIOD / 1000);
}

float WindSpeed() {
  return ((4 * PI_NUMBER * ANEMOMETER_RADIUS * RPM()) / 60) / 1000;
}

void addcount() {
  anemometerCounter++;
}

void WindVelocity() {
  anemometerCounter = 0;
  attachInterrupt(0, addcount, RISING);
  unsigned long millis();
  long startTime = millis();
  while (millis() < startTime + PERIOD) {}
}

// GETTERS

float getWindSpeed() {
  WindVelocity();
  return WindSpeed();
}

String getWindDir() {
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
