#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>


#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección del LCD y configuración

int UVOUT = A1; //Output from the sensor
int REF_3V3 = A2;

byte Heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.createChar(0, Heart);
  lcd.setCursor(0, 0);
  lcd.print("Datos ambientales");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
}


void loop() {
  int temperature = dht.readTemperature();
  int humidity = dht.readHumidity();
  int soilHumidity = analogRead(A0); // Conexión del higrómetro al pin A0
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);

  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;

  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level

  // Envía los datos a la Raspberry Pi
  Serial.print("TEMP:");
  Serial.println(temperature);
  Serial.print("HUM:");
  Serial.println(humidity);
  Serial.print("HUM_SUELO:");
  Serial.println(soilHumidity);
  Serial.print("UV_INTENSITY:");
  Serial.println(uvIntensity);

  // Actualiza la pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("HT:");
  lcd.print(soilHumidity);
  lcd.print("%");
  lcd.setCursor(8, 0);
  lcd.print("L:");
  lcd.print(uvIntensity);
  lcd.print("UV");
  lcd.setCursor(8, 1);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(14,1);
  lcd.write(0);
  delay(5000); // Espera 5 segundos antes de volver a leer los sensores
}
//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return (runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
