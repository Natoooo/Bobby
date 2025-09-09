#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Servo.h>

// --- Buzzer
#define BUZZER_PIN 8   // ⚠️ la Mega n’a pas de pin 27, choisis-en une libre (ici D8)

// --- OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 oledLeft(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 oledRight(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Ultrasonic pins
#define TRIG_PIN 7
#define ECHO_PIN 6   

// --- Servo tête
Servo servoTete;

// --- Seuils
const int distanceSeuil = 30;
const unsigned long tempsInactivite = 60000;   // 1 min
const unsigned long intervalAnimation = 5000;  // yeux + tête toutes les 5 sec
const unsigned long dureeMouvement = 1000;     // restent 1 sec avant retour centre
const unsigned long dureeClignement = 300;     // temps yeux fermés (ms)

// --- États
bool isAwake = false;
unsigned long lastActivityTime = 0;
unsigned long lastMoveTime = 0;
bool atCenter = true;

// --- Clignement
unsigned long lastBlinkTime = 0;
unsigned long nextBlinkInterval = 0;
bool eyesClosed = false;

void playWakeupSound() {
  int melody[] = { 900, 1200, 1050, 1300, 1000, 800, 950 };
  int duration[] = { 80, 60, 70, 50, 90, 100, 60 };

  for (int i = 0; i < 7; i++) {
    tone(BUZZER_PIN, melody[i], duration[i]);
    delay(duration[i] + 20);
  }
  noTone(BUZZER_PIN);
}

void playSleepSound() {
  int melody[] = { 1175, 1047, 988, 880 };
  int duration[] = { 150, 150, 200, 300 };

  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, melody[i], duration[i]);
    delay(duration[i] + 30);
  }
  noTone(BUZZER_PIN);
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  if (duration == 0) return -1;
  long distance = duration * 0.034 / 2;
  return distance;
}

void drawEye(Adafruit_SSD1306 &screen, bool open, int offsetX = 0, int offsetY = 0) {
  screen.clearDisplay();
  if (open) {
    screen.fillRoundRect(44 + offsetX, 7 + offsetY, 40, 50, 10, WHITE);
  } else {
    screen.drawLine(44, 32, 84, 32, WHITE);
  }
  screen.display();
}

void reveil() {
  playWakeupSound();
  drawEye(oledLeft, true, 0, 0);
  drawEye(oledRight, true, 0, 0);

  // Mouvement tête complet
  servoTete.write(45);   // gauche
  delay(700);
  servoTete.write(135);  // droite
  delay(700);
  servoTete.write(90);   // centre
  delay(700);

  isAwake = true;
  lastActivityTime = millis();
  lastMoveTime = millis();
  atCenter = true;

  // Premier clignement dans 3-8 sec
  nextBlinkInterval = random(3000, 8000);
  lastBlinkTime = millis();
}

void sommeil() {
  drawEye(oledLeft, false);
  drawEye(oledRight, false);
  playSleepSound();
  servoTete.write(90); // tête centrée
  isAwake = false;
}

void setup() {
  Serial.begin(115200);

  // --- Servo
  servoTete.attach(9);
  servoTete.write(90);

  // --- OLEDs (bus I2C unique SDA=20, SCL=21)
  Wire.begin();
  if (!oledLeft.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Écran gauche introuvable (0x3C)"));
  }
  if (!oledRight.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("Écran droit introuvable (0x3D)"));
  }
  drawEye(oledLeft, false); 
  drawEye(oledRight, false);

  // --- Ultrason
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // --- Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  randomSeed(analogRead(0));
}

void loop() {
  long distance = readDistanceCM();
  unsigned long now = millis();

  Serial.print("Distance: ");
  Serial.println(distance);

  // --- Réveil
  if (!isAwake && distance > 0 && distance < distanceSeuil) {
    reveil();
  }

  // --- Si éveillé
  if (isAwake) {
    if (distance > 0 && distance < distanceSeuil) {
      lastActivityTime = now; // activité détectée
    }

    // Animation yeux + tête toutes les 5 sec
    if (atCenter && now - lastMoveTime > intervalAnimation) {
      static bool left = true;
      if (left) {
        drawEye(oledLeft, true, -8, 0); 
        drawEye(oledRight, true, -8, 0);
        servoTete.write(70); // tête à gauche
      } else {
        drawEye(oledLeft, true, 8, 0); 
        drawEye(oledRight, true, 8, 0);
        servoTete.write(110); // tête à droite
      }
      left = !left;
      atCenter = false;
      lastMoveTime = now;
    }

    // Retour yeux + tête au centre après 1 sec
    if (!atCenter && now - lastMoveTime > dureeMouvement) {
      drawEye(oledLeft, true, 0, 0);
      drawEye(oledRight, true, 0, 0);
      servoTete.write(90); // tête centre
      atCenter = true;
      lastMoveTime = now;
    }

    // Gestion du clignement
    if (!eyesClosed && now - lastBlinkTime > nextBlinkInterval) {
      drawEye(oledLeft, false);
      drawEye(oledRight, false);
      eyesClosed = true;
      lastBlinkTime = now;
    }

    if (eyesClosed && now - lastBlinkTime > dureeClignement) {
      drawEye(oledLeft, true, 0, 0);
      drawEye(oledRight, true, 0, 0);
      eyesClosed = false;
      lastBlinkTime = now;
      nextBlinkInterval = random(3000, 8000); // prochain clignement aléatoire
    }

    // Sommeil après inactivité
    if (now - lastActivityTime > tempsInactivite) {
      sommeil();
    }
  }

  delay(100);
}