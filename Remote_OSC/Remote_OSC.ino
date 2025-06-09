#include <ArduinoOSCEther.h>
#include <Bounce2.h>
#include <MagicPot.h>

// Ethernet stuff
byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA};
IPAddress ip(192, 168, 3, 84);
// Ethernet with useful options
// const IPAddress dns (192, 168, 1, 1);
// const IPAddress gateway (192, 168, 1, 1);
// const IPAddress subnet (255, 255, 255, 0);

// for ArduinoOSC
const char *host = "192.168.3.94";
// const char *host_2 = "192.168.3.92";
const int recv_port = 7000;
const int bind_port = 54345;
const int send_port = 7001;
const int publish_port = 7002;
// send / receive varibales
int i;
float f;
String s;

// required to use manual packet parsing
OscEtherServer server(recv_port);
OscEtherClient client;
// OscEtherClient client(1234);  // you can set the local port of client manually (default: 9)

// ============ Button ===========
enum BTNDATA
{
  _BTN_1_LEFT,
  _BTN_2_RIGHT,
  _BTN_3_ORIGIN,
  _BTN_4_GOORIGIN,
  _BTN_5_SERVOON,
  _BTN_6_STOP,
  _BTN_7_EMERGENCY
};
BTNDATA BTN;

String BTNNAME[7] = {"_BTN_1_LEFT", "_BTN_2_RIGHT", "_BTN_3_ORIGIN", "_BTN_4_GOORIGIN", "_BTN_5_SERVOON", "_BTN_6_STOP", "_BTN_7_EMERGENCY"};

// #define _BTN_1_LEFT 0
// #define _BTN_2_RIGHT 4
// #define _BTN_3_ORIGIN 2
// #define _BTN_4_GOORIGIN 5
// #define _BTN_5_SERVOON 21
// #define _BTN_6_STOP 22
// #define _BTN_7_EMERGENCY 27

// int BTN_pin[7] = {_BTN_1_LEFT, _BTN_2_RIGHT, _BTN_3_ORIGIN, _BTN_4_GOORIGIN, _BTN_5_SERVOON, _BTN_6_STOP, _BTN_7_EMERGENCY};
int BTN_pin[7] = {0, 4, 2, 5, 21, 22, 27};

// INSTANTIATE A Bounce OBJECT
Bounce bounce[7];

//============== KNOB ====================
#define POTENTIOMETER_PIN 34
#define POTENTIOMETER_MIN_READ 0
#define POTENTIOMETER_MAX_READ 255
MagicPot potentiometer(POTENTIOMETER_PIN, POTENTIOMETER_MIN_READ, POTENTIOMETER_MAX_READ, MAGIC_POT_MAX_RAW_VALUE_12B);
unsigned long knob_time;
int knob_value;
int knob_value_prev;
bool knob_send_repeat;

// Function Declare
void Button_Update();
void onPotentiometerChange();
void Knob_Update();
void OSC_Update();

//===============================================
//---------------- Main -------------------------
//===============================================

void setup()
{
  for (int i = 0; i < 7; i++)
  {
    bounce[i] = Bounce();
    bounce[i].attach(BTN_pin[i], INPUT_PULLUP);
    bounce[i].interval(10); // interval in ms
  }

  Serial.begin(115200);
  delay(1000);

  // Ethernet stuff
  Ethernet.init(15);
  Ethernet.begin(mac, ip);

  // Ethernet with useful options
  // Ethernet.begin(mac, ip, dns, gateway, subnet); // full
  Ethernet.setRetransmissionCount(2);    // default: 8[times]
  Ethernet.setRetransmissionTimeout(50); // default: 200[ms]

  potentiometer.begin();
  potentiometer.onChange(onPotentiometerChange);

  Serial.println("start");
}

void loop()
{
  Button_Update();
  Knob_Update();
  // OSC_Update();
}

//===============================================
//---------------- Function ---------------------
//===============================================

void Knob_Update()
{
  potentiometer.read(10);

  if (millis() - knob_time > 200)
  {
    knob_value = potentiometer.getValue();
    if (knob_value != knob_value_prev)
    {
      Serial.println("Knob : " + String(knob_value));
      client.send(host, send_port, "/KNOB", knob_value);
      // client.send(host_2, send_port, "/KNOB", knob_value);
      knob_value_prev = knob_value;
    }
    knob_time = millis();
  }
}

void onPotentiometerChange()
{
  // if (millis() - knob_time > 50)
  // {
  //   // int percent = map(potentiometer.getValue(), 0, 255, 0, 100);
  //   knob_value = potentiometer.getValue();
  //   Serial.println("Knob : " + String(knob_value));
  //   client.send(host, send_port, "/KNOB", knob_value);
  //   knob_time = millis();
  //   knob_send_repeat = true;
  // }
}

void Button_Update()
{
  for (int i = 0; i < 7; i++)
  {
    bounce[i].update();

    if (bounce[i].changed())
    {
      int deboucedInput = bounce[i].read();
      if (!deboucedInput)
      {
        Serial.println(BTNNAME[i] + " : ON");
        client.send(host, send_port, "/" + BTNNAME[i], 1);
        // client.send(host_2, send_port, "/" + BTNNAME[i], 1);
      }
      if (deboucedInput)
      {
        Serial.println(BTNNAME[i] + " : OFF");
        client.send(host, send_port, "/" + BTNNAME[i], 0);
        // client.send(host_2, send_port, "/" + BTNNAME[i], 0);
      }
    }
  }
}

void OSC_Update()
{
  // static uint32_t prev_func_ms = millis();
  // if (millis() > prev_func_ms + 500)
  // {
  //   client.send(host, publish_port, F("/publish/func"), millis(), micros());
  //   prev_func_ms = millis();
  // }

  // if (server.parse())
  // {
  //   const OscMessage *msg = server.message();
  //   if (msg->address() == F("/need/reply"))
  //   {
  //     Serial.println(F("/need/reply"));
  //     Serial.print(msg->arg<int>(0));
  //     Serial.println();
  //     int i = millis();
  //     float f = (float)micros() / 1000.f;
  //     String s = F("hello");
  //     client.send(host, send_port, F("/reply"), i, f, s);
  //   }
  // }
}
