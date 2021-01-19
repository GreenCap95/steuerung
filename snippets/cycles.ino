/*
======ALL SNIPPETS ARE MINIMAL CODES PERFORMING A CERTAIN FUNCTIONALITY=========

This code snippet is for testing if the controlling of the door works as expected
*/

int switch_door_open=5;
int switch_door_closed=4;
int valve_supply=6;
int valve_close_door=7;
int valve_open_door=8;

bool door_is_closing=false;
bool door_is_opening=false;
bool door_is_open=false;
bool door_is_closed=false;

int TIMEFRAME=30;   // sec; new cycle should not start before this time
                    // frame has passed, thus ensuring each cycle has the same
                    // amount of samples features measured
unsigned long millis_when_door_opened=0; // time by then door is completly open
unsigned long millis_when_door_closed=0; // time by then door is completly closed
int millis_count_door_idle=2000;       // ms door stays opened or 
int millis_cycle_start;

int cycles_to_perform=2;
int cycle_counter=0;

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
// <==

// ==> Functions to controll valves
void open_supply_valve(){
    digitalWrite(valve_supply,LOW);
}

void close_supply_valve(){
    digitalWrite(valve_supply,HIGH);
}

void route_air_open(){
    // deactivate magnet pulling valve in "close door" position
    digitalWrite(valve_close_door,HIGH);
    // pull valve in "open door" position
    digitalWrite(valve_open_door,LOW);
}

void route_air_close(){
    // deactivate magnet pulling valve in "open door" position
    digitalWrite(valve_open_door,HIGH);
    // pull valve in "close door" position
    digitalWrite(valve_close_door,LOW);
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

    // make sure supply valve is shut at first
    close_supply_valve();    

    // ==> start first cycle
    // door is still closed yet
    open_door();
    millis_cycle_start=millis();
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
        // <== check current door status

        // ==> control door depending on door status
        if (door_is_open)
        {
            // check if door has been open for specified time
            if ((millis()-millis_when_door_opened>millis_count_door_idle)&(millis_when_door_opened!=0))
            {
                // close door
                close_door();
                door_is_open=false;
                // reset open door time tracker
                millis_when_door_opened=0;
            }
            // check if door is open and waiting
            // door is open since 50ms
            else if ((millis()-millis_when_door_opened>50) & (millis_when_door_opened!=0))
            {
                // wait until door has been open for the wished duration
            }
            else // door has just been opened (not even 50ms ago)
            {
                close_supply_valve();
                // log time when door has been opened
                millis_when_door_opened=millis();
            }
        }
        if (door_is_closed)
        {
            // check if door has been closed for specified time
            if ((millis()-millis_when_door_closed>millis_count_door_idle) & (millis_when_door_closed!=0))
            {
                // start new cycle
                open_door();
                door_is_closed=false;
                // reset variables for new cycle
                millis_when_door_closed=0;
                millis_cycle_start=millis(); // track beginning time of cycle
            }
            // check if door is closed and waiting
            // door is closed since 50ms
            else if ((millis()-millis_when_door_closed>50) & (millis_when_door_closed!=0))
            {
                // wait until door has been closed for the wished duration
            }
            else // door has just been closed (just 50ms ago)
            {
                close_supply_valve();
                // log time when door  has been closed
                millis_when_door_closed=millis();
            }
        // <==
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