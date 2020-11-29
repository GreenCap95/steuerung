// ***Anforderungsliste an Arduinocode***

// - Tür n-mal automatisch öffnen und schließen lassen
// - je Türzyklus Zeitreihe für Druckwerte per USB rausschicken
// - je Türzyklus Zeitreihe für Beschleunigungswerte (linear/rotatorisch) per USB rausschicken
// - Tür manuell per Knopfdruck öffnen und schließen lassen
// - Drucksensorwerte auslesen
// - B-Sensorwerte auslesen

// load libaries
#include <Adafruit_LSM6DS33.h>
#include <map>

// assign pin numbers
int button_close=2;
int button_open=3;
int switch_door_closed=5;
int switch_door_open=4;
int valve_close_door=7;
int valve_open_door=8;
int valve_supply=6;
int piep=11;

// define sensor objects
Adafruit_LSM6DS33 lsm6ds33;

// define process variables
int TIMEFRAME=30 // timeframe in s in which to make evenly spaced measurments
int STEP=0.2 // sec between two measurments. TIMEFRAME must be dividabl by STEP
int values_count=TIMEFRAME/STEP // amount of measurments per timeframe
bool button_close_pressed=false;
bool button_open_pressed=false;
bool door_is_closing=false;
bool door_is_opening=false;
bool door_is_open=false;
bool door_is_closed=false;

unsigned long last_millis;
int cycles_to_perform=2;
int cycle_counter=0;

// define containers for sensor values
list<float> cycle_accel_x;
list<float> cycle_accel_y;
list<float> cycle_accel_z;
list<float> cycle_gyro_x;
list<float> cycle_gyro_y;
list<float> cycle_gyro_z;
list<float> cycle_presure;

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

void send_sensor_values(map lsm6ds33_values){
    Serial.println(pressure_values);
    Serial.println(lsm6ds33_values[""]);
    Serial.println(acce_y);
    Serial.println(acce_z);
    Serial.println(gyro_x);
    Serial.println(gyro_y);
    Serial.println(gyro_z);
    return;
}


// Function for serial communication
void ready()
{
    // Arduino signal Pi that is it ready for serial communication
    Serial.println("ok");
    // wait for response
    while
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
    digitalWrite(valve_close_door,LOW)
}
//==============================================================================
//==============================================================================
void setup()
{
    // setup pinmodes
    pinMode(button_close, INPUT);
    pinMode(button_open, INPUT);
    pinMode(switch_door_open, INPUT);
    pinMode(switch_door_closed, INPUT);
    pinMode(valve_close_door, OUTPUT);
    pinMode(valve_open_door, OUTPUT);
    pinMode(valve_supply, OUTPUT);
    pinMode(Piep, OUTPUT);
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    digitalWrite(8,HIGH);
    
    last_millis=millis();
    int millis_cycle_start;
    int millis_cycle_end;

    sensors_event_t accel;
    sensors_event_t gyro;
    senosrs_event_t temp;

    // *****main loop*****
    while (cycle_counter<cycles_to_perform)
    {
        // update process variables
        button_close_pressed=(button_close==HIGH);
        button_open_pressed=(button_open==HIGH);
        door_is_closed=(switch_door_closed==HIGH);
        door_is_open=(switch_door_open==HIGH);

        // => control door
        if (door_is_closed)
        {
            close_supply_valve();
            door_is_closing=false;
            cycle_counter++;

            // => send cycle duration to Pi 
            // check if Pi is ready to recieve duration value
            int ready_t=Serial.read();
            if (ready_t==2);
            {
                t=millis()-millis_cycle_start 
                Serial.write(t)
            }
            open_door();
            millis_cycle_start=millis()
        }
            
        if (door_is_open)
        {
            close_supply_valve();
            door_is_opening=false;
            if ((millis()-last_millis)>500)
            {
                close_door();
            }
        }
        // <=

        
        // while the door is not yet closed again...
        if (door_is_closing or door_is_opening or door_is_open)
        {
            // ...read sensor values every 0.2s
            if ((millis()-last_millis)>200)
            {
                // => read sensors
                // read pressur value

                // update lsm6ds33 events
                lsm6ds33.getEvent(&accel, &gyro, &temp); 
                // read acceleration and gyro values
                float ax=accel.acceleration.x;
                float ay=accel.acceleration.y;
                float az=accel.acceleration.z;
                float gx=gyro.gyro.x;
                float gy=gyro.gyro.y;
                float gz=gyro.gyro.z;
                // <=

                // => send sensor values to Pi
                // check if Pi is ready to recieve sensor values
                int pi_ready=Serial.read(); // ready==1 means ready
                if (pi_ready==1)
                // ... send sensor values and duration if available
                {
                    Serial.write(p);
                    Serial.write(ax);
                    Serial.write(ay);
                    Serial.write(az);
                    Serial.write(gx);
                    Serial.write(gy);
                    Serial.write(gz);
                }
                // <=
            }
        }
    }
}

void loop()
{
    // this loop does nothing except holding the arduino idle after the
    // experiment is completed. In order to rerun the experiment
    // the arduino needs to be reset.
    delay(1000);
}