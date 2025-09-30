#ifndef CONFIG_H // "Si CONFIG_H n'est PAS défini"
#define CONFIG_H // "Alors définis CONFIG_H"

// ═══════════════════════════════════════════════════════════
// 📌 CONFIGURATION DU ROBOT BOBBY
// ═══════════════════════════════════════════════════════════

// --- PINS ---
// Capteur ultrason
#define TRIG_PIN 7
#define ECHO_PIN 6

// Servos
#define SERVO_HEAD_PIN 9
#define SERVO_ARM_R_PIN 8

// --- ÉCRANS OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_LEFT_ADDRESS 0x3C
#define OLED_RIGHT_ADDRESS 0x3D

// --- SEUILS ET TIMINGS ---
#define DISTANCE_SEUIL 35             // Distance de détection (cm)
#define INACTIVATE_TIME 30000          // Temps avant sommeil (30 sec)
#define INTERVAL_ANIMATION 10000       // Intervalle animations yeux/tête (10 sec)
#define MOVE_TIMING 1000               // Durée avant retour centre (1 sec)
#define BLINK_TIME 300                 // Durée yeux fermés (ms)
#define WAKEUP_DELAY 5000              // Délai min entre 2 réveils (5 sec)

// --- ANGLES SERVOS ---
// Tête
#define HEAD_CENTER 90
#define HEAD_LEFT 45
#define HEAD_RIGHT 135
#define HEAD_LOOK_LEFT 70
#define HEAD_LOOK_RIGHT 110

// Bras droit
#define ARM_DOWN 135     // Bras baissé
#define ARM_NEUTRAL 155  // Position repos
#define ARM_LIFTED 180   // Bras levé (ajustez entre 90-110)

// --- DFPLAYER ---
#define DFPLAYER_VOLUME 20

// Numéros des pistes audio
#define TRACK_WAKEUP 2
#define TRACK_SLEEP 3
#define TRACK_HELLO 4
#define TRACK_NAME 5
#define TRACK_GOODBYE 6

// --- CLIGNEMENTS ---
#define BLINK_MIN_INTERVAL 3000  // Min entre 2 clignements (3 sec)
#define BLINK_MAX_INTERVAL 8000  // Max entre 2 clignements (8 sec)

#endif