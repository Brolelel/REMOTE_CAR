#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CONNECTION 10
#define ENA 5   // Left motor speed (PWM)
#define ENB 3   // Right motor speed (PWM)

#define IN1 6   // Left motor direction
#define IN2 7
#define IN3 4   // Right motor direction
#define IN4 2

unsigned long lastReceivedTime = 0;
const unsigned long timeout = 500; // milliseconds

LiquidCrystal_I2C lcd(0x27, 16, 2);

RF24 radio(8, 9); // CE, CSN

const byte address[6] = "00001";

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Michael's RC Car");

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    lastReceivedTime = millis();  // update connection time

    if (strcmp(text, "FORWARD") == 0){
      forwardMotors();
    } else if (strcmp(text, "BACKWARD") == 0){
      backwardMotors();
    } else if (strcmp(text, "RIGHT") == 0) {
      rightTurn();
    } else if (strcmp(text, "LEFT") == 0){
      leftTurn();
    } else if (strcmp(text, "STOP") == 0) {
      stopMotors();
    }
  }

  // Connection LED logic
  if (millis() - lastReceivedTime < timeout) {
    digitalWrite(CONNECTION, HIGH); // still connected
  } else {
    digitalWrite(CONNECTION, LOW);  // lost connection
  }
}

// ===== MOTOR FUNCTIONS =====

void forwardMotors() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  clearRow(1);
  lcd.setCursor(0,1);
  lcd.print("GO!!!");
}

void backwardMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  clearRow(1);
  lcd.setCursor(0,1);
  lcd.print("REVERSING!!!");
}

void rightTurn() {
  digitalWrite(IN1, LOW);   // Left motor OFF
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Right motor forward
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 200);
  clearRow(1);
  lcd.setCursor(0,1);
  lcd.print("TURN RIGHT!");
}

void leftTurn() {
  digitalWrite(IN1, HIGH);  // Left motor forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Right motor OFF
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 200);
  analogWrite(ENB, 0);
  clearRow(1);
  lcd.setCursor(0,1);
  lcd.print("TURN LEFT!");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  clearRow(1);
}

void clearRow(int row) {
  lcd.setCursor(0,row);
  for (int i=0; i< 16; i++) {
    lcd.print(" ");
  }
}