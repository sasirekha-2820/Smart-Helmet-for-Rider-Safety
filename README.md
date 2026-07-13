# Smart Helmet Accident Detection and Emergency Alert System

## Overview
The Smart Helmet is an embedded safety system designed to automatically detect motorcycle accidents and send emergency alerts with the rider's location to predefined contacts.

The system continuously monitors motion and impact using sensors. When an accident or abnormal fall is detected, the system retrieves the GPS coordinates and sends an emergency SMS through the GSM module.

---

## Features

- Accident detection using accelerometer and IMU data
- Real-time GPS location tracking
- Automatic emergency SMS alerts
- Battery-powered portable operation
- STM32-based embedded implementation
- Designed for motorcycle rider safety applications

---

## Hardware Components

- STM32 Nucleo Development Board
- ISM330DHCX IMU Sensor
- ADXL335 Accelerometer
- u-blox NEO-M8N GPS Module
- SIM800L GSM Module
- 18650 Li-ion Battery
- TP4056 Charging Module
- 3.3V Voltage Regulator

---

## Software Tools

- STM32CubeIDE
- STM32 HAL Drivers
- Embedded C Programming Language

---

## Project Structure

```
SmartHelmet/
│
├── Core/
│   ├── Inc/
│   └── Src/
│
├── Drivers/
│
├── IFP_SmartHelmetv1.ioc
│
└── README.md
```

---

## Working Principle

1. The accelerometer and IMU continuously monitor helmet movement.
2. The STM32 analyzes sensor data to identify crashes or abnormal falls.
3. Once an accident is detected, the system reads the current GPS coordinates.
4. The GSM module sends an SMS containing the rider's location to emergency contacts.
5. This enables faster response and improves rider safety.

---

## Future Improvements

- Mobile application integration
- Cloud-based accident logging
- Real-time tracking dashboard
- Voice assistance features
- Helmet wear detection
---
## Skills Demonstrated

- Embedded C Programming
- STM32 Microcontroller Development
- UART Communication
- I2C Communication
- GPS Integration
- GSM Communication
- Sensor Interfacing
- Embedded System Design
- Debugging and Testing
---

## Applications

- Motorcycle rider safety
- Emergency response systems
- Fleet monitoring
- Smart transportation systems

---

## Author

Developed by **SASI REKHA C** as part of an Embedded Systems project focused on improving motorcycle rider safety through accident detection and emergency communication technologies.

- Department: Electronics and Communication Engineering
- Institution: SSN College of Engineering, Chennai
- Technologies: STM32, Embedded C, GPS, GSM, IMU Sensors
