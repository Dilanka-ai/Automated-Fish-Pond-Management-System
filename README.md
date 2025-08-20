# 🐟 Automated Fish Pond Management System

An **Internet of Things (IoT) solution** designed to automate the monitoring and management of a fish pond environment.  
This system uses an **ESP32 microcontroller** to gather data from various sensors and publish it to a **cloud-based MQTT broker**, enabling **real-time monitoring and remote control** of pond parameters.

---

<img src="https://github.com/Dilanka-ai/Automated-Fish-Pond-Management-System/blob/main/Klip%20Video%20Still%202025-08-20%200513pm.jpg?raw=true" width="500"/>

---

## ✨ Features

- **Automated Feeding**: A feeder motor is activated at preset times (8:00, 12:00, and 18:00) to ensure consistent fish feeding.  
- **Water Quality Monitoring**: Continuously measures critical parameters:
  - **Electrical Conductivity (EC) / Total Dissolved Solids (TDS)**: Measures the concentration of dissolved ions in the water.  
  - **Temperature**: Monitors the water temperature in both Celsius and Fahrenheit.  
  - **pH Level**: Measures the acidity or alkalinity of the water, with a manual adjustment feature.  
  - **Water Level**: Uses an ultrasonic sensor to calculate and monitor the pond’s water level.  
- **Remote Control**: The system can be remotely controlled via MQTT to:
  - Activate the feeder motor  
  - Control `water_out` and `water_in` motors  
  - Adjust the pH value  
- **Cloud Integration**: All sensor data is published to an **MQTT broker (HiveMQ)** for remote access and visualization.  
- **Real-Time Clock (RTC)**: Tracks date and time, enabling precise scheduling for automated tasks.  

---

## 🛠️ Hardware Components

- **Microcontroller**: ESP32 (compatible with ESP8266)  
- **Sensors**:  
  - DS18B20 Temperature Sensor (DallasTemperature library)  
  - EC/TDS Sensor (custom voltage divider with 1000Ω resistor)  
  - HC-SR04 Ultrasonic Sensor (water level)  
  - Analog pH Sensor Module  
  - DS1307 Real-Time Clock Module  
- **Actuators**:  
  - Feeder Motor  
  - Water Out Motor  
  - Water In Motor  
- **Connectivity**: WiFi module for internet and MQTT  

---

## ⚙️ Software & Libraries

Built using **Arduino IDE**.  
Required libraries:  

- `ArduinoJson` – for creating JSON payloads  
- `PubSubClient` – for MQTT communication  
- `WiFi.h` / `ESP8266WiFi.h` – for WiFi connectivity  
- `WiFiClientSecure.h` – for secure WiFi connection  
- `RTClib.h` – for RTC module  
- `OneWire.h` & `DallasTemperature.h` – for temperature sensor  

---

## 🚀 Setup & Installation

### 1. Hardware Assembly

Connect sensors and motors to ESP32 according to these pin definitions:

- `motor`: GPIO 27  
- `EC_Read`: GPIO 33  
- `ECPower`: GPIO 35  
- `Temp_pin`: GPIO 13  
- `echoPin`: GPIO 4  
- `trigPin`: GPIO 2  
- `pH_sensor_pin`: GPIO 32  
- `water_out_motor`: GPIO 15  
- `water_in_motor`: GPIO 5  

---

### 2. Software Configuration

1. **Install Arduino IDE**.  
2. **Board Manager**: Add the ESP32 board to Arduino IDE.  
3. **Library Manager**: Install all required libraries.  
4. **Update Credentials**: In `Final_code.ino`, set your WiFi & MQTT details:  
   ```cpp
   const char* ssid = "your_wifi_name";
   const char* password = "your_wifi_password";
   const char* mqtt_server = "your_mqtt_broker_address";
   const char* mqtt_username = "your_mqtt_username";
   const char* mqtt_password = "your_mqtt_password";
