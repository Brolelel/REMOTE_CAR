#define ENA 5   // Left motor speed (PWM)
#define ENB 3   // Right motor speed (PWM)

#define IN1 6   // Left motor direction
#define IN2 7
#define IN3 4   // Right motor direction
#define IN4 2


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  pinMode(button, INPUT_PULLUP);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int x = analogRead(A0);
  int y = analogRead(A1);

  Serial.print("X: "); Serial.print(x);
  Serial.print(" Y: "); Serial.println(y);

  // Dead zone
  bool backward  = (y > 800);
  bool forward = (y < 200);
  bool right    = (x > 800);
  bool left     = (x < 200);

  // STOP everything first
  stopMotors();

  // ===== MOVEMENT LOGIC =====
  if (forward) {
    Serial.println("FORWARD");
    forwardMotors();
  }
  else if (backward) {
    Serial.println("BACKWARD");
    backwardMotors();
  }

  // Turning overrides one side
  if (right) {
    Serial.println("RIGHT");
    rightTurn();
  }
  else if (left) {
    Serial.println("LEFT");
    leftTurn();
  }

  delay(50);
}

// ===== MOTOR FUNCTIONS =====

void forwardMotors() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void backwardMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void rightTurn() {
  digitalWrite(IN1, LOW);   // Left motor OFF
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Right motor forward
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 200);
}

void leftTurn() {
  digitalWrite(IN1, HIGH);  // Left motor forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Right motor OFF
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 200);
  analogWrite(ENB, 0);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}