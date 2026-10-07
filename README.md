# Universal Embedded Hardware Drivers (`L3_Sensors`)

A modular, cross-platform C++ driver library designed for embedded systems and microcontrollers (Arduino, STM32, ESP32, Bare-Metal). 

## Supported Hardware & Modules

### 1. Ultrasonic Range Sensors (`UltrasonicRanger.hpp`)
Provides abstracted, cross-platform pulse timing drivers for 4-pin and 3-pin ultrasonic distance modules:
* **HC-SR04**: Standard 4-pin ultrasonic distance module.
* **HY-SRF05**: 5-pin enhanced replacement module.
* **JSN-SR04T**: Sealed waterproof ultrasonic sensor (Mode 1 / Pulse mode).
* **US-100**: Dual-mode ultrasonic range sensor (Pulse mode).

### 2. IR Break-Beam Sensors (`ir_breakbeam.h` / `ir_breakbeam.cpp`)
Provides non-blocking, debounced edge detection and optical channel monitoring:
* Active-High and Active-Low logic support.
* Flexible edge detection (`ON_ENTER`, `ON_EXIT`, `BOTH`).
* Hardware pull-up configuration and software debouncing.
* Asynchronous event callbacks with pulse duration measurement.

---

## Directory Structure

```text
.
├── include/
│   └── sensors/
│       ├── UltrasonicRanger.hpp
│       └── ir_breakbeam.h
├── src/
│   └── ir_breakbeam.cpp
├── examples/
│   ├── ultrasonic_example.cpp
│   └── breakbeam_example.cpp
├── .gitignore
├── LICENSE
└── README.md