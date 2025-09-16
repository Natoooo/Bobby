#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// --- OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 oledLeft(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 oledRight(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Ultrasonic pins
#define TRIG_PIN 7
#define ECHO_PIN 6   

// --- Servo tête
Servo servoHead;

// DFPlayer
HardwareSerial& dfSerial = Serial1;   // Mega : TX1=18 → DFPlayer RX, RX1=19 ← DFPlayer TX
DFRobotDFPlayerMini myDFPlayer;
//#define DFPLAYER_BUSY_PIN 8    

// --- Seuils
const int distanceSeuil = 30;
const unsigned long tempsInactivite = 60000;   // 1 min
const unsigned long intervalAnimation = 10000;  // yeux + tête toutes les 10 sec
const unsigned long dureeMouvement = 1000;     // restent 1 sec avant retour centre
const unsigned long dureeClignement = 300;     // temps yeux fermés (ms)

// --- États
bool isAwake = false;
bool soundPlaying = false;
bool isSleeping = false; 
unsigned long lastActivityTime = 0;
unsigned long lastMoveTime = 0;
bool atCenter = true;

// --- Clignement
unsigned long lastBlinkTime = 0;
unsigned long nextBlinkInterval = 0;
bool eyesClosed = false;


// ---- Fonctions sons ----
/* void playTrack(int trackNum) {
  if (!myDFPlayer.available()) return;
  myDFPlayer.play(trackNum);
  // attend la fin de lecture
  while (digitalRead(DFPLAYER_BUSY_PIN) == LOW) {
    // tu peux ici animer les yeux pendant que ça joue
    delay(10);
  }
} */

void playWakeupSound()   { Serial.println("🔊 Réveil");     myDFPlayer.play(2); }
void playHelloSound()    { Serial.println("🔊 Bonjour");   myDFPlayer.play(4); }
void playNameSound()     { Serial.println("🔊 Je m'appelle Bobby"); myDFPlayer.play(5); }
void playSleepSound()    { Serial.println("🔊 Sommeil");    myDFPlayer.play(3); }
void playGoodbyeSound()  { Serial.println("🔊 Au revoir");  myDFPlayer.play(6); }

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

void wakeUp() {
  if (isAwake) {
    Serial.println("⚠️  Déjà réveillé !");
    return;  // évite d'appeler plusieurs fois
  }

  Serial.println("🤖 RÉVEIL !");
  
  playWakeupSound();
  drawEye(oledLeft, true, 0, 0);
  drawEye(oledRight, true, 0, 0);

  // Mouvement tête complet
  servoHead.write(45);   // gauche
  delay(700);
  servoHead.write(135);  // droite
  delay(700);
  servoHead.write(90);   // centre
  delay(700);

  isAwake = true;
  isSleeping = false;
  lastActivityTime = millis();
  lastMoveTime = millis();
  atCenter = true;

  playHelloSound();
  // Pause entre les sons
  delay(2000);
  playNameSound();
  delay(2000);

  // Premier clignement dans 3-8 sec
  nextBlinkInterval = random(3000, 8000);
  lastBlinkTime = millis();

  Serial.println("✅ Réveil terminé");
}

void sleep() {
    if (!isAwake || isSleeping) {
    Serial.println("⚠️  Déjà endormi !");
    return;
  }

  Serial.println("😴 SOMMEIL !");
  
  playGoodbyeSound();
  // Attendre que le son se termine complètement
  delay(2000);
  
  drawEye(oledLeft, false);
  drawEye(oledRight, false);
  servoHead.write(90); // tête centrée
  
  isAwake = false;
  isSleeping = true;  
  playSleepSound();
  delay(2000);


  Serial.println("✅ Sommeil terminé");

}

void setup() {
  Serial.begin(115200);
  Serial.println("🚀 Démarrage du robot...");

  // --- Servo
  servoHead.attach(9);
  Serial.println("🔄 Test servo...");
  servoHead.write(0);
  delay(1000);
  servoHead.write(90);
  delay(1000);
  servoHead.write(180);
  delay(1000);
  servoHead.write(90);
  Serial.println("✅ Test servo terminé");

  // --- OLEDs (bus I2C unique SDA=20, SCL=21)
  Wire.begin();
  if (!oledLeft.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("❌ Écran gauche introuvable (0x3C)"));
  } else {
    Serial.println(F("✅ Écran gauche OK"));
  }
  if (!oledRight.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("❌ Écran droit introuvable (0x3D)"));
  } else {
    Serial.println(F("✅ Écran droit OK"));
  }
  
  drawEye(oledLeft, false); 
  drawEye(oledRight, false);

  // DFPlayer sur Serial1
  dfSerial.begin(9600);
  //pinMode(DFPLAYER_BUSY_PIN, INPUT);  // BUSY du module branché ici
  Serial.println("→ Initialisation DFPlayer…");
  if (!myDFPlayer.begin(dfSerial)) {
    Serial.println("❌ DFPlayer non détecté sur Serial1");
  } else {
    myDFPlayer.volume(20);
    delay(1000);           // volume < 30
    Serial.println("✅ DFPlayer prêt");
  }

  // --- Ultrason
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  randomSeed(analogRead(0));
  
  Serial.println("✅ Robot prêt !");
}

void loop() {
  long distance = readDistanceCM();
  unsigned long now = millis();

  // Debug moins verbeux
  static unsigned long lastDebug = 0;
  if (now - lastDebug > 2000) {  // affiche toutes les 2 sec
    Serial.print("📏 Distance: ");
    Serial.print(distance);
    Serial.print(" cm | État: ");
    Serial.println(isAwake ? "ÉVEILLÉ" : "ENDORMI");
    lastDebug = now;
  }

  // --- Réveil - avec délai pour éviter les répétitions
  if (!isAwake && distance > 0 && distance < distanceSeuil) {
    static unsigned long lastWakeupAttempt = 0;
    if (now - lastWakeupAttempt > 5000) {  // Minimum 5 sec entre réveils
      wakeUp();
      lastWakeupAttempt = now;
    }
  }

  // --- Si éveillé
  if (isAwake) {
    if (distance > 0 && distance < distanceSeuil) {
      lastActivityTime = now; // activité détectée
    }

    // Animation yeux + tête toutes les 10 sec
    if (atCenter && now - lastMoveTime > intervalAnimation) {
      static bool left = true;
      if (left) {
        drawEye(oledLeft, true, -8, 0); 
        drawEye(oledRight, true, -8, 0);
        servoHead.write(70); // tête à gauche
        Serial.println("👁️ Regarde à gauche");
      } else {
        drawEye(oledLeft, true, 8, 0); 
        drawEye(oledRight, true, 8, 0);
        servoHead.write(110); // tête à droite
        Serial.println("👁️ Regarde à droite");
      }
      left = !left;
      atCenter = false;
      lastMoveTime = now;
    }

    // Retour yeux + tête au centre après 1 sec
    if (!atCenter && now - lastMoveTime > dureeMouvement) {
      drawEye(oledLeft, true, 0, 0);
      drawEye(oledRight, true, 0, 0);
      servoHead.write(90); // tête centre
      atCenter = true;
      lastMoveTime = now;
      Serial.println("👁️ Retour au centre");
    }

    // Gestion du clignement
    if (!eyesClosed && now - lastBlinkTime > nextBlinkInterval) {
      drawEye(oledLeft, false);
      drawEye(oledRight, false);
      eyesClosed = true;
      lastBlinkTime = now;
      Serial.println("😉 Clignement");
    }

    if (eyesClosed && now - lastBlinkTime > dureeClignement) {
      drawEye(oledLeft, true, 0, 0);
      drawEye(oledRight, true, 0, 0);
      eyesClosed = false;
      lastBlinkTime = now;
      nextBlinkInterval = random(3000, 8000);
    }

    // Sommeil après inactivité
    if (now - lastActivityTime > tempsInactivite) {
      sleep();
    }
  }

  delay(100);
}