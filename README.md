# esp8266-433mhz-communication

This project shows how to use **ESP8266** with **433 MHz ASK/OOK modules** (FS1000A transmitter + XY-MK-5V receiver) to build a simple wireless link using the [RadioHead](http://www.airspayce.com/mikem/arduino/RadioHead/) library (`RH_ASK`).

The original examples used fixed strings (`"Hello World"`).  
This repository has now been **extended**:  

👉 The **transmitter** can read data from the UART (USB-Serial connection to PC) and immediately send those bytes over 433 MHz.  
👉 The **receiver** just prints whatever it receives back to its own UART.  

This makes the ESP8266 act as a **transparent wireless bridge**:  
```
PC → UART → ESP8266 + TX module → 433 MHz → ESP8266 + RX module → UART → PC
```

---

## Hardware

- **ESP8266 development board** (e.g. NodeMCU, Wemos D1 mini)  
- **433 MHz ASK transmitter** (FS1000A)  
- **433 MHz ASK receiver** (XY-MK-5V)  

### Connections

#### Transmitter
- RF data pin → **GPIO4 (D2)**  
- VCC → 5 V  
- GND → GND  

#### Receiver
- RF data pin → default `RH_ASK` RX pin (GPIO11 on ESP8266 internally, no need to change)  
- VCC → 5 V  
- GND → GND  

> ⚠️ Some modules claim 5 V supply but work more reliably with **3.3 V** when used with ESP8266. Check your specific modules.  

---

## Software

### Libraries
- [RadioHead](http://www.airspayce.com/mikem/arduino/RadioHead/)  

Install it via Arduino Library Manager or download directly.

### Transmitter (`transmitter.ino`)

- Opens UART at **115200 bps**.  
- Collects bytes arriving from PC into a buffer.  
- Sends a packet over 433 MHz when:  
  - a newline (`\n` or `\r`) arrives, **or**  
  - a **gap timeout** (default 30 ms without new data) expires, **or**  
  - the buffer is full (≈ 50 bytes).  
- After sending, the buffer is cleared.  
- Packets contain **exactly** the bytes received — no extra terminators are added.  

### Receiver (`receiver.ino`)

- Uses `driver.recv(buf, &len)` to get packets from the air.  
- Prints received data to Serial (UART 115200).  
- No changes needed — works as before.  

---

## Usage

1. Connect ESP8266 boards to your PC.  
2. Upload `transmitter.ino` to the board with TX module.  
3. Upload `receiver.ino` to the board with RX module.  
4. Open **two Serial Monitors** at 115200 bps.  
   - One connected to the transmitter board (input).  
   - One connected to the receiver board (output).  
5. Type text into the transmitter’s Serial Monitor or send data programmatically from PC.  
   - Data is sent over 433 MHz.  
   - Receiver board prints the same data to its Serial Monitor.  

---

## Example

- On TX Serial Monitor you type:  
  ```
  Hello 433!
  ```
- The RX Serial Monitor shows:  
  ```
  Received: Hello 433!
  ```

---

## Notes

- Maximum packet length is defined by `RH_ASK_MAX_MESSAGE_LEN` (~50 bytes).  
- Longer data is automatically split into multiple packets.  
- Timing-based packetization means if you type very slowly in Serial Monitor, you may see single letters arriving as separate packets (use Enter or increase timeout if needed).  
- For more robust communication, you can add a simple protocol (e.g. `LEN + CRC`).  

---

## License

MIT
