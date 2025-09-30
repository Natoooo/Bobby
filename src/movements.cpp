#include "movements.h"
#include "config.h"
#include <Arduino.h>

Movements::Movements() {
}

void Movements::begin() {
  servoHead.attach(SERVO_HEAD_PIN);
  servoArmR.attach(SERVO_ARM_R_PIN);
  
  // Position initiale
  headCenter();
  armNeutral();
  
  Serial.println("✅ Servos initialisés");
}

void Movements::testServos() {
  Serial.println("🔄 Test servo tête...");
  servoHead.write(HEAD_LEFT);
  delay(1000);
  servoHead.write(HEAD_CENTER);
  delay(1000);
  servoHead.write(HEAD_RIGHT);
  delay(1000);
  servoHead.write(HEAD_CENTER);
  Serial.println("✅ Test tête terminé");
  
  Serial.println("🔄 Test servo bras...");
  servoArmR.write(ARM_DOWN);
  delay(1000);
  servoArmR.write(ARM_NEUTRAL);
  delay(1000);
  servoArmR.write(ARM_LIFTED);
  delay(1000);
  servoArmR.write(ARM_NEUTRAL);
  delay(1000);
  servoArmR.write(ARM_DOWN);
  delay(500);
  Serial.println("✅ Test bras terminé");
}

// --- TÊTE ---
void Movements::headCenter() {
  servoHead.write(HEAD_CENTER);
}

void Movements::headLeft() {
  servoHead.write(HEAD_LEFT);
}

void Movements::headRight() {
  servoHead.write(HEAD_RIGHT);
}

void Movements::headLookLeft() {
  servoHead.write(HEAD_LOOK_LEFT);
}

void Movements::headLookRight() {
  servoHead.write(HEAD_LOOK_RIGHT);
}

// --- BRAS DROIT ---
void Movements::armLift() {
  Serial.println("🤲 Bras droit levé");
  servoArmR.write(ARM_LIFTED);
}

void Movements::armDown() {
  Serial.println("🤲 Bras droit baissé");
  servoArmR.write(ARM_DOWN);
}

void Movements::armNeutral() {
  Serial.println("🤲 Bras droit neutre");
  servoArmR.write(ARM_NEUTRAL);
}