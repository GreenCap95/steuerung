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

void send_datapoint(float pressure_values[],float acce_x[],float acce_y[],
float acce_z[], float gyro_x[], float gyro_y[], float gyro_z[], int duration){
    Serial.write(pressure_values);
    Serial.write(acce_x);
    Serial.write(acce_y);
    Serial.write(acce_z);
    Serial.write(gyro_x);
    Serial.write(gyro_y);
    Serial.write(gyro_z);
    Serial.write(duration);
    return;
}

map read_lsm6ds33()
{
    sensors_event_t accel;
    sensors_event_t gyro;
    senosrs_event_t temp;
    lsm6ds33.getEvent(&accel, &gyro, &temp);
    map <char, float> lsm6ds33_values={
        {"accel_x",accel.acceleration.x},
        {"accel_y",accel.acceleration.y},
        {"accel_z",accel.acceleration.z},
        {"gyro_x",gyro.gyro.x},
        {"gyro_y",gyro.gyro.y},
        {"gyro_z",gyro.gyro.z},
    };
    return lsm6ds33_values;
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

    // *****main loop*****
    while (cycle_counter<cycles_to_perform)
    {
        // update process variables
        button_close_pressed=(button_close==HIGH);
        button_open_pressed=(button_open==HIGH);
        door_is_closed=(switch_door_closed==HIGH);
        door_is_open=(switch_door_open==HIGH);

        if (door_is_closing or door_is_opening or door_is_open)
        {
            // while the door is not closed yet again the data for the current
            // cycle is accumulated
            if ((millis()-last_millis)>200)
            {
                // TODO: update pressure and acceleration value lists for this
                // cycle
                // read current accelerometer values
                lsm6ds33_values=read_lsm6ds33();
                // fill value list for current datapoint/cycle
                cycle_accel_x.push_back(lsm6ds33_values["accel_x"]);
                cycle_accel_y.push_back(lsm6ds33_values["accel_y"]);
                cycle_accel_z.push_back(lsm6ds33_values["accel_z"]);
                cycle_gyro_x.push_back(lsm6ds33_values["gyro_x"]);
                cycle_gyro_y.push_back(lsm6ds33_values["gyro_y"]);
                cycle_gyro_z.push_back(lsm6ds33_values["gyro_z"]);
                // add temperature?
            }

            if (door_is_closed)
            {
                close_supply_valve();
                door_is_closing=false;
                cycle_counter++;
                if ((millis()-last_millis)>500)
                {
                    // call: sendatapoint()
                    open_door();
                }
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