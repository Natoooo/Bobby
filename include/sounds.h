#ifndef SOUNDS_H
#define SOUNDS_H

#include <DFRobotDFPlayerMini.h>

// ═══════════════════════════════════════════════════════════
// 🔊 GESTION DES SONS (DFPLAYER)
// ═══════════════════════════════════════════════════════════

class Sounds {
public:
  Sounds();
  
  // Initialisation
  bool begin();
  
  // Sons
  void playWakeup();
  void playHello();
  void playName();
  void playSleep();
  void playGoodbye();
  
  // Réglages
  void setVolume(int volume);
  
private:
  DFRobotDFPlayerMini player;
};

#endif