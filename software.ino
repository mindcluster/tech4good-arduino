#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#define SCREEN_ADDRESS 0x3C

const unsigned short tempSensor = A0;
const unsigned short lumSensor = A1;
const unsigned short potentiometerSensor = A2;
const unsigned short led = 8;

const Adafruit_SSD1306 screen(128, 64);

const unsigned short jsonLenght = 50;

void setup() {
  pinMode(tempSensor, INPUT);
  pinMode(lumSensor, INPUT);
  pinMode(potentiometerSensor, INPUT);
  pinMode(led, OUTPUT);

  screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  screen.clearDisplay();

  Serial.begin(9600);
}

void loop() {
  const float temp = readTemp(tempSensor);
  const float ph = readPh(potentiometerSensor);
  const float lum = readLum(lumSensor);
  
  writeScreen("Luminosidade: " + String(lum) + "\nTemperatura: " + String(temp) + "\nPH:" + String(ph));

  StaticJsonDocument<jsonLenght> json;
  json["phSensor"] = ph;
  json["lumSensor"] = lum;
  json["tempSensor"] = temp;

  serializeJson(json, Serial);
  Serial.println();

  verifyAlert();
	 
  //delay de 30 segundos entre a captura de informações
  delay(30000);
}

float readPh(int phSensor) {
  //Simula um retorno de valor de PH
  float vo = analogRead(phSensor);
  float phSimulator = map(vo, 0, 1023, 0, 15);
  return phSimulator;
}

float readLum(int uvSensor) {
  //Transforma em um valor de luminosidade mais coerente
  float rdark = 127410.0;
  int r1 = 10000;
  int vo = analogRead(uvSensor);
  float rldr = r1 * (1023.0 / (float)vo - 1.0);
  float lum = pow(rdark / rldr, 1 / 0.8582);
  return lum;
}

float readTemp(int tempSensor) {
  //Transforma em graus Celsius
  float c1 = 1.009249522e-03;
  float c2 = 2.378405444e-04;
  float c3 = 2.019202697e-07;
  int vo = analogRead(tempSensor);
  int r1 = 10000;
  int r2 = r1 * (1023.0 / (float)vo - 1.0);
  float logR2 = log(r2);
  float t = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  float tc = t - 273.15;
  return tc;
}

void writeScreen(String text) {
  screen.clearDisplay();
  screen.setTextSize(1);
  screen.setTextColor(SSD1306_WHITE);
  screen.setCursor(1, 1);
  screen.print(text);
  screen.display();
}

void verifyAlert() {
  String response = Serial.readStringUntil("\n");
  if(response.length() > 0) {
    digitalWrite(led, HIGH); 
  }
  else {
    digitalWrite(led, LOW); 
  }
}

