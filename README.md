# Remote Control for Mitsubishi FX5U via MODBUS TCP and OSC

This repository contains two Arduino sketches for controlling a Mitsubishi FX5U PLC remotely using two different communication protocols:

- 🧰 **MODBUS TCP** via Ethernet (for industrial standard communication)
- 🎵 **OSC (Open Sound Control)** via UDP (for media and creative applications)

## 📁 Files

- `Remote_MODBUSTCP.ino`  
  Uses MODBUS TCP protocol to communicate with FX5U PLC over Ethernet. Designed for industrial control and automation applications.

- `Remote_OSC.ino`  
  Uses OSC (Open Sound Control) over UDP. Ideal for integration with creative tools like TouchDesigner, Ableton Live, VDMX, and other multimedia software.

---

## 📡 Hardware

- Arduino-compatible board (ESP32/ESP8266 recommended for WiFi or Ethernet)
- Ethernet/WiFi shield (if not built-in)
- Mitsubishi FX5U PLC with MODBUS TCP enabled
- Network router/switch for communication
- Optional: Media controller for OSC (e.g., TouchOSC, Lemur, custom app)

---

## 🔧 Setup

### MODBUS TCP Version

1. Configure IP address of your Arduino and FX5U PLC.
2. Make sure FX5U has MODBUS TCP Server enabled.
3. Upload `Remote_MODBUSTCP.ino` to your board.
4. Monitor communication via serial console.
5. PLC can be controlled via MODBUS registers/coils.

### OSC Version

1. Define OSC address/port to match the software you want to control.
2. Use tools like TouchOSC or Max/MSP to send OSC messages.
3. Upload `Remote_OSC.ino` to your board.
4. Board will listen and respond to OSC commands over UDP.

---

## 💡 Use Cases

- **MODBUS TCP**
  - Factory automation
  - SCADA integration
  - Remote relay control

- **OSC**
  - Interactive installations
  - Light and sound synchronization
  - Control of robot arms and kinetic sculptures

---

## 📜 License

MIT License
