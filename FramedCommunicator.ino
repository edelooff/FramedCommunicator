#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

const int frameSize = 8;
byte frame[frameSize];
const uint64_t thisAddress = 0xF0F0F0F0D2LL;
const uint64_t thatAddress = 0xF0F0F0F0E1LL;
RF24 radio(9,10);

void setup() {
  // Set up the radio
  radio.begin();
  radio.setPayloadSize(frameSize);
  radio.setRetries(1, 2);
  radio.openWritingPipe(thisAddress);
  radio.openReadingPipe(1, thatAddress);

  // Set up the serial connection.
  Serial.begin(9600);
  Serial.println("[FramedCommunicator]");
}

void loop() {
  // Read frames from serial input and prints their info.
  byte messageLength = readMessage(frame);
  if (messageLength) {
    printMessageInfo(messageLength);
    transmitMessage(messageLength);
  }
}

void transmitMessage(byte length) {
  while (length < frameSize)
    frame[length++] = 0x20; // pad with spaces
  radio.write(&frame, frameSize);
}

void printMessageInfo(byte messageLength) {
  // Prints the length and contents of each constructed frame.
  Serial.print(messageLength);
  Serial.print(": '");
  for (byte n = 0; n < messageLength; ++n)
    Serial.print((char) frame[n]);
  Serial.println();
}

byte readMessage(byte messageBuffer[]) {
  // Reads up to one message into the buffer and returns the length.
  byte serialInput, messageLength = 0;
  while (messageLength < frameSize && readByte(serialInput)) {
    *messageBuffer++ = serialInput;
    messageLength++;
  }
  return messageLength;
}

bool readByte(byte &receivedByte) {
  // Read a single byte from serial or time out; return success state.
  const int readTimeout = 250;
  long startTime = millis();
  while (!Serial.available())
    if ((millis() - startTime) > readTimeout)
      return false;
  receivedByte = Serial.read();
  return true;
}
