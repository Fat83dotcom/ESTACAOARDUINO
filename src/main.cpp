#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include "classesdefiltro.h"

#define SCREEN_WIDTH 128 // OLED display width(largura), in pixels
#define SCREEN_HEIGHT 64 // OLED display height(altura), in pixels
#define LED 13
#define SENSOR10_K 0
#define TEMPO_IN 100
#define TEMPO_OUT 470

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;
Temporarios t;
FiltraNaN fNaN;

void setup() {
  
  pinMode(LED, OUTPUT);
  pinMode(SENSOR10_K, INPUT);
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  bme.begin(0x76);
  display.clearDisplay();
  delay(10);
}

double getTemp() {
  
  int RawADC = analogRead(SENSOR10_K);
  long Resistance;
  double Temp;
  Resistance=((10240000/RawADC) - 10000);
  Temp = log(Resistance);
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius
  return Temp;  
}

void run() {

  static float mediaUmi;
  static float mediaTemp;
  static float mediaPress;
  static float media10k;
  static unsigned long ultimaLeitura0 = millis();
  static unsigned long ultimaLeitura1 = millis();
  static unsigned long ultimaLeitura2 = millis();
  static unsigned long ultimaLeitura3 = millis();
  static unsigned long ultimaLeitura4 = millis();
  static float soma10k, somaUmi, somaTemp, somaPress;
  
  static int cont = 0;
  static int minuto, hora, dias;
  minuto = hora = dias = 0;
  int divisor = 100;
  minuto = millis() / 60000;
  hora = minuto / 60;
  dias = hora / 24;
  
  if (cont < divisor) {

    somaUmi += fNaN.umi_NaN(bme.readHumidity(), t.pt_U);
    somaTemp += fNaN.temp_Nan(bme.readTemperature(), t.pt_T);
    somaPress += fNaN.press_Nan(bme.readPressure() / 100.0F, t.pt_P);
    soma10k += fNaN.t10k_Nan(getTemp(), t.pt_10);
    cont++;
    digitalWrite(LED, 0);
  }
  else {
    
    mediaUmi = somaUmi / divisor;
    mediaPress = somaPress / divisor;
    mediaTemp = somaTemp / divisor;
    media10k = soma10k / divisor;
    cont = 0;
    somaUmi = somaTemp = somaPress = soma10k = 0;
    digitalWrite(LED, 1);
  }

  if ((millis() - ultimaLeitura0) < 495) {

    display.invertDisplay(true);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(2, 5);
    display.println("ESTACAO METEREOLOGICA");
    display.setCursor(14, 17);
    display.print("Umidade: ");
    display.print(mediaUmi);
    display.println(" %");
    display.setCursor(14, 27);
    display.print("Press: ");
    display.print(mediaPress);
    display.println(" hPa");
    display.setCursor(14, 37);
    display.print("TempInt: ");
    display.print(mediaTemp);
    display.println(" C");
    display.setCursor(14, 47);
    display.print("TempExt: ");
    display.print(media10k);
    display.println(" C");
    display.setCursor(2, 56);
    display.print("D: ");
    display.print(dias);
    display.print(",H: ");
    display.print(hora);
    display.print(",M: ");
    display.println(minuto);
    display.setCursor(116, 56);
    display.print(cont);
    display.display();
    display.clearDisplay();
  }
  else {
    ;
  }

  if ((millis() - ultimaLeitura0) > 500) {

    ultimaLeitura0 = millis();
  }
  
  if ((millis() - ultimaLeitura1) < TEMPO_IN) {

    Serial.print("u ");
    Serial.println(mediaUmi, DEC);
  }
  if ((millis() - ultimaLeitura1) > TEMPO_OUT) {

    ultimaLeitura1 = millis();
  }

  if ((millis() - ultimaLeitura2) < TEMPO_IN) {
   
    Serial.print("1 ");
    Serial.println(mediaTemp, DEC); 
  }
  if ((millis() - ultimaLeitura2) > TEMPO_OUT) {

    ultimaLeitura2 = millis();
  }

  if ((millis() - ultimaLeitura3) < TEMPO_IN) {

    Serial.print("p ");
    Serial.println(mediaPress, DEC);
  }

  if ((millis() - ultimaLeitura3) > TEMPO_OUT) {

    ultimaLeitura3 = millis();
  }

  if ((millis() - ultimaLeitura4) < TEMPO_IN) {

    Serial.print("2 ");
    Serial.println(media10k, DEC);
  }

  if ((millis() - ultimaLeitura4) > TEMPO_OUT) {

    ultimaLeitura4 = millis();
  }
}

void loop() {
  run();
}