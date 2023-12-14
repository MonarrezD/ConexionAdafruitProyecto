#include "config.h"
#include "DHT.h"

const int pinInfrarojo = 33;
const int pinDHT = 14;
const int pinVoltaje = 32;
const int pinLDR = 34;
const int pinmag = 26;
const int pinmagana = 35;
const int Sensores = 7;
const int pinsLeds [] = {12 ,13};
const int voltajeMax = 5000; 
int lecturaDigital; 
float voltaje;
int temperaturamax;
String Id[Sensores] = {"TE", "HU", "FO", "VO", "IN", "MA", "TX"};
AdafruitIO_Feed *Feeds[Sensores] = {nullptr};


void asignarFeeds() {
  Feeds[0] = io.feed("Habitacion1_Temperatura");
  Feeds[1] = io.feed("Habitacion1_Humedad");
  Feeds[2] = io.feed("Habitacion1_Luz");
  Feeds[3] = io.feed("Habitacion1_Voltaje");
  Feeds[4] = io.feed("Habitacion1_Infrarojo");
  Feeds[5] = io.feed("Habitacion1_Magnetico");
}

AdafruitIO_Feed *temperaturamaximafeed = io.feed("Habitación1_Temperaturamax");


void imprimirError(int indice, int valor) {
  Serial.print(Id[indice] + "-Error: ");
  Serial.println(valor);
}


void imprimirVoltaje(float v){
Serial.println(Id[3] + ":01:" + v);
delay(2000);
}

void recibirtemperaturamax(AdafruitIO_Data *data)
{
  temperaturamax = data->toInt();
}





DHT dht(pinDHT, DHT11);
int umbral = 500;


void setup() {
  Serial.begin(115200);
  pinMode(pinInfrarojo, INPUT);
  pinMode(pinmag, INPUT);
  pinMode(pinLDR, INPUT);
  Serial.print("Conectando a Adafruit IO");
  io.connect();
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  for (int i = 0; i < sizeof(pinsLeds) / sizeof(pinsLeds[0]); i++) {
    pinMode(pinsLeds[i], OUTPUT);
  }
  temperaturamaximafeed->onMessage(recibirtemperaturamax);
  Serial.println();
  Serial.println(io.statusText());
  dht.begin();
  asignarFeeds();
}


void loop() {

  // Sensor Magnetico
  int valor = digitalRead(pinmag);
  int valorana = analogRead(pinmagana);
  if (valor == LOW) {
    Serial.println(Id[5] + ":01:1");
    Feeds[5]->save(1);
    delay(2000);
  } else {
    Serial.println(Id[5] + ":01:0");
    Feeds[5]->save(0);
    delay(2000);
  }


  // Sensor Infrarojo
  int valorInfrarojo = digitalRead(pinInfrarojo);
  if (valorInfrarojo == HIGH) {
    Serial.println(Id[4] + ":01:1" );
    Feeds[4]->save(valorInfrarojo);
    delay(3000);
  } else {
    Serial.println(Id[4] + ":01:0" );
    Feeds[4]->save(valorInfrarojo);
    delay(5000);
  }


  // Sensor Humedad/Temperatura
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  if (!isnan(temperatura) && !isnan(humedad)) {
    Serial.println(Id[0] + ":01:" + String(temperatura));
    Serial.println(Id[1] + ":01:" + String(humedad));
    Feeds[0]->save(temperatura);
    Feeds[1]->save(humedad);
    delay(2000);
  } else {
    Serial.println("Revisar La Conexion");
  }

  // Obtener valor del feed Habitacion1_TemperaturaMax
  Serial.println("tmax:"+ String(temperaturamax));
  if (temperatura > temperaturamax) {
    digitalWrite(pinsLeds[0], HIGH);
    digitalWrite(pinsLeds[1], HIGH);
  } else {
    digitalWrite(pinsLeds[0], LOW);
    digitalWrite(pinsLeds[1], LOW);
  }


  // Sensor Voltaje
  lecturaDigital = analogRead(pinVoltaje);
  voltaje = map(lecturaDigital, 0, 1023, 0, voltajeMax) / 1000.0;  
  //voltaje = (lecturaDigital / 1024) * 5.0;

  imprimirVoltaje (voltaje);
  Feeds[3]->save(voltaje);
  delay (1000);

  // Sensor Luz
  int valorLDR = digitalRead(pinLDR);
  if (valorLDR == 1) {
    Serial.println(Id[2] + ":01:0");
    Feeds[2]->save(0);
    delay(3000);
  } else {
    Serial.println(Id[2] + ":01:1");
    Feeds[2]->save(1);
    delay(5000);
  }
  // Ejecuta Adafruit IO
  io.run();
}