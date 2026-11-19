// ==========================================
// [WILLIAN] DEFINICAO DOS PINOS
// ==========================================

#define IR_PIN 2
#define PIR_PIN 3

#define BUZZER_PIN 8
#define LED_PIN 9

#define TMP36_PIN A0

// ==========================================
// [WILLIAN] VARIAVEIS DOS SENSORES
// ==========================================

float temperature = 0.0;

bool irDetected = false;
bool motionDetected = false;
bool irSignalReceived = false;