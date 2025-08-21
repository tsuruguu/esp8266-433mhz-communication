#include <RH_ASK.h>
#include <SPI.h>  // wymagane przez RadioHead na ESP8266

// RH_ASK(bitRate, rxPin, txPin, pttPin, pttInverted)
// bitRate 2000 bps jest stabilne dla FS1000A/MX-RM
RH_ASK driver(2000, -1, 4, -1, false);  // txPin = GPIO4 (D2)

void setup() {
  Serial.begin(115200);
  delay(200);
  if (!driver.init()) {
    Serial.println("RH_ASK init FAILED");
    while (1) { delay(1000); }
  }
  Serial.println("TX ready");
}

void loop() {
  const char* msg = "HELLO_433";  // dowolny tekst/bajty
  driver.send((uint8_t*)msg, strlen(msg));
  driver.waitPacketSent();  // czekaj aż wyśle
  Serial.println("Sent: HELLO_433");
  delay(1000); // wysyłaj co sekundę
}
