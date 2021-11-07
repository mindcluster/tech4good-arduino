#include <Adafruit_SSD1306.h>

#define SCREEN_ADDRESS 0x3C

int potentiometerSensor = A2;
int uvSensor = A1;
int tempSensor = A0;

float ph;
float temp;
float uv;

Adafruit_SSD1306 oled(128, 64);

void setup() {
  pinMode(potentiometerSensor, INPUT);
  pinMode(uvSensor, INPUT);
  pinMode(tempSensor, INPUT);

  Serial.begin(9600);

  oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  oled.clearDisplay();
}

void loop() {
  ph = readPh(potentiometerSensor);
  uv = readUv(uvSensor);
  temp = readTemp(tempSensor);
	 
  writeOled("Luminosidade: " + String(uv) + "\nTemperatura: " + String(temp) + "\nPH:" + String(ph));
	
  //delay de 30 segundos entre a captura de informações
  //delay(30000);

  delay(1000);
}

float readPh(int phSensor) {
  //Simula um retorno de valor de PH
  float vo = analogRead(phSensor);
  float phSimulator = map(vo, 0, 1023, 0, 15);
  return phSimulator;
}

float readUv(int uvSensor) {
  //Transforma em um valor de luminosidade mais coerente
  float rdark = 127410.0;
  int r1 = 10000;
  int vo = analogRead(uvSensor);
  float rldr = r1 * (1023.0 / (float)vo - 1.0);
  float lux = pow(rdark / rldr, 1 / 0.8582);
  return lux;
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

void writeOled(String text) {
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(1, 1);
  oled.print(text);
  oled.display();
}

