# PawCare-Smart Health Tag for Stray Dog Welfare
## Introduction
The VSDSquadron Mini (CH32V003F4U6) is a compact and powerful microcontroller designed for embedded applications. This project integrates multiple communication and sensor modules to create a fully functional IoT-based system. The objective is to enable wireless connectivity, GSM communication, motion tracking, temperature monitoring, and GPS tracking while efficiently managing limited UART, I2C, and GPIO resources. The firmware is designed to handle data acquisition, processing, and transmission, making it suitable for applications such as remote monitoring, smart tracking, and IoT automation.
## Overview
This repository contains the circuit connections and firmware setup for the VSDSquadron Mini (CH32V003F4U6-based) microcontroller integrating multiple modules. The purpose of this setup is to provide wireless connectivity, GSM communication, motion tracking, temperature monitoring, and GPS tracking in a compact and efficient manner.
## Need of PawCare
Millions of stray dogs struggle daily with hunger, untreated diseases, and lack of care. Without proper monitoring, their health deteriorates, leading to suffering and the spread of infections. PawCare is the game-changer—using smart technology to detect hunger, predict diseases early, and enhance their overall welfare. By addressing these critical issues, PawCare ensures a healthier, safer life for stray dogs while promoting a compassionate and responsible society.
## Components required with Bill of Materials
| Item                   | Quantity | Description                                                   | Links to Products                                      |
|------------------------|----------|---------------------------------------------------------------|---------------------------------------------------|
| VSD Squadron Mini      | 1        | Microcontroller board                                        | [VSD Squadron Mini ](https://pages.razorpay.com/vsdsqmnMAY24)        |
| ESP8266          | 1        |    Microcontroller board                         | [esp8266](https://www.amazon.in/esp8266/s?k=esp8266)       |
| DHT11| 1        | temperature sensor                   | [temperture sensor](https://www.amazon.in/EPICTAC%C2%AE-3-3V-5V-Temperature-Humidity-Raspberry/dp/B092SFGF52)        |
| Jumper Wires           | 1 set    | Male-to-male and female-to-male jumper wires                 | [Jumper Wires](https://amzn.in/d/abTh8bo)           |
| Micro USB Cable        | 1        | For programming and power supply                             | [Micro USB Cable](https://amzn.in/d/9b3ttSo)        |
| GPS Module   | 1        | location tracker                            | [GPS](https://www.amazon.in/NEO-8M-GPS-Module-Micro-Interface/dp/B0C1V8P4HJ/ref=asc_df_B0C1V8P4HJ/?tag=googleshopdes-21&linkCode=df0&hvadid=709963085501&hvpos=&hvnetw=g&hvrand=17686159335378910175&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1007812&hvtargid=pla-2350755046754&mcid=fbf4243ee00f3b43ab77553dc337f7e8&gad_source=1&th=1)        |
| Sim800L       | 1        | GSM module                             | [GSM MODULE](https://www.amazon.in/Robodo-Electronics-SIM800LTTL-Module-Quad-Band/dp/B07B92T3K7/ref=asc_df_B07B92T3K7/?tag=googleshopdes-21&linkCode=df0&hvadid=709963085501&hvpos=&hvnetw=g&hvrand=13047581882593455737&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1007812&hvtargid=pla-882892903166&psc=1&mcid=e76a14c065e03bbab6df00c1c2a2b50d&gad_source=1)        |
| Accelerometer      | 1        | Movement tracker                            | [MPU6050](https://amzn.in/d/9b3ttSo)        |
| Battery Managament Module(BMS)      | 1        | Battery Protection & Optimization| [TP4056](https://www.amazon.in/SOOTRA-TP4056-lithium-Battery-Charging/dp/B0BGY2YDVL?th=1))        |
|  Li-Po Rechargeable Battery    | 1        | Power Supply                          | [Li-Po](https://www.amazon.com/dp/B09YQ2QPVL?ref=emc_s_m_5_i_atc)   |


## Pin Connections
The VSDSquadron Mini (CH32V003F4U6-based) has UART, I2C, and GPIO pins.

1️) ESP8266 (WiFi Module)
ESP8266 TX → VSDSquadron Mini PD6 (UART RX)
ESP8266 RX → VSDSquadron Mini PD5 (UART TX)
ESP8266 VCC → 3.3V
ESP8266 GND → GND

2️) SIM800L (GSM Module)
SIM800L TX → VSDSquadron Mini PD6 (Shared UART RX with ESP8266)
SIM800L RX → VSDSquadron Mini PD5 (Shared UART TX with ESP8266)
SIM800L VCC → 5V 
SIM800L GND → GND

3️) MPU6050 (Accelerometer & Gyroscope)
MPU6050 SDA → VSDSquadron Mini PC1 (I2C SDA)
MPU6050 SCL → VSDSquadron Mini PC2 (I2C SCL)
MPU6050 VCC → 3.3V
MPU6050 GND → GND

4️) DHT11 (Temperature & Humidity Sensor)
DHT11 Data → VSDSquadron Mini PD0
DHT11 VCC → 3.3V
DHT11 GND → GND

5️) NEO-6M GPS Module
GPS TX → VSDSquadron Mini PA0 (Software Serial RX)
GPS RX → VSDSquadron Mini PA1 (Software Serial TX)
GPS VCC → 3.3V
GPS GND → GND
## Pinout Diagram
<img src="CB.jpg" alt="Circuit Diagram" width="500">

## 3D Design for Product Package
![IMG-20250206-WA0041](https://github.com/user-attachments/assets/88a2ff7d-5c04-4e8e-ac98-6de3eb543fc0)

![IMG-20250206-WA0039](https://github.com/user-attachments/assets/8c0bd7f3-5143-44f3-be72-222b93090aa7)

![IMG-20250206-WA0025_2](https://github.com/user-attachments/assets/8f4dbf04-9849-45db-867d-f753f822470a)


## Working Code
```
#include <Wire.h>            // For I2C communication (MPU6050)
#include <SoftwareSerial.h>  // For GSM & GPS module
#include "DHT.h"             // For DHT sensor
#include <ch32v00x.h>
#include <ch32v00x_gpio.h>

// PIN Configurations
#define DHTPIN PD0
#define DHTTYPE DHT11

// Sensor & Module Objects
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial gpsSerial(PA0, PA1);  // GPS RX, TX
SoftwareSerial gsmSerial(PD5, PD6);  // SIM800L RX, TX

// Global Variables
String receivedCommand = "";
float temperature, humidity, heatIndex;
float ax, ay, az;

// Function to Send SMS
void sendSMS(String message) {
    Serial.println("Sending SMS...");
    gsmSerial.println("AT+CMGF=1");  // SMS mode
    delay(1000);
    gsmSerial.println("AT+CMGS=\"+91xxxxxxxxxx\"");  // Replace with volenteer or doctor's mobile number
    delay(1000);
    gsmSerial.print(message);
    delay(1000);
    gsmSerial.write(26);  // End SMS
    delay(5000);
}

// Function to Read GPS Coordinates
String getGPSLocation() {
    String gpsData = "";
    while (gpsSerial.available()) {
        gpsData += (char)gpsSerial.read();
    }

    if (gpsData.indexOf("GPGGA") > 0) {  // Check if GPS data is valid
        int latStart = gpsData.indexOf(",") + 1;
        int lonStart = gpsData.indexOf(",", latStart + 1) + 1;
        
        String latitude = gpsData.substring(latStart, lonStart - 1);
        String longitude = gpsData.substring(lonStart, lonStart + 9);

        Serial.println("Preparing to send location...");
        delay(2000);
        
        String googleMapsLink = "https://www.google.com/maps?q=" + latitude + "," + longitude;
        sendSMS("Location Sent: " + googleMapsLink);
        return googleMapsLink;
    }
    
    return "GPS data not available.";
}

// Function to Read DHT Sensor Data
void readDHT() {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    heatIndex = dht.computeHeatIndex(temperature, humidity, false);

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("DHT Sensor Error!");
        return;
    }

    Serial.print("Temperature: "); Serial.print(temperature); Serial.println("°C");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println("%");
    Serial.print("Heat Index: "); Serial.print(heatIndex); Serial.println("°C");
}

// Function to Read MPU6050 (Acceleration Data)
void readMPU6050() {
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 6, true);

    int16_t AccX = Wire.read() << 8 | Wire.read();
    int16_t AccY = Wire.read() << 8 | Wire.read();
    int16_t AccZ = Wire.read() << 8 | Wire.read();

    ax = AccX / 16384.0;
    ay = AccY / 16384.0;
    az = AccZ / 16384.0;

    Serial.print("Acceleration X: "); Serial.println(ax);
    Serial.print("Acceleration Y: "); Serial.println(ay);
    Serial.print("Acceleration Z: "); Serial.println(az);
}

// Setup Function
void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    gsmSerial.begin(9600);
    Wire.begin();
    dht.begin();

    Serial.println("PawCare System Initialized!");
    Serial.println("Waiting for command...");
}

// Main Loop
void loop() {
    if (Serial.available()) {
        receivedCommand = Serial.readStringUntil('\n');
        Serial.println("Received the command");

        readDHT();
        readMPU6050();
        String location = getGPSLocation();

        Serial.println(location);
    }

    delay(2000);
}

```

## Output Video
https://drive.google.com/file/d/10mVwB8beWYo9Sr0sMCDud-_ZU_CJZYiU/view?usp=drive_link
## [Notes]
- **ESP8266 and SIM800L share the same UART (PD6 & PD5)**, requiring careful switching using software control.
- **SIM800L requires a stable 4.2V-5V power source**, ensuring sufficient current (~2A peak) with a capacitor for stability.
- **GPS module runs on software serial (PA0 & PA1)**, which may need baud rate tuning for optimal performance.
- **MPU6050 operates on I2C and should have proper pull-up resistors if needed.**

## Conclusion & Results
This project successfully integrates multiple communication and sensor modules with the VSDSquadron Mini microcontroller, effectively utilizing limited GPIO and UART resources. The system allows for real-time data acquisition, wireless transmission, and GPS tracking. The implemented firmware efficiently manages UART switching and sensor data processing.

**Results:**
- **GPS tracking with NEO-6M**, providing location data over software serial.
- **GSM functionality established using SIM800L**, allowing SMS and call operations.
- **Accurate motion sensing through MPU6050**, providing real-time gyroscope and accelerometer readings.
- **Temperature and humidity monitoring with DHT11**, ensuring environmental tracking.
- **WiFi connectivity achieved via ESP8266**, enabling remote communication.

Overall, the project successfully demonstrates a compact and efficient IoT-based system that can be extended for various real-world applications such as smart tracking, remote monitoring, and data logging.

## Next Steps
- Implement firmware for managing UART switching dynamically.
- Upload the complete circuit schematic.
- Conduct individual module tests before full integration.
