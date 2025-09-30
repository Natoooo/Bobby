
// ═══════════════════════════════════════════════════════════
// 🤖 BOBBY - ROBOT INTERACTIF
// ═══════════════════════════════════════════════════════════
// Fichier principal - Architecture modulaire
// ═══════════════════════════════════════════════════════════

#include <Arduino.h>
#include "config.h"
#include "eyes.h"
#include "movements.h"
#include "sounds.h"
#include "sensors.h"
#include "states.h"

// --- Modules du robot ---
Eyes eyes;
Movements movements;
Sounds sounds;
Sensors sensors;
RobotStates robotStates(eyes, movements, sounds);

// --- Variables globales ---
unsigned long lastDebugTime = 0;

// ═══════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  Serial.println("════════════════════════════════════════");
  Serial.println("🚀 BOBBY - Démarrage du robot...");
  Serial.println("════════════════════════════════════════");
  
  // Initialisation des modules
  Serial.println("\n📦 Initialisation des modules:");
  Serial.println("────────────────────────────────────────");
  
  movements.begin();
  eyes.begin();
  sounds.begin();
  sensors.begin();
  
  Serial.println("────────────────────────────────────────");
  
  // Tests des servos au démarrage
  Serial.println("\n🔧 Tests des servos:");
  Serial.println("────────────────────────────────────────");
  movements.testServos();
  Serial.println("────────────────────────────────────────");
  
  // Initialisation random
  randomSeed(analogRead(0));
  
  Serial.println("\n✅ Robot prêt !");
  Serial.println("════════════════════════════════════════\n");
}

void loop() {
  // Lecture capteur
  //long distance = sensors.readDistanceCM();
  unsigned long now = millis();
  
  // Debug toutes les 2 secondes
  if (now - lastDebugTime > 2000) {
    // Serial.print("📏 Distance: ");
    // Serial.print(distance);
    // Serial.print(" cm | État: ");
    Serial.println(robotStates.isAwake() ? "ÉVEILLÉ" : "ENDORMI");
    lastDebugTime = now;
  }
  
  // --- LOGIQUE PRINCIPALE ---
  // DEBUG - Isoler le problème
  bool shouldWake = robotStates.shouldWakeUp();
  bool presenceDetected = sensors.detectPresence();
  // Serial.print("🔍 shouldWakeUp=");
  // Serial.print(shouldWake);
  // Serial.print(" | detectPresence=");
  // Serial.println(presenceDetected);
    // Détection présence → Réveil
  if (shouldWake && presenceDetected) {
    Serial.println("🎯 PRÉSENCE DÉTECTÉE - Tentative réveil");  // ← AJOUTEZ CETTE LIGNE
    robotStates.wakeUp();
  }
    
  // Si Bobby est éveillé
  if (robotStates.isAwake()) {
    
    // Activité détectée
    if (sensors.detectPresence()) {
      robotStates.resetActivityTimer();
    }
    
    // Animations (yeux + tête)
    robotStates.updateAnimations();
    
    // Clignements des yeux
    eyes.updateBlink();
    
    // Vérifier si doit s'endormir
    if (robotStates.shouldSleep()) {
      robotStates.sleep();
    }
  }
  
  delay(100);
}