// load libaries
// #include <Adafruit_LSM6DS33.h>

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
// Adafruit_LSM6DS33 lsm6ds33;

// define process variables
// int TIMEFRAME=30; // timeframe in s in which to make evenly spaced measurments
// int STEP=0.2; // sec between two measurments. TIMEFRAME must be dividabl by STEP
// int values_count=TIMEFRAME/STEP; // amount of measurments per timeframe
// bool button_close_pressed=false;
bool button_open_pressed=false;
bool door_is_closing=false;
bool door_is_opening=false;
// bool door_is_open=false;
// bool door_is_closed=false;
// bool new_readings_available=false;

// time relatet variables
// unsigned long millis_last_reading; // time by then  sensors had been read last
// unsigned long millis_door_opened=0; // time by then door is completly open
// unsigned long millis_door_closed=0; // time by then door is completly closed
// int millis_cycle_start;
// int millis_cycle_end;
// int timeframe_door_idle=2000; // ms door stays opened or closed
// int timeframe_measurement=15000; // timeframe while sensors are read
// int sensor_sampling_rate=200; // sensor values are read every x ms

// int cycles_to_perform=2;
// int cycle_counter=0;


// ==> Funcitons to controll door movement
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
// <== Functions to controll door movement



// ==> Functions to controll valves
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
// <== Functions to controll valves

// ==> Functions for sensor readings
float get_pressure()
{
    // get number corresponding to voltage coming from pressur sensor
    int sensor_value=analogRead(pressure_sensor_pin); // between 0 and 1023
    // calc pressur in MPa
    // pressure range is 0..10bar, UNIT USED Pa (1Mpa=10bar)
    float pressure=sensor_value*(1000000.0/1023.0);
    return pressure;
}
// <== Functions for sensor readings
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
    digitalWrite(valve_supply,HIGH);
    digitalWrite(valve_close_door,HIGH);
    digitalWrite(valve_open_door,HIGH);
    
    float p;

    route_air_open();
}

void loop()
{   
    button_open_pressed=(digitalRead(button_open)==HIGH)
    if (button_open_pressed){
        open_supply_valve();
    }
}