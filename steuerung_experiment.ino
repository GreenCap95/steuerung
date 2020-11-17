// ***Anforderungsliste an Arduinocode***

// - Tür n-mal automatisch öffnen und schließen lassen
// - je Türzyklus Zeitreihe für Druckwerte per USB rausschicken
// - je Türzyklus Zeitreihe für Beschleunigungswerte (linear/rotatorisch) per USB rausschicken
// - Tür manuell per Knopfdruck öffnen und schließen lassen
// - Drucksensorwerte auslesen
// - B-Sensorwerte auslesen

// assign pin numbers
int button_close=2;
int button_open=3;
int switch_door_closed=5;
int switch_door_open=4;
int valve_close_door=7;
int valve_open_door=8;
int valve_supply=6;
int piep=11;

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
float acce_z[], int duration){
    // declare amount of datapoints to use of each array. Reach beyond cycle end
    int n_points=500; // ADJUST AS NEEDED!!!
    // send each values of all measurements one by one
    // send pressure values
    Serial.write(pressure_values, n_points);
    Serial.write(acce_x,n_points);
    Serial.write(acce_y,n_points);
    Serial.write(acce_z,n_points);
    Serial.write(duration);
    return;
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