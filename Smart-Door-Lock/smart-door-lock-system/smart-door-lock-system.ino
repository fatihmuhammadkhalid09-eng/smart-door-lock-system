#include <Keypad.h>
#include <Stepper.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= SERIAL =================
#define mySerial Serial1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= KEYPAD =================
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ================= PASSWORD =================
const int passwordLength = 6;
char password[passwordLength] = {'1','2','3','4','5','6'};
char enteredPassword[passwordLength];
int passwordIndex = 0;

// ================= MOTOR =================
const int stepsPerRevolution = 200;
Stepper stepperMotor(stepsPerRevolution, 10,11,12,13);

// ================= PIN =================
const int relayPin = 53;
const int buzzerPin = 52;
const int irPin = 50;

// ================= STATUS =================
bool doorOpen = false;

// ================= SETUP =================
void setup() {
  Serial.begin(9600);
  mySerial.begin(57600);

  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(irPin, INPUT_PULLUP);

  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);

  stepperMotor.setSpeed(60);

  lcd.init();
  lcd.backlight();

  lcd.print("Smart Door Lock");
  delay(1500);

  lcd.clear();
  lcd.print("Scan Finger");

  if (!finger.verifyPassword()) {
    lcd.clear();
    lcd.print("Sensor Error");
    while (1);
  }
}

// ================= LOOP =================
void loop() {
  checkIR();
  checkFingerprint();
  checkKeypad();
}

// ================= IR SENSOR =================
void checkIR() {
  static unsigned long lastTime = 0;

  if (digitalRead(irPin) == LOW && millis() - lastTime > 10000) {
    lastTime = millis();

    lcd.clear();
    lcd.print("Orang Terdeteksi");

    buzz(200);

    delay(1000);

    lcd.clear();
    lcd.print("Scan Finger");
  }
}

// ================= KEYPAD =================
void checkKeypad() {
  char key = keypad.getKey();

  if (key) {
    buzz(50);

    lcd.clear();
    lcd.print("Input: ");
    lcd.print(key);

    if (key == 'A') {
      if (passwordIndex == passwordLength && checkPassword()) {
        accessGranted();
      } else {
        accessDenied();
      }
      resetPassword();
    }

    else if (key == 'B') {
      resetPassword();
      lcd.clear();
      lcd.print("Reset");
    }

    else {
      if (passwordIndex < passwordLength) {
        enteredPassword[passwordIndex++] = key;
      }
    }
  }
}

// ================= PASSWORD =================
bool checkPassword() {
  for (int i = 0; i < passwordLength; i++) {
    if (enteredPassword[i] != password[i]) return false;
  }
  return true;
}

void resetPassword() {
  passwordIndex = 0;
  memset(enteredPassword, 0, sizeof(enteredPassword));
}

// ================= FINGERPRINT =================
void checkFingerprint() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return;

  p = finger.fingerSearch();

  if (p == FINGERPRINT_OK) {
    accessGranted();
  }
}

// ================= AKSES =================
void accessGranted() {
  lcd.clear();
  lcd.print("Akses Diterima");

  buzz(100);

  openDoor();

  delay(3000);

  closeDoor();

  lcd.clear();
  lcd.print("Scan Finger");
}

void accessDenied() {
  lcd.clear();
  lcd.print("Akses Ditolak");

  buzz(500);

  delay(1500);

  lcd.clear();
  lcd.print("Scan Finger");
}

// ================= MOTOR =================
void openDoor() {
  if (!doorOpen) {
    digitalWrite(relayPin, HIGH);

    stepperMotor.step(stepsPerRevolution * 2); // buka

    doorOpen = true;
  }
}

void closeDoor() {
  if (doorOpen) {
    stepperMotor.step(-stepsPerRevolution * 2); // tutup

    digitalWrite(relayPin, LOW);

    doorOpen = false;
  }
}

// ================= BUZZER =================
void buzz(int duration) {
  digitalWrite(buzzerPin, HIGH);
  delay(duration);
  digitalWrite(buzzerPin, LOW);
}