#ifndef STATES_H
#define STATES_H

#include "eyes.h"
#include "movements.h"
#include "sounds.h"

// ═══════════════════════════════════════════════════════════
// 🎭 GESTION DES ÉTATS DU ROBOT
// ═══════════════════════════════════════════════════════════

class RobotStates {
public:
  RobotStates(Eyes& eyes, Movements& movements, Sounds& sounds);
  
  // États
  void wakeUp();
  void sleep();
  
  // Vérifications
  bool isAwake();
  bool shouldWakeUp();
  bool shouldSleep();
  
  // Animations
  void updateAnimations();
  void resetActivityTimer();
  
private:
//Le & permet à RobotStates de contrôler directement les yeux, 
//mouvements et sons du robot sans faire de copies !
//Ex: Sans le &, ça modifierait une copie inutile et les écrans ne changeraient jamais !
  Eyes& eyes; 
  Movements& movements;
  Sounds& sounds;
  
  bool awake;
  bool sleeping;
  bool atCenter;
  
  unsigned long lastActivityTime;
  unsigned long lastMoveTime;
  unsigned long lastWakeupAttempt;
};

#endif