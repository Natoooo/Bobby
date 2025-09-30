#include "sounds.h"
#include "config.h"
#include <Arduino.h>

Sounds::Sounds() {
}

bool Sounds::begin() {
  Serial1.begin(9600);
  
  Serial.println("→ Initialisation DFPlayer…");
  
  if (!player.begin(Serial1)) {
    Serial.println("❌ DFPlayer non détecté sur Serial1");
    return false;
  }
  
  player.volume(DFPLAYER_VOLUME);
  delay(1000);
  Serial.println("✅ DFPlayer prêt");
  
  return true;
}

void Sounds::playWakeup() {
  Serial.println("🔊 Réveil");
  player.play(TRACK_WAKEUP);
}

void Sounds::playHello() {
  Serial.println("🔊 Bonjour");
  player.play(TRACK_HELLO);
}

void Sounds::playName() {
  Serial.println("🔊 Je m'appelle Bobby");
  player.play(TRACK_NAME);
}

void Sounds::playSleep() {
  Serial.println("🔊 Sommeil");
  player.play(TRACK_SLEEP);
}

void Sounds::playGoodbye() {
  Serial.println("🔊 Au revoir");
  player.play(TRACK_GOODBYE);
}

void Sounds::setVolume(int volume) {
  player.volume(volume);
}