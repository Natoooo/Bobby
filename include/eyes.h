#ifndef EYES_H
#define EYES_H

#include <Adafruit_SSD1306.h>

// ═══════════════════════════════════════════════════════════
// 👁️ GESTION DES YEUX OLED
// ═══════════════════════════════════════════════════════════

class Eyes {
public:
  Eyes();
  
  // Initialisation
  bool begin();
  
  // Affichage
  void drawOpen(int offsetX = 0, int offsetY = 0);
  void drawClosed();
  void lookLeft();
  void lookRight();
  void lookCenter();
  
  // Clignements
  void updateBlink();
  void resetBlinkTimer();
  
private:
  Adafruit_SSD1306 oledLeft;
  Adafruit_SSD1306 oledRight;
  
  unsigned long lastBlinkTime;
  unsigned long nextBlinkInterval;
  bool eyesClosed;
  
  void drawEye(Adafruit_SSD1306 &screen, bool open, int offsetX, int offsetY);
};

#endif