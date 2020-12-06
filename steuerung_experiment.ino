// ***Anforderungsliste an Arduinocode***

// - Tür n-mal automatisch öffnen und schließen lassen
// - je Türzyklus Zeitreihe für Druckwerte per USB rausschicken
// - je Türzyklus Zeitreihe für Beschleunigungswerte (linear/rotatorisch) per USB rausschicken
// - Tür manuell per Knopfdruck öffnen und schließen lassen
// - Drucksensorwerte auslesen
// - B-Sensorwerte auslesen

// load libaries
#include <Adafruit_LSM6DS33.h>

// assign pin numbers
int button_close=2;
int button_open=3;
int switch_door_closed=5;
int switch_door_open=4;
int valve_close_door=7;
int valve_open_door=8;
int valve_supply=6;
int piep=11;
int pressure_sensor_pin=A0; // A0 is just a variable storing a number

// define sensor objects
Adafruit_LSM6DS33 lsm6ds33;

// define process variables
int TIMEFRAME=30; // timeframe in s in which to make evenly spaced measurments
int STEP=0.2; // sec between two measurments. TIMEFRAME must be dividabl by STEP
int values_count=TIMEFRAME/STEP; // amount of measurments per timeframe
bool button_close_pressed=false;
bool button_open_pressed=false;
bool door_is_closing=false;
bool door_is_opening=false;
bool door_is_open=false;
bool door_is_closed=false;
bool new_readings_available=false;

// time relatet variables
unsigned long millis_last_reading; // time by then  sensors had been read last
unsigned long millis_door_opened=0; // time by then door is completly open
unsigned long millis_door_closed=0; // time by then door is completly closed
int millis_cycle_start;
int millis_cycle_end;
int timeframe_door_idle=2000; // ms door stays opened or closed
int timeframe_measurement=15000; // timeframe while sensors are read
int sensor_sampling_rate=200; // sensor values are read every x ms

int cycles_to_perform=2;
int cycle_counter=0;


// Funcitons to controll door movement
void open_door(){
    open_supply_valve();
    route_air_open();
    door_is_opening=true;
}

void close_door(){
    open_supply_valve();
    route_air_close();
    door_is_closing=true;
}




// Functions to controll valves
void open_supply_valve(){
    digitalWrite(valve_supply,LOW);
}

void close_supply_valve(){
    digitalWrite(valve_supply,HIGH);
}

void route_air_open(){
    digitalWrite(valve_open_door,LOW);
}

void route_air_close(){
    digitalWrite(valve_close_door,LOW);
}

// Functions for sensor readings
float get_pressure()
{
    // get number corresponding to voltage coming from pressur sensor
    int sensor_value=analogRead(pressure_sensor_pin); // between 0 and 1023
    // calc pressur in MPa
    // pressure range is 0..10bar, UNIT USED Pa (1Mpa=10bar)
    float pressure=sensor_value*(1000000.0/1023.0);
    return pressure;
}
//==============================================================================
//==============================================================================
void setup()
{
    Serial.begin(115200);

    // setup pinmodes
    pinMode(button_close, INPUT);
    pinMode(button_open, INPUT);
    pinMode(switch_door_open, INPUT);
    pinMode(switch_door_closed, INPUT);
    pinMode(valve_close_door, OUTPUT);
    pinMode(valve_open_door, OUTPUT);
    pinMode(valve_supply, OUTPUT);
    pinMode(piep, OUTPUT);
    pinMode(pressure_sensor_pin,INPUT);
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    digitalWrite(8,HIGH);
    
    float p;

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    // ==> start first cycle
    // door is still closed yet
    open_door();
    millis_cycle_start=millis();
    millis_last_reading=millis();
    bool duration_send=false;
    // <==

    // *****main loop*****
    while (cycle_counter<cycles_to_perform)
    {
        // ==> check current door status
        // door can only be open, if it was opening before
        if (door_is_opening)
        {
            // check if door is open yet
            door_is_open=(digitalRead(switch_door_open)==HIGH);
            if (door_is_open)
            {
                // door is not opening any longer
                door_is_opening=false;
            }
        }
        if (door_is_closing)
        {
            // check if door is closed yet
            door_is_closed=(digitalRead(switch_door_closed)==HIGH);
            if (door_is_closed)
            {
                door_is_closing=false;
            }
        }
        // <==

        // ==> control door depending on door status
        if (door_is_open)
        {
            // check if door has been open for specified time
            if ((millis()-millis_door_opened>timeframe_door_idle)&(millis_door_opened!=0))
            {
                // close door
                close_door();
                door_is_open=false;
                // reset open door time tracker
                millis_door_opened=0;
            }
            // check if door is open and waiting
            // door is open since 50ms
            else if ((millis()-millis_door_opened>50) & (millis_door_opened!=0))
            {
                // wait until door has been open for the wished duration
            }
            else // door has just been opened (not even 50ms ago)
            {
                close_supply_valve();
                // log time when door has been opened
                millis_door_opened=millis();
            }
        }
        if (door_is_closed)
        {
            // check if door has been closed for specified time
            if ((millis()-millis_door_closed>timeframe_door_idle) & (millis_door_closed!=0))
            {
                // start new cycle
                open_door();
                door_is_closed=false;
                // reset variables for new cycle
                millis_door_closed=0;
                millis_cycle_start=millis(); // track beginning time of cycle
                duration_send=false; // for this new cycle
            }
            // check if door is closed and waiting
            // door is closed since 50ms
            else if ((millis()-millis_door_closed>50) & (millis_door_closed!=0))
            {
                // wait until door has been closed for the wished duration
            }
            else // door has just been closed (just 50ms ago)
            {
                close_supply_valve();
                // log time when door  has been closed
                millis_door_closed=millis();
            }
        // <==

        // ==> read sensor data
        // Requirment: read sensors every x seconds in a timeframe of y seconds
        // since the start of the cycle

        // check if still in measurement timeframe since cycle started
        if (millis()-millis_cycle_start<timeframe_measurement)
        {
            // check if its been x sec since last sensor reading
            if (millis()-millis_last_reading>sensor_sampling_rate)
            {
                p=get_pressure();

                // update lsm6ds33 events
                lsm6ds33.getEvent(&accel, &gyro, &temp); 
                // read acceleration and gyro values
                float ax=accel.acceleration.x;
                float ay=accel.acceleration.y;
                float az=accel.acceleration.z;
                float gx=gyro.gyro.x;
                float gy=gyro.gyro.y;
                float gz=gyro.gyro.z;

                // update variables
                millis_last_reading=millis();
                new_readings_available=true;
            }
        }
        // <==

        // ==> send sensor readings
        // check if new sensor readings are available
        if (new_readings_available)
        {
            // check if Pi is ready to recieve sensor readings
            // (while Pi is ready for that it keeps sending "1")
            char ready_for_sensor_reading=Serial.read();
            if (ready_for_sensor_reading=="1")
            {
                // Pi is ready to recieve readings
                Serial.write(p);
                Serial.write(ax);
                Serial.write(ay);
                Serial.write(az);
                Serial.write(gx);
                Serial.write(gy);
                Serial.write(gz);

                // update variables
                new_readings_available=false;
            }
        }
        // <==

        // ==> send duration of cycle
        // the duration is only send when one cycle ended aka when the door
        // closed. At this point the sensor reading may still continue
        if (door_is_closed) & !(duration_send)
        {
            // check if Pi is ready to recieve duration value
            // Pi is only ever sending 2 while it is waiting for the duration
            char ready_t=Serial.read();
            if (ready_t=="2")
            {
                // pi is ready
                // calc and send duration to pi
                t=millis()-millis_cycle_start 
                Serial.write(t)
                duration_send=true;
            }
            // use variable to not send duration more than once while door
            // yis closed
            bool duration_send=true;
        }
        // <==
    }
}

void loop()
{
    // this loop does nothing except holding the arduino idle after the
    // experiment is completed. In order to rerun the experiment
    // the arduino needs to be reset.
    delay(1000);
}