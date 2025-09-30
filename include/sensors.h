#ifndef SENSORS_H
#define SENSORS_H

// ═══════════════════════════════════════════════════════════
// 📡 GESTION DES CAPTEURS
// ═══════════════════════════════════════════════════════════

class Sensors {
public:
  Sensors();
  
  // Initialisation
  void begin();
  
  // Lecture
  long readDistanceCM();
  bool detectPresence();
  
private:
  // Variables internes si nécessaire
};

#endif