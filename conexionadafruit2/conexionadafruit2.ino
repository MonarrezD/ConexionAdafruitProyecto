#include "config.h"

const int pinSensor = 34;
const int pinInfrarojo = 33;
const int pinLDR = 32;
const int umbralLuz = 500;  // Ajusta este valor según sea necesario
AdafruitIO_Feed *voltaje = io.feed("Voltaje");
AdafruitIO_Feed *Infrarojo = io.feed("Infrarojo");
AdafruitIO_Feed *Luz = io.feed("Luz");

void setup() {
  Serial.begin(115200);
  pinMode(pinInfrarojo, INPUT);
  Serial.print("Conectando a Adafruit IO");
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  // Lectura del sensor de luz (fotorresistencia)
  int valorLDR = analogRead(pinLDR);

  // Determinar si hay suficiente luz
  int luzStatus = (valorLDR > umbralLuz) ? 1 : 0;
  Serial.print("SL:");
  Serial.println(luzStatus);

  // Envío del estado de luz al feed correspondiente
  Luz->save(luzStatus);
  delay(3000);

  // Lectura del sensor de voltaje
  int valorSensor = analogRead(pinSensor);
  float voltajeValue = (valorSensor / 4095.0) * 3.3;
  Serial.print("SV:");
  Serial.println(voltajeValue);

  voltaje->save(voltajeValue);
  delay(3000);

  // Lectura del sensor infrarrojo
  int valorInfrarojo = digitalRead(pinInfrarojo);
  if (valorInfrarojo == HIGH) {
    Serial.println("inf:1");
    Infrarojo->save(valorInfrarojo);
    delay(3000);
  } else {
    Serial.println("inf:0");
    Infrarojo->save(valorInfrarojo);
    delay(500);
  }

  io.run();
}
