#include <Wire.h>            // I2C for MPU6050
#include <SoftwareSerial.h>  // Soft Serial for GPS & GSM
#include "DHT.h"             // DHT11 library

// PIN Assignments
#define DHTPIN PD0
#define DHTTYPE DHT11

// Objects for sensors & modules
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial gpsSerial(PA0, PA1);  // GPS RX, TX
SoftwareSerial gsmSerial(PD5, PD6);  // SIM800L RX, TX

// Global variables
float lastTemperature = 0.0;
float lastHumidity = 0.0;
bool movementDetected = false;

// Function to send SMS alert via SIM800L
void sendAlertSMS(String message) {
    Serial.println("Sending SMS Alert...");
    gsmSerial.println("AT+CMGF=1");  // Set SMS mode
    delay(1000);
    gsmSerial.println("AT+CMGS=\"+91xxxxxxxxxx\"");  // Replace with phone number
    delay(1000);
    gsmSerial.print(message);
    delay(1000);
    gsmSerial.write(26);  // End SMS command
    delay(5000);
}

// Function to read & parse GPS data
void readGPS() {
    String gpsData = "";
    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        gpsData += c;
    }
    Serial.print("GPS Raw Data: ");
    Serial.println(gpsData);
    // Extract latitude & longitude (Implement further parsing)
}

// Function to read DHT11 sensor
void readDHT() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("DHT11 Sensor Error!");
        return;
    }

    Serial.print("Temperature: "); Serial.print(temperature); Serial.println("°C");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println("%");

    // Send alert if temperature goes too high
    if (temperature > 40.0 && lastTemperature <= 40.0) {
        sendAlertSMS("Warning! High temperature detected.");
    }

    lastTemperature = temperature;
    lastHumidity = humidity;
}

// Function to detect motion from MPU6050
void detectMotion() {
    Wire.beginTransmission(0x68); // MPU6050 I2C Address
    Wire.write(0x3B); // Accelerometer register
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 6, true);

    int16_t AccX = Wire.read() << 8 | Wire.read();
    int16_t AccY = Wire.read() << 8 | Wire.read();
    int16_t AccZ = Wire.read() << 8 | Wire.read();

    float ax = AccX / 16384.0;
    float ay = AccY / 16384.0;
    float az = AccZ / 16384.0;

    if (ax > 2.0 || ay > 2.0 || az > 2.0) {
        Serial.println("⚠ Motion Detected!");
        movementDetected = true;
        sendAlertSMS("PawCare Alert: Sudden movement detected!");
    } else {
        movementDetected = false;
    }
}

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    gsmSerial.begin(9600);
    Wire.begin();
    dht.begin();

    Serial.println("PawCare System Initialized!");
}

void loop() {
    readDHT();     // Read temperature & humidity
    readGPS();     // Read GPS location
    detectMotion(); // Check for sudden movements

    delay(2000); // Delay for stability
}