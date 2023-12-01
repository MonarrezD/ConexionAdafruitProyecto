#include "config.h"

const int pinSensor = 34;  // Utilizando el pin GPIO 34 como ejemplo
const int pinInfrarojo = 33;
AdafruitIO_Feed *voltaje = io.feed("Voltaje");
AdafruitIO_Feed *Infrarojo = io.feed("Infrarojo");


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

  int valorSensor = analogRead(pinSensor);
  float voltajeValue = (valorSensor / 4095.0) * 3.3;  // El ESP32 utiliza una resolución de 12 bits
  
  Serial.print("Valor Analógico: ");
  Serial.println(valorSensor);
  
  Serial.print("Voltaje: ");
  Serial.println(voltajeValue);
  
  voltaje->save(voltajeValue);
  delay(3000);

  int valorInfrarojo = digitalRead(pinInfrarojo);
    if (valorInfrarojo == HIGH){
      Serial.println("inf:1");
      Infrarojo->save(valorInfrarojo);
      delay(3000);
    }else{
      Serial.println("inf:0");
      Infrarojo->save(valorInfrarojo);
      delay(500);
    }
  io.run();
}
