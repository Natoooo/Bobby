#include "sensors.h"
#include "config.h"
#include <Arduino.h>

Sensors::Sensors() {
}

void Sensors::begin() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.println("✅ Capteur ultrason initialisé");
}

long Sensors::readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  if (duration == 0) return -1;
  
  long distance = duration * 0.034 / 2;
  return distance;
}

bool Sensors::detectPresence() {
  long distance = readDistanceCM();
  return (distance > 0 && distance < DISTANCE_SEUIL);
}