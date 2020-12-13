/*
======ALL SNIPPETS ARE MINIMAL CODES PERFORMING A CERTAIN FUNCTIONALITY=========

This code snippet is for testing if the pressure sensor is read correctly. 
*/

int pressure_sensor_pin=4;

float read_pressure()
{
    // get value from sensor mapping volt (0..1023 -> 0..5V)
    int value=analogRead(pressure_sensor_pin);
    float pressure_Pa=value*(10/1024)*100000;
    return pressure_Pa;
}
void setup()
{
    pinMode(pressure_sensor_pin,INPUT);
}

void loop()
{
     // ==> setup serial connection
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10); // will pause Uno, etc until serial console opens
    }
    // <==

    float pressure=read_pressure();
    Serial.print("Druck in Pa: ");
    Serial.println(pressure);
    delay(1000);
}