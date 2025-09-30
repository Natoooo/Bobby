#include "eyes.h"
#include "config.h"
#include <Wire.h>

Eyes::Eyes() 
  : oledLeft(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1),
    oledRight(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1),
    lastBlinkTime(0),
    nextBlinkInterval(0),
    eyesClosed(false) {
}

bool Eyes::begin() {
  Wire.begin();
  
  bool leftOk = oledLeft.begin(SSD1306_SWITCHCAPVCC, OLED_LEFT_ADDRESS);
  bool rightOk = oledRight.begin(SSD1306_SWITCHCAPVCC, OLED_RIGHT_ADDRESS);
  
  if (!leftOk) {
    Serial.println(F("❌ Écran gauche introuvable"));
  } else {
    Serial.println(F("✅ Écran gauche OK"));
  }
  
  if (!rightOk) {
    Serial.println(F("❌ Écran droit introuvable"));
  } else {
    Serial.println(F("✅ Écran droit OK"));
  }
  
  // Yeux fermés au démarrage
  drawClosed();
  
  return leftOk && rightOk;
}

void Eyes::drawEye(Adafruit_SSD1306 &screen, bool open, int offsetX, int offsetY) {
  screen.clearDisplay();
  if (open) {
    screen.fillRoundRect(44 + offsetX, 7 + offsetY, 40, 50, 10, WHITE);
  } else {
    screen.drawLine(44, 32, 84, 32, WHITE);
  }
  screen.display();
}

void Eyes::drawOpen(int offsetX, int offsetY) {
  drawEye(oledLeft, true, offsetX, offsetY);
  drawEye(oledRight, true, offsetX, offsetY);
}

void Eyes::drawClosed() {
  drawEye(oledLeft, false, 0, 0);
  drawEye(oledRight, false, 0, 0);
}

void Eyes::lookLeft() {
  drawOpen(-8, 0);
}

void Eyes::lookRight() {
  drawOpen(8, 0);
}

void Eyes::lookCenter() {
  drawOpen(0, 0);
}

void Eyes::updateBlink() {
  unsigned long now = millis();
  
  // Fermer les yeux
  if (!eyesClosed && now - lastBlinkTime > nextBlinkInterval) {
    drawClosed();
    eyesClosed = true;
    lastBlinkTime = now;
    Serial.println("😉 Clignement");
  }
  
  // Rouvrir les yeux
  if (eyesClosed && now - lastBlinkTime > BLINK_TIME) {
    lookCenter();
    eyesClosed = false;
    lastBlinkTime = now;
    nextBlinkInterval = random(BLINK_MIN_INTERVAL, BLINK_MAX_INTERVAL);
  }
}

void Eyes::resetBlinkTimer() {
  lastBlinkTime = millis();
  nextBlinkInterval = random(BLINK_MIN_INTERVAL, BLINK_MAX_INTERVAL);
  eyesClosed = false;
}