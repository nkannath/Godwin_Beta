/*
    Script to save multiple sensors to SD card
        - can continuously run
        - all data is saved to a single database
*/

// Libraries
#include <FileIO.h>
#include <Bridge.h>


// Variables
const int BAUD = 9600;                  // baud rate
const int REFRESH_RATE = 250;           // refresh rate (ms)

int cur_time_ms;


// Main
void setup() {
    Serial.begin(BAUD);
    Bridge.begin();
    FileSystem.begin();

    SerialUSB.println("Multiple Sensor SD Save");
}

void loop() {
    String readings;
    readings += String(millis());

    // read each sensor and append to readings
    for(int analog_pin=0; analog_pin<7; analog_pin++){
        int sensor_reading = analogRead(analog_pin);
        readings += String(sensor_reading);
        if (analog_pin<6){
            readings += ","
        }
    }

    // open file to write to
    File dataFile = FileSystem.open("/mnt/sd/chair_datalog.csv", FILE_APPEND);

    // if file can be written to, write
    if (dataFile) {
        dataFile.println(readings);
        dataFile.close();
        Serial.println(readings);
    }
    // throw error if file can't be written to
    else {
        Serial.println("error opening chair_datalog.csv");
    }

    delay(REFRESH_RATE);
}