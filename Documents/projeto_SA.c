#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ==========================================
// WAREHOUSE MONITORING AND SAFETY SYSTEM
// Autores: Willian & Richard
// ==========================================

// ==========================================
// DEFINICAO DOS PINOS (CORRIGIDO CONFORME HARDWARE)
// ==========================================

#define PIR_PIN     10
#define BUZZER_PIN  8
#define LED_PIN     9

// ==========================================
// CONFIGURACAO DO LCD
// ==========================================

#define LCD_ADDRESS 0x27
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

// ==========================================
// CONFIGURACOES DO SISTEMA
// ==========================================

const unsigned long LCD_INTERVAL = 250;
const unsigned long SERIAL_INTERVAL = 1000;
const int BUZZER_FREQUENCY = 1000;
const bool LED_ACTIVE_LOW = false;

// ==========================================
// VARIAVEIS GLOBAIS
// ==========================================

bool motionDetected = false;
unsigned long lastLCDUpdate = 0;
unsigned long lastSerialUpdate = 0;
int previousSystemState = -1;

// ==========================================
// ESTADOS DO SISTEMA
// ==========================================

#define SYSTEM_READY     0
#define SYSTEM_PIR_ALERT 1

// ==========================================
// CONTROLE DO LED
// ==========================================

void setLED(bool state)
{
  if (state) {
    digitalWrite(LED_PIN, LED_ACTIVE_LOW ? LOW : HIGH);
  } else {
    digitalWrite(LED_PIN, LED_ACTIVE_LOW ? HIGH : LOW);
  }
}

// ==========================================
// LEITURA DOS SENSORES
// ==========================================

void readSensors()
{
  motionDetected = (digitalRead(PIR_PIN) == HIGH);
}

// ==========================================
// CONTROLE DOS ATUADORES
// ==========================================

void updateOutputs()
{
  int systemState = motionDetected ? SYSTEM_PIR_ALERT : SYSTEM_READY;

  if (systemState == previousSystemState) return;

  previousSystemState = systemState;

  switch (systemState)
  {
    case SYSTEM_PIR_ALERT:
      setLED(true);
      tone(BUZZER_PIN, BUZZER_FREQUENCY);
      break;

    case SYSTEM_READY:
      setLED(false);
      noTone(BUZZER_PIN);
      break;
  }
}

// ==========================================
// ATUALIZACAO DO LCD
// ==========================================

void updateLCD()
{
  lcd.setCursor(0, 0);
  lcd.print("PIR: ");
  lcd.print(motionDetected ? "MOTION     " : "NO MOTION  ");

  lcd.setCursor(0, 1);
  lcd.print(motionDetected ? "ALERTA!        " : "Sistema OK     ");
}

// ==========================================
// MONITOR SERIAL
// ==========================================

void updateSerial()
{
  Serial.println("==============================");
  Serial.print("PIR: ");
  Serial.println(motionDetected ? "MOTION DETECTED" : "NO MOTION");
  Serial.print("PIR DIGITAL: ");
  Serial.println(digitalRead(PIR_PIN));
  Serial.print("SYSTEM: ");
  Serial.println(motionDetected ? "MOTION ALERT" : "READY");
  Serial.println("==============================");
}

// ==========================================
// SETUP
// ==========================================

void setup()
{
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  setLED(false);
  noTone(BUZZER_PIN);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("WAREHOUSE SYSTEM");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  Serial.println("WAREHOUSE SYSTEM - INITIALIZING...");
  delay(1500);
  lcd.clear();

  lastLCDUpdate = millis();
  lastSerialUpdate = millis();
  Serial.println("SYSTEM INITIALIZED");
}

// ==========================================
// LOOP PRINCIPAL
// ==========================================

void loop()
{
  unsigned long currentTime = millis();

  readSensors();
  updateOutputs();

  if (currentTime - lastLCDUpdate >= LCD_INTERVAL) {
    updateLCD();
    lastLCDUpdate = currentTime;
  }

  if (currentTime - lastSerialUpdate >= SERIAL_INTERVAL) {
    updateSerial();
    lastSerialUpdate = currentTime;
  }
}