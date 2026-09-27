#include <Servo.h>

// ====== PINS ======
#define LM35_PIN A0
const int servoPin   = 9;
const int ledPin     = 5;
const int buzzerPin  = 8;

// ====== OBJETS ======
Servo ventilation;

// ====== PARAMÈTRES ======
const int TEMP_SEUIL = 30;

unsigned long previousMillis = 0;
const long interval = 200;
bool ledState = false;

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  ventilation.attach(servoPin);
  ventilation.write(0);
}

void loop() {

  // ====== LECTURE LM35 ======
  int analogValue = analogRead(LM35_PIN);
  float voltage = analogValue * (5.0 / 1023.0);
  int temperature = voltage * 100;

  // ====== ENVOI VERS LABVIEW ======
  Serial.print("TEMP:");
  Serial.println(temperature);   

  // ====== ALARME TEMPÉRATURE ======
  if (temperature >= TEMP_SEUIL) {
    tone(buzzerPin, 1000);

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }

  } else {
    ventilation.write(0);
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
    ledState = false;
  }

  // ====== COMMANDES DE LABVIEW ======
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == '1') {
      noTone(buzzerPin);
      digitalWrite(ledPin, LOW);
      ledState = false;
    }

    if (input == '2') {
      ventilation.write(180);
      noTone(buzzerPin);
      digitalWrite(ledPin, LOW);
      ledState = false;
    }
  }

  delay(200);
}
