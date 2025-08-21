#include <RH_ASK.h>
#include <SPI.h>

// RH_ASK(bitRate, rxPin, txPin, pttPin, pttInverted)
// rxPin = GPIO4 (D2), prędkość taka sama jak w TX: 2000 bps
RH_ASK driver(2000, 4, -1, -1, false);

void setup() {
  Serial.begin(115200);
  delay(200);
  if (!driver.init()) {
    Serial.println("RH_ASK init FAILED");
    while (1) { delay(1000); }
  }
  Serial.println("RX ready");
}

void loop() {
  uint8_t buf[50];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) {     // mamy pakiet
    buf[buflen] = 0;                   // zakończ tekst
    Serial.print("Received: ");
    Serial.println((char*)buf);
  }
}
