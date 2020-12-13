/*
======ALL SNIPPETS ARE MINIMAL CODES PERFORMING A CERTAIN FUNCTIONALITY=========

This code snippet is for testing if the values of the accelerometer can be read. The goal is to store the acceleration values as floats in corresponding variables.

To check these variables a serial connection to the Pi is enabled printing the values to the serial terminal.
*/

/*
The main loop of the test programm calls new measurements
*/

#include <Adafruit_LSM6DS33.h>

// define sensor objects
Adafruit_LSM6DS33 lsm6ds33;


void setup()
{
    // ==> setup serial connection
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10); // will pause Uno, etc until serial console opens
    }
    // <==

    // ==> setup connection to accelerometer
    if (!lsm6ds33.begin_I2C())
    {
        Serial.println("Failed to find LSM6DS33 chip");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("LSM6DS33 Found!");
    // <==

}

void loop()
{
    // Get normalized sensor events
    // usually events store sensor data as 32-bit floats
    sensors_event_t accel; // events are obj to hold results
    sensors_event_t gyro;
    sensors_event_t temp;
    lsm6ds33.getEvent(&accel, &gyro, &temp); // actually read the sensor

    Serial.print("\t\tTemperature ");
    Serial.print(temp.temperature);
    Serial.println(" deg C");
    
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("\t\tAccel X: ");
    Serial.print(accel.acceleration.x);
    Serial.print(" \tY: ");
    Serial.print(accel.acceleration.y);
    Serial.print(" \tZ: ");
    Serial.print(accel.acceleration.z);
    Serial.println(" m/s^2 ");
    
    /* Display the results (rotation is measured in rad/s) */
    Serial.print("\t\tGyro X: ");
    Serial.print(gyro.gyro.x);
    Serial.print(" \tY: ");
    Serial.print(gyro.gyro.y);
    Serial.print(" \tZ: ");
    Serial.print(gyro.gyro.z);
    Serial.println(" radians/s ");
    Serial.println();
    
    delay(100);
}  
    