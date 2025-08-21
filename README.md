# ESP8266 433 MHz Communication Demo

## 📌 Project description
This project demonstrates simple wireless communication between two **ESP8266 (NodeMCU)** boards using inexpensive **433 MHz RF modules**.  
One ESP8266 acts as a **transmitter (TX)**, the other as a **receiver (RX)**.  
The **RadioHead (RH_ASK)** library is used to implement **ASK/OOK modulation**.

Educational project – shows how to send text data in the 433 MHz ISM band.

---

## 🛠️ Hardware used
- 2× **ESP8266 NodeMCU v3 (ESP-12E)**  
- 1× **433 MHz Transmitter FS1000A**  
- 1× **433 MHz Receiver MX-RM-5V**  
- 2× antennas – simple wire **~17 cm** (quarter-wave for 433 MHz)  
- Jumper wires, computer with Arduino IDE  

---

## 🔌 Wiring

### Transmitter (FS1000A)
- **DATA → D2 (GPIO4)** ESP8266  
- **VCC → VIN (5 V from USB)**  
- **GND → GND**  
- **ANT → ~17 cm wire**

### Receiver (MX-RM-5V)
- **DATA → D2 (GPIO4)** ESP8266  
- **VCC → VIN (5 V from USB)**  
- **GND → GND**  
- ⚠️ Note: if the receiver outputs 5 V on DATA, use a voltage divider to step down to **3.3 V max for ESP8266!**

---

## 💻 Software setup

### 1. Arduino IDE
- Install **ESP8266 board support**:  
  `File → Preferences → Additional Boards URLs`  
  ```
  http://arduino.esp8266.com/stable/package_esp8266com_index.json
  ```
  Then go to **Tools → Board → Board Manager** and install **ESP8266 by ESP8266 Community**.

- Select the board:  
  `Tools → Board → NodeMCU 1.0 (ESP-12E Module)`

- Recommended settings:  
  - **Upload Speed:** 115200  
  - **CPU Freq:** 80 MHz  
  - **Flash Size:** 4M (1M SPIFFS)  

### 2. Library
Install **RadioHead** (by Mike McCauley) via Arduino Library Manager.

---

## 📡 Example code

### Transmitter (TX)
```cpp
#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, -1, 4, -1, false); // 2000 bps, TX=GPIO4 (D2)

void setup() {
  Serial.begin(115200);
  if (!driver.init()) {
    Serial.println("RH_ASK init FAILED");
    while (1);
  }
  Serial.println("TX ready");
}

void loop() {
  const char *msg = "HELLO_433";
  driver.send((uint8_t*)msg, strlen(msg));
  driver.waitPacketSent();
  Serial.println("Sent: HELLO_433");
  delay(1000);
}
```

### Receiver (RX)
```cpp
#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 4, -1, -1, false); // 2000 bps, RX=GPIO4 (D2)

void setup() {
  Serial.begin(115200);
  if (!driver.init()) {
    Serial.println("RH_ASK init FAILED");
    while (1);
  }
  Serial.println("RX ready");
}

void loop() {
  uint8_t buf[50];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) {
    buf[buflen] = 0;
    Serial.print("Received: ");
    Serial.println((char*)buf);
  }
}
```

---

## 🧪 Testing
1. Connect both ESP8266 boards to your computer (different USB ports).  
2. Upload the TX code to one ESP and the RX code to the other.  
3. Open **Serial Monitor** (115200 baud) on both ports.  
4. On the transmitter you should see:  
   ```
   TX ready
   Sent: HELLO_433
   ```
5. On the receiver you should see:  
   ```
   RX ready
   Received: HELLO_433
   ```

---

## 📜 License
This project is released under the **MIT License** – you are free to use, copy, and modify the code with attribution to the original author.
