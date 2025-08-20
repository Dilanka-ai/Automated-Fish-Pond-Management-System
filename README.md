# 🐟 Automated Fish Pond Management System

An Internet of Things (IoT) solution designed to automate the monitoring and management of a fish pond environment. This system uses an ESP32 microcontroller to gather data from various sensors and publish it to a cloud-based MQTT broker, enabling real-time monitoring and remote control of pond parameters.
![Your GIF Alt Text](path/to/your/video.gif)
## ✨ Features
* [cite_start]**Automated Feeding**: A feeder motor is activated at preset times (8:00, 12:00, and 18:00) to ensure consistent fish feeding[cite: 129, 130, 131].
* **Water Quality Monitoring**: The system continuously measures critical water parameters:
    * [cite_start]**Electrical Conductivity (EC) / Total Dissolved Solids (TDS)**: Measures the concentration of dissolved ions in the water[cite: 87, 89, 139].
    * [cite_start]**Temperature**: Monitors the water temperature in both Celsius and Fahrenheit[cite: 87, 137].
    * [cite_start]**pH Level**: Measures the acidity or alkalinity of the water, with a manual adjustment feature[cite: 91, 115, 116, 135].
    * [cite_start]**Water Level**: Uses an ultrasonic sensor to calculate and monitor the water level in the pond[cite: 91, 132, 133].
* **Remote Control**: The system can be remotely controlled via MQTT to perform actions such as:
    * [cite_start]Activating a water `feed` motor[cite: 108].
    * [cite_start]Controlling `water_out` and `water_in` motors[cite: 110, 112].
    * [cite_start]Adjusting the pH value[cite: 115, 116].
* [cite_start]**Cloud Integration**: All sensor data is published to an MQTT broker (HiveMQ) for remote access and data visualization[cite: 94, 95, 96, 135].
* [cite_start]**Real-Time Clock (RTC)**: An RTC module keeps track of the date and time, enabling precise scheduling for automated tasks[cite: 84, 120, 126].

## 🛠️ Hardware Components
* [cite_start]**Microcontroller**: ESP32 (or ESP8266, as the code is compatible with both)[cite: 84].
* **Sensors**:
    * [cite_start]**Temperature Sensor**: DS18B20 (DallasTemperature library)[cite: 86].
    * [cite_start]**EC/TDS Sensor**: Custom setup using a voltage divider with a 1000-ohm resistor (R1)[cite: 86].
    * [cite_start]**Ultrasonic Sensor**: HC-SR04 for water level[cite: 91].
    * [cite_start]**pH Sensor**: Analog pH sensor module[cite: 91].
    * [cite_start]**Real-Time Clock**: RTC DS1307 module[cite: 84].
* **Actuators**:
    * [cite_start]Feeder Motor[cite: 85].
    * [cite_start]Water Out Motor[cite: 92].
    * [cite_start]Water In Motor[cite: 92].
* [cite_start]**Connectivity**: WiFi module for internet and MQTT connection[cite: 98].

## ⚙️ Software & Libraries
The project is built using the Arduino IDE. The following libraries are required:
* [cite_start]`ArduinoJson`: For creating JSON payloads to send via MQTT[cite: 84].
* [cite_start]`PubSubClient`: For MQTT communication[cite: 84, 96].
* [cite_start]`WiFi.h` or `ESP8266WiFi.h`: For WiFi connectivity[cite: 84].
* [cite_start]`WiFiClientSecure.h`: For a secure WiFi connection[cite: 84].
* [cite_start]`RTClib.h`: For the RTC module[cite: 84].
* [cite_start]`OneWire.h` & `DallasTemperature.h`: For the temperature sensor[cite: 86].

## 🚀 Setup & Installation

### 1. Hardware Assembly
Connect all sensors and motors to the ESP32 microcontroller according to the pin definitions in the code.
* [cite_start]`motor`: GPIO 27[cite: 85].
* [cite_start]`EC_Read`: GPIO 33[cite: 87].
* [cite_start]`ECPower`: GPIO 35[cite: 87].
* [cite_start]`Temp_pin`: GPIO 13[cite: 86].
* [cite_start]`echoPin`: GPIO 4[cite: 91].
* [cite_start]`trigPin`: GPIO 2[cite: 91].
* [cite_start]`pH_sensor_pin`: GPIO 32[cite: 91].
* [cite_start]`water_out_motor`: GPIO 15[cite: 92].
* [cite_start]`water_in_motor`: GPIO 5[cite: 92, 93].

### 2. Software Configuration
1.  **Install Arduino IDE**: Download and install the Arduino IDE.
2.  **Board Manager**: Add the ESP32 board to your Arduino IDE.
3.  **Library Manager**: Install the required libraries listed above via the Arduino Library Manager.
4.  **Update Credentials**: Open the `Final_code.ino` file and update the following variables with your own credentials:
    * [cite_start]`ssid`: Your WiFi network name[cite: 94].
    * [cite_start]`password`: Your WiFi password[cite: 94].
    * [cite_start]`mqtt_server`: Your MQTT broker address[cite: 94].
    * [cite_start]`mqtt_username`: Your MQTT username[cite: 95].
    * [cite_start]`mqtt_password`: Your MQTT password[cite: 95].
5.  **Upload the Code**: Connect your ESP32 board to your computer, select the correct board and COM port, and upload the code.

## 🤝 Contribution
Feel free to fork this repository and contribute to its development. You can add new features, improve the code, or enhance the documentation.

## 📄 License
This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).
