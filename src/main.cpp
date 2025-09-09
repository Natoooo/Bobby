#include <Arduino.h>
#include <Servo.h>

Servo servoTete;

// Pins capteur ultrason
const int trigPin = 7;
const int echoPin = 6;

// Seuil de déclenchement
const int distanceSeuil = 30;

// Pour éviter les déclenchements en boucle
bool dejaDetecte = false;

void setup() {
  Serial.begin(9600);

  servoTete.attach(9);
  servoTete.write(90); // tête centrée

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long lireDistance() {
  // Pulse ultrason
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duree = pulseIn(echoPin, HIGH, 20000); // timeout 20ms (~3,5m max)
  if (duree == 0) return -1; // pas de mesure valide

  long distance = duree * 0.034 / 2;
  return distance;
}

long moyenneDistance(int mesures) {
  long somme = 0;
  int valides = 0;
  for (int i = 0; i < mesures; i++) {
    long d = lireDistance();
    if (d > 0) { // ignorer les -1 (pas de mesure)
      somme += d;
      valides++;
    }
    delay(50);
  }
  if (valides == 0) return -1;
  return somme / valides;
}

void loop() {
  long distance = moyenneDistance(5); // moyenne sur 5 mesures
  Serial.print("Distance moyenne: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < distanceSeuil && !dejaDetecte) {
    // Mouvement une seule fois quand on s'approche
    servoTete.write(45);   // gauche
    delay(700);
    servoTete.write(135);  // droite
    delay(700);
    servoTete.write(90);   // centre
    delay(700);

    dejaDetecte = true; // éviter de répéter tant que tu restes devant
  }
  else if (distance >= distanceSeuil || distance == -1) {
    // Si on s'éloigne → réarmer
    dejaDetecte = false;
  }

  delay(200);
}