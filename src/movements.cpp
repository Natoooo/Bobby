#include "movements.h"
#include "config.h"
#include <Arduino.h>

Movements::Movements() {
}

void Movements::begin() {
  servoHead.attach(SERVO_HEAD_PIN);
  servoArmR.attach(SERVO_ARM_R_PIN);
  servoArmL.attach(SERVO_ARM_L_PIN); 
  
  // Position initiale
  headCenter();
  armRightNeutral();
  armLeftNeutral(); 
  
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
  
  Serial.println("🔄 Test servo bras droit...");
  servoArmR.write(ARM_R_NEUTRAL);
  delay(1000);
  servoArmR.write(ARM_R_LIFTED);
  delay(1000);
  servoArmR.write(ARM_R_NEUTRAL);
  delay(1000);
  servoArmR.write(ARM_R_DOWN);
  delay(500);
  Serial.println("✅ Test bras droit terminé");

  Serial.println("🔄 Test servo bras gauche...");
  servoArmL.write(ARM_L_NEUTRAL);
  delay(1000);
  servoArmL.write(ARM_L_LIFTED);
  delay(1000);
  servoArmL.write(ARM_L_NEUTRAL);
  delay(1000);
  servoArmL.write(ARM_L_DOWN);
  delay(500);
  Serial.println("✅ Test bras gaucheterminé");
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
void Movements::armRightLift() {
  Serial.println("🤲 Bras droit levé");
  servoArmR.write(ARM_R_LIFTED);
}

void Movements::armRightDown() {
  Serial.println("🤲 Bras droit baissé");
  servoArmR.write(ARM_R_DOWN);
}

void Movements::armRightNeutral() {
  Serial.println("🤲 Bras droit neutre");
  servoArmR.write(ARM_R_NEUTRAL);
}

// --- BRAS GAUCHE ---
void Movements::armLeftLift() {
  Serial.println("🤲 Bras gauche levé");
  servoArmL.write(ARM_L_LIFTED);
}

void Movements::armLeftDown() {
  Serial.println("🤲 Bras gauche baissé");
  servoArmL.write(ARM_L_DOWN);
}

void Movements::armLeftNeutral() {
  Serial.println("🤲 Bras gauche neutre");
  servoArmL.write(ARM_L_NEUTRAL);
}