const byte frameSize = 16;
byte frame[frameSize];

void setup() {
  // Set up the serial connection.
  Serial.begin(9600);
}

void loop() {
  // Read frames from serial input and prints their info.
  byte messageLength = readMessage(frame);
  if (messageLength)
    printMessageInfo(messageLength);
}

void printMessageInfo(byte messageLength) {
  // Prints the length and contents of each constructed frame.
  Serial.print(messageLength);
  Serial.print(": '");
  for (byte n = 0; n < messageLength; ++n)
    Serial.print((char) frame[n]);
  Serial.println("'");
}

byte readMessage(byte messageBuffer[]) {
  // Reads up to one message into the buffer and returns the length.
  byte serialInput, messageLength = 0;
  while (messageLength < frameSize
         && readByte(serialInput)
         && serialInput != '\n') {
    *messageBuffer++ = serialInput;
    messageLength++;
  }
  return messageLength;
}

bool readByte(byte &receivedByte) {
  // Read a single byte from serial or time out; return success state.
  const int readTimeout = 20;
  long startTime = millis();
  while (!Serial.available())
    if ((millis() - startTime) > readTimeout)
      return false;
  receivedByte = Serial.read();
  return true;
}
