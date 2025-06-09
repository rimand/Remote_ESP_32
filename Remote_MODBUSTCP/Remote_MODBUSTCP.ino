#include <SPI.h>
#include <Ethernet.h>
#include <ModbusEthernet.h>
#include <Bounce2.h>
#include <MagicPot.h>

// ================== Ethernet ==================
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA };
IPAddress ip(192, 168, 3, 85);       // IP ของ ESP32/W5500
IPAddress remote(192, 168, 3, 205);  // IP ของ PLC FX5U
ModbusEthernet mb;

// ============ Button ===========
enum BTNDATA {
  _BTN_1_LEFT,
  _BTN_2_RIGHT,
  _BTN_3_ORIGIN,
  _BTN_4_GOORIGIN,
  _BTN_5_SERVOON,
  _BTN_6_STOP,
  _BTN_7_EMERGENCY
};
String BTNNAME[7] = {
  "_BTN_1_LEFT", "_BTN_2_RIGHT", "_BTN_3_ORIGIN", "_BTN_4_GOORIGIN",
  "_BTN_5_SERVOON", "_BTN_6_STOP", "_BTN_7_EMERGENCY"
};
int BTN_pin[7] = { 0, 4, 2, 5, 21, 22, 27 };
int PLC_DATA[7] = { 8247, 8248, 9999, 9999, 8192, 8202, 8203 };
Bounce bounce[7];

// ============ Potentiometer ===========
#define POTENTIOMETER_PIN 34
#define POTENTIOMETER_MIN_READ 0
#define POTENTIOMETER_MAX_READ 255
MagicPot potentiometer(POTENTIOMETER_PIN, POTENTIOMETER_MIN_READ, POTENTIOMETER_MAX_READ, MAGIC_POT_MAX_RAW_VALUE_12B);
unsigned long knob_time;
int knob_value;
int knob_value_prev;

// ============ Setup ===========
void setup() {
  Serial.begin(115200);
  delay(1000);

  for (int i = 0; i < 7; i++) {
    bounce[i].attach(BTN_pin[i], INPUT_PULLUP);
    bounce[i].interval(10);
  }

  // Ethernet stuff
  Ethernet.init(15);
  Ethernet.begin(mac, ip);

  // Ethernet with useful options
  // Ethernet.begin(mac, ip, dns, gateway, subnet); // full
  Ethernet.setRetransmissionCount(2);     // default: 8[times]
  Ethernet.setRetransmissionTimeout(50);  // default: 200[ms]
  delay(1000);

  mb.client();  // ใช้ Modbus TCP เป็น client
  potentiometer.begin();

  Serial.println("Modbus TCP Controller Ready");
}

// ============ Loop ===========
void loop() {
  Button_Update();
  Knob_Update();

  if (!mb.isConnected(remote)) {
    mb.connect(remote);
  }
  mb.task();
}

// ============ Button Function ===========
void Button_Update() {
  for (int i = 0; i < 7; i++) {
    bounce[i].update();

    if (bounce[i].changed()) {
      int deboucedInput = bounce[i].read();
      if (!deboucedInput) {
        Serial.println(BTNNAME[i] + " : ON");

        if (mb.isConnected(remote)) {
          // เขียน M0 ถึง M6 ตาม index ปุ่ม
          if (i == 6) {
            mb.writeCoil(remote, PLC_DATA[i], false);  // !state: กด = LOW = true
          } else {
            mb.writeCoil(remote, PLC_DATA[i], true);  // !state: กด = LOW = true
          }
        }
      }
      if (deboucedInput) {
        Serial.println(BTNNAME[i] + " : OFF");
        if (mb.isConnected(remote)) {
          // เขียน M0 ถึง M6 ตาม index ปุ่ม

          if (i == 6) {
            mb.writeCoil(remote, PLC_DATA[i], true);  // !state: กด = LOW = true
          } else {
            if (i != 4) {
              mb.writeCoil(remote, PLC_DATA[i], false);  // !state: กด = LOW = true
            }
          }
        }
      }
    }
  }
}

// ============ Potentiometer Function ===========
void Knob_Update() {
  potentiometer.read(5);

  if (millis() - knob_time > 100) {
    knob_value = potentiometer.getValue();
    if (knob_value != knob_value_prev) {
      Serial.println("Knob : " + String(knob_value));
      if (mb.isConnected(remote)) {
        int32_t value = ((float)knob_value / 255.0) * 200000;
        // uint16_t lowWord = value & 0xFFFF;
        // uint16_t highWord = (value >> 16) & 0xFFFF;

        // mb.writeHreg(remote, 600, lowWord);   // LSW ไปที่ D600
        // mb.writeHreg(remote, 601, highWord);  // MSW ไปที่ D601
        uint16_t regs[2];
        regs[0] = value & 0xFFFF;          // LSW
        regs[1] = (value >> 16) & 0xFFFF;  // MSW
        mb.writeHreg(remote, 600, regs, 2);  // เขียน 2 words: D600 และ D601

        delay(50);
        int32_t value_ = -value;
        // lowWord = value & 0xFFFF;
        // highWord = (value >> 16) & 0xFFFF;

        // mb.writeHreg(remote, 610, lowWord);   // LSW ไปที่ D600
        // mb.writeHreg(remote, 611, highWord);  // MSW ไปที่ D601
        regs[0] = value_ & 0xFFFF;          // LSW
        regs[1] = (value_ >> 16) & 0xFFFF;  // MSW
        mb.writeHreg(remote, 610, regs, 2);  // เขียน 2 words: D600 และ D601

        Serial.println("Knob PLC: " + String(value));
      }
      knob_value_prev = knob_value;
    }
    knob_time = millis();
  }
}
