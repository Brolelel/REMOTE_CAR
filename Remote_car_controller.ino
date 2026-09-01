#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int x = analogRead(A0);
  int y = analogRead(A1);

  bool backward = (y > 800);
  bool forward  = (y < 200);
  bool right    = (x > 800);
  bool left     = (x < 200);

  const char* command = "STOP";

  if (forward) {
    command = "FORWARD";
  }
  else if (backward) {
    command = "BACKWARD";
  }
  else if (right) {
    command = "RIGHT";
  }
  else if (left) {
    command = "LEFT";
  }

  Serial.println(command);
  radio.write(command, strlen(command) + 1);

  delay(50);
}





  // const char text[] = "Hello World";
  // Serial.write("Hello World");
  // Serial.write("\n");
  // radio.write(&text, sizeof(text));
  // delay(1000);

