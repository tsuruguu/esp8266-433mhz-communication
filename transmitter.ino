#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, -1, 4, -1, false);

#ifndef RH_ASK_MAX_MESSAGE_LEN
#define RH_ASK_MAX_MESSAGE_LEN 50
#endif

static const uint32_t GAP_TIMEOUT_MS = 30;

static uint8_t  lineBuf[RH_ASK_MAX_MESSAGE_LEN];
static uint8_t  lineLen = 0;
static uint32_t lastByteAt = 0;

inline void startGapTimer() { lastByteAt = millis(); }

void flushAndSend()
{
  if (lineLen == 0) return;

  driver.send(reinterpret_cast<uint8_t*>(lineBuf), lineLen);
  driver.waitPacketSent();

  Serial.print(F("TX bytes: "));
  Serial.println(lineLen);

  lineLen = 0;
}

void setup() {
  Serial.begin(115200);
  delay(200);

  if (!driver.init()) {
    Serial.println(F("RH_ASK init FAILED"));
    while (1) { delay(1000); }
  }
  Serial.println(F("TX ready — send bytes over Serial."));
  Serial.println(F("Packet ends on: \\n/\\r, gap-timeout, or buffer full."));
  startGapTimer();
}

void loop() {
  bool gotByte = false;

  while (Serial.available() > 0) {
    int b = Serial.read();
    if (b < 0) break;

    gotByte = true;
    startGapTimer();

    if (b == '\n' || b == '\r') {
      flushAndSend();
      continue;
    }

    if (lineLen < RH_ASK_MAX_MESSAGE_LEN) {
      lineBuf[lineLen++] = static_cast<uint8_t>(b);
    } else {
      flushAndSend();
      lineBuf[lineLen++] = static_cast<uint8_t>(b);
    }
  }

  if (!gotByte && lineLen > 0) {
    if (millis() - lastByteAt >= GAP_TIMEOUT_MS) {
      flushAndSend();
      startGapTimer();
    }
  }
}
