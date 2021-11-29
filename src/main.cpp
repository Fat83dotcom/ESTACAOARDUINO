#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include "myClassAndFunctions.h"

const int led = 13;
const int sensorK10 = 0;
const int tempoIn = 100;
const int tempoOut = 470;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // Oled display width(largura), in pixels
#define SCREEN_HEIGHT 64 // Oled display height(altura), in pixels
#define Oled_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, Oled_RESET);
Adafruit_BME280 bme;
Temporarios t;
FiltraNaN filtroNaN;

void setup() {
  
  pinMode(led, OUTPUT);
  pinMode(sensorK10, INPUT);
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  bme.begin(0x76);
  display.clearDisplay();
  delay(10);
}

double getTemp(int sensor);

void run() {

  static double mediaUmi;
  static double mediaTemp;
  static double mediaPress;
  static double media10k;
  static unsigned long ultimaLeitura0 = millis();
  static unsigned long ultimaLeitura1 = millis();
  static unsigned long ultimaLeitura2 = millis();
  static unsigned long ultimaLeitura3 = millis();
  static unsigned long ultimaLeitura4 = millis();
  static double soma10k, somaUmi, somaTemp, somaPress;
  
  static int cont = 0;
  static int minuto, hora, dias;
  minuto = hora = dias = 0;
  int divisor = 100;
  minuto = millis() / 60000;
  hora = minuto / 60;
  dias = hora / 24;
  
  if (cont < divisor) {

    somaUmi += filtroNaN.umi_NaN(bme.readHumidity(), t.pt_U);
    somaTemp += filtroNaN.temp_Nan(bme.readTemperature(), t.pt_T);
    somaPress += filtroNaN.press_Nan(bme.readPressure() / 100.0F, t.pt_P);
    soma10k += filtroNaN.t10k_Nan(getTemp(sensorK10), t.pt_10);
    cont++;
    digitalWrite(led, 0);
  }
  else {
    
    mediaUmi = somaUmi / divisor;
    mediaPress = somaPress / divisor;
    mediaTemp = somaTemp / divisor;
    media10k = soma10k / divisor;
    cont = 0;
    somaUmi = somaTemp = somaPress = soma10k = 0;
    digitalWrite(led, 1);
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
  
  if ((millis() - ultimaLeitura1) < tempoIn) {

    Serial.print("u ");
    Serial.println(mediaUmi, DEC);
  }
  if ((millis() - ultimaLeitura1) > tempoOut) {

    ultimaLeitura1 = millis();
  }

  if ((millis() - ultimaLeitura2) < tempoIn) {
   
    Serial.print("1 ");
    Serial.println(mediaTemp, DEC); 
  }
  if ((millis() - ultimaLeitura2) > tempoOut) {

    ultimaLeitura2 = millis();
  }

  if ((millis() - ultimaLeitura3) < tempoIn) {

    Serial.print("p ");
    Serial.println(mediaPress, DEC);
  }

  if ((millis() - ultimaLeitura3) > tempoOut) {

    ultimaLeitura3 = millis();
  }

  if ((millis() - ultimaLeitura4) < tempoIn) {

    Serial.print("2 ");
    Serial.println(media10k, DEC);
  }

  if ((millis() - ultimaLeitura4) > tempoOut) {

    ultimaLeitura4 = millis();
  }
}

void loop() {
  run();
}