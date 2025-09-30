#include "states.h"
#include "config.h"
#include <Arduino.h>

RobotStates::RobotStates(Eyes& e, Movements& m, Sounds& s)
  : eyes(e), movements(m), sounds(s),
    awake(false), sleeping(false), atCenter(true),
    lastActivityTime(0), lastMoveTime(0), lastWakeupAttempt(0) {
}

void RobotStates::wakeUp() {
  Serial.println("DEBUG: Entrée dans wakeUp()");

  if (awake) {
    Serial.println("⚠️  Déjà réveillé !");
    return;
  }

  //Serial.println("DEBUG: Vérification délai..."); 
  
  // Vérifier délai minimum entre réveils
  unsigned long now = millis();
  if (now - lastWakeupAttempt < WAKEUP_DELAY) {
    Serial.println("DEBUG: Trop tôt, sortie");
    return;
  }
  lastWakeupAttempt = now;

  Serial.println("🤖 RÉVEIL !");
  Serial.println("DEBUG: Lecture du son réveil...");
  
  // Son de réveil
  sounds.playWakeup();
  delay(1000);

  Serial.println("DEBUG: Ouverture yeux...");
  
  // Ouvrir les yeux
  eyes.drawOpen(0, 0);
  
  // Mouvement tête
  movements.headLeft();
  delay(700);
  movements.headRight();
  delay(700);
  movements.headCenter();
  delay(700);

  // Mise à jour états
  awake = true;
  sleeping = false;
  lastActivityTime = millis();
  lastMoveTime = millis();
  atCenter = true;

  // Sons de présentation
  sounds.playHello();
  delay(500);
  // Geste du bras
  movements.armLift();
  delay(500);
  movements.armNeutral();
  delay(500);
  sounds.playName();
  delay(500);
  movements.armDown();
  delay(500);

  // Préparer clignements
  eyes.resetBlinkTimer();

  Serial.println("✅ Réveil terminé");
}

void RobotStates::sleep() {
  if (!awake || sleeping) {
    Serial.println("⚠️  Déjà endormi !");
    return;
  }

  Serial.println("😴 SOMMEIL !");

  // Bras se baisse
  movements.armDown();
  delay(500);
  movements.armLift();
  delay(500);
  movements.armDown();

  // Son d'au revoir
  sounds.playGoodbye();
  delay(1000);
  
  // Fermer yeux et centrer tête
  eyes.drawClosed();
  movements.headCenter();
  
  // Mise à jour états
  awake = false;
  sleeping = true;
  
  // Son de sommeil
  sounds.playSleep();
  delay(1000);

  Serial.println("✅ Sommeil terminé");
}

bool RobotStates::isAwake() {
  return awake;
}

bool RobotStates::shouldWakeUp() {
  return !awake;
}

bool RobotStates::shouldSleep() {
  unsigned long now = millis();
  return (now - lastActivityTime > INACTIVATE_TIME);
}

void RobotStates::updateAnimations() {
  unsigned long now = millis();
  
  // Animation yeux + tête toutes les 10 sec
  if (atCenter && now - lastMoveTime > INTERVAL_ANIMATION) {
    static bool left = true;
    
    if (left) {
      eyes.lookLeft();
      movements.headLookLeft();
      Serial.println("👁️ Regarde à gauche");
    } else {
      eyes.lookRight();
      movements.headLookRight();
      Serial.println("👁️ Regarde à droite");
    }
    
    left = !left;
    atCenter = false;
    lastMoveTime = now;
  }

  // Retour au centre après 1 sec
  if (!atCenter && now - lastMoveTime > MOVE_TIMING) {
    eyes.lookCenter();
    movements.headCenter();
    atCenter = true;
    lastMoveTime = now;
    Serial.println("👁️ Retour au centre");
  }
}

void RobotStates::resetActivityTimer() {
  lastActivityTime = millis();
}