#ifndef MOVEMENTS_H
#define MOVEMENTS_H

#include <Servo.h>

// ═══════════════════════════════════════════════════════════
// 🤖 GESTION DES MOUVEMENTS (TÊTE + BRAS)
// ═══════════════════════════════════════════════════════════

class Movements {
public:
  Movements();
  
  // Initialisation
  void begin();
  void testServos();
  
  // Tête
  void headCenter();
  void headLeft();
  void headRight();
  void headLookLeft();
  void headLookRight();
  
  // Bras droit
  void armRightLift();
  void armRightDown();
  void armRightNeutral();
  
    // Bras gauche
  void armLeftLift();
  void armLeftDown();
  void armLeftNeutral();


private:
  Servo servoHead;
  Servo servoArmR;
  Servo servoArmL; 
};

#endif