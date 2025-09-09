#include <Arduino.h>
#include <Servo.h>

Servo servoTete;  // Création de l’objet servo

void setup() {
  servoTete.attach(9);   // On branche le servo sur la broche D9
  servoTete.write(90);   // Position initiale : milieu
  delay(1000);
}

void loop() {
  servoTete.write(45);   // Tête à gauche
  delay(1000);           // Attendre 1 seconde
  servoTete.write(135);  // Tête à droite
  delay(1000);
  servoTete.write(90);   // Retour au centre
  delay(1000);
}