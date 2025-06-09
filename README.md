Remote Control for Mitsubishi FX5U via MODBUS TCP and OSC
This repository contains two Arduino sketches for controlling a Mitsubishi FX5U PLC remotely using two different communication protocols:

🧰 MODBUS TCP via Ethernet (for industrial standard communication)

🎵 OSC (Open Sound Control) via UDP (for media and creative applications)

📁 Files
Remote_MODBUSTCP.ino
Uses MODBUS TCP protocol to communicate with FX5U PLC over Ethernet. Designed for industrial control and automation applications.

Remote_OSC.ino
Uses OSC (Open Sound Control) over UDP. Ideal for integration with creative tools like TouchDesigner, Ableton Live, VDMX, and other multimedia software.

📡 Hardware
Arduino-compatible board (ESP32/ESP8266 recommended for WiFi or Ethernet)

Ethernet/WiFi shield (if not built-in)

Mitsubishi FX5U PLC with MODBUS TCP enabled

Network router/switch for communication

Optional: Media controller for OSC (e.g., TouchOSC, Lemur, custom app)

🔧 Setup
MODBUS TCP Version
Configure IP address of your Arduino and FX5U PLC.

Make sure FX5U has MODBUS TCP Server enabled.

Upload Remote_MODBUSTCP.ino to your board.

Monitor communication via serial console.

PLC can be controlled via MODBUS registers/coils.

OSC Version
Define OSC address/port to match the software you want to control.

Use tools like TouchOSC or Max/MSP to send OSC messages.

Upload Remote_OSC.ino to your board.

Board will listen and respond to OSC commands over UDP.

💡 Use Cases
MODBUS TCP

Factory automation

SCADA integration

Remote relay control

OSC

Interactive installations

Light and sound synchronization

Control of robot arms and kinetic sculptures

📜 License
MIT License
