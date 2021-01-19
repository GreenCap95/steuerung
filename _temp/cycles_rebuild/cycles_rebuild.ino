int switch_door_open=4;
int switch_door_closed=5;
int valve_supply=6;
int valve_close_door=7;
int valve_open_door=8;
int button_open=3;

bool door_is_closing=false;
bool door_is_opening=false;
bool door_is_open=false;
bool door_is_closed=false;
bool button_open_pressed=false;

int TIMEFRAME=30;   // sec; new cycle should not start before this time
                    // frame has passed, thus ensuring each cycle has the same
                    // amount of samples features measured
unsigned long millis_when_door_opened=0; // time by when door is completly open
unsigned long millis_when_door_closed=0; // time by when door is completly closed
int millis_count_door_idle=2000;       // ms door stays opened or 
int millis_cycle_start;

int cycles_to_perform=2;
int cycle_counter=0;

// ==> Funcitons to controll door movement
void open_door(){
    route_air_open();
    open_supply_valve();
    door_is_opening=true;
}

void close_door(){
    route_air_close();
    open_supply_valve();
    door_is_closing=true;
}
// <==

// ==> Functions to controll valves
void open_supply_valve(){
    digitalWrite(valve_supply,LOW);
}

void close_supply_valve(){
    digitalWrite(valve_supply,HIGH);
}

void route_air_open(){
    // pull valve in "open door" position
    digitalWrite(valve_open_door,LOW);
}

void route_air_close(){
    // pull valve in "close door" position
    digitalWrite(valve_close_door,LOW);
}

void reset_valves(){
    close_supply_valve();
    // deactivate both magnets
    digitalWrite(valve_open_door,HIGH);
    digitalWrite(valve_close_door,HIGH);
}
// <==

void setup()
{
    // setup pinmodes
    pinMode(switch_door_open, INPUT);
    pinMode(switch_door_closed, INPUT);
    pinMode(valve_close_door, OUTPUT);
    pinMode(valve_open_door, OUTPUT);
    pinMode(valve_supply, OUTPUT);
    pinMode(button_open,INPUT);

    // setup serial connection
    Serial.begin(9600);

    // make sure supply valve is shut at first
    Serial.println("reset valves");
    reset_valves();

    // wait to start until button "open" is pressed
    while (!button_open_pressed)
    {
        button_open_pressed=(digitalRead(button_open)==HIGH);
    }
    Serial.println("START");
    
    // ==> start first cycle
    // door is still closed yet
    open_door();
    millis_cycle_start=millis();
    delay(1000);
    // <==

    // *****main loop*****
    // loop until the last cycle has ended (aka door closed)
    while (true){
        // update door status
        // just one or none can be true at the same time
        door_is_open=(digitalRead(switch_door_open)==HIGH);
        door_is_closed=(digitalRead(switch_door_closed)==HIGH);

        if (door_is_open){
            Serial.println("Door is open");
            reset_valves();
            delay(500);
            Serial.println("close door");
            close_door();
            delay(1000); // wait so switch gets deactivatet
            door_is_open=false;
        }
        if (door_is_closed){
            Serial.println("Door is closed");
            reset_valves();
            delay(500);
            cycle_counter++;
            // check if there is another cycle to perform
            if (cycle_counter<cycles_to_perform){
                Serial.println("open door");
                open_door();
                delay(1000); // wait so switch gets deactivatet
                door_is_closed=false;
            }
            else{
                // all demanded cycles have been performed
                Serial.println("all cycles completed");
                close_supply_valve(); // restate default position
                route_air_close();
                break;
            }
        }
    }
    Serial.println("end of main loop");
}

void loop()
{
    // this loop does nothing except holding the arduino idle after the
    // experiment is completed. In order to rerun the experiment
    // the arduino needs to be reset.
    delay(1000);
} 