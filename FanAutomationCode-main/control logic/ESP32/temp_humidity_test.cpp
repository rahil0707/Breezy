// Based on: https://www.esp32learning.com/code/hdc2080-humidity-and-temperature-sensor-connected-to-a-esp32.php

#include <HDC2080.h>

#define ADDR 0x40 // SENSOR ADDRESS SUBJECT TO CHANGE DEPENDING ON OUR SENSOR'S ADDRESS
HDC2080 sensor(ADDR);

float temperature = 0, humidity = 0;

void setup() {

Serial.begin(9600);
Serial.println("TinkeringTech HDC2080 Test");

// Initialize I2C communication
sensor.begin();

// Begin with a device reset
sensor.reset();

// Set up the comfort zone -- CAN BE CHANGED BASED ON OUR SENSOR
sensor.setHighTemp(48); // High temperature of 28C
sensor.setLowTemp(2); // Low temperature of 22C
sensor.setHighHumidity(95); // High humidity of 55%
sensor.setLowHumidity(10); // Low humidity of 40%

// Configure Measurements
sensor.setMeasurementMode(TEMP_AND_HUMID); // Set measurements to temperature and humidity
sensor.setRate(ONE_HZ); // Set measurement frequency to 1 Hz
sensor.setTempRes(FOURTEEN_BIT);
sensor.setHumidRes(FOURTEEN_BIT);

//begin measuring
sensor.triggerMeasurement();
}

void loop() {

Serial.print("Temperature (C): ");
Serial.print(sensor.readTemp());
Serial.print("\t\tHumidity (%): ");
Serial.println(sensor.readHumidity());

// Wait 1 second for the next reading
delay(2000);

}
