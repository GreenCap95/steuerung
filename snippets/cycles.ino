int switch_door_open=5;
int switch_door_closed=4;
int valve_supply=6;
int valve_close_door=7;
int valve_open_door=8;
int button_open=3;

bool door_is_closing=false;
bool door_is_opening=false;
bool door_is_open=false;
bool door_is_closed=false;
// bool button_open_pressed=false;
bool start=false;
String input_serial;

int TIMEFRAME=10000;   // sec; new cycle should not start before this time
                    // frame has passed, thus ensuring each cycle has the same
                    // amount of samples features measured
unsigned long millis_when_door_opened=0; // time by when door is completly open
unsigned long millis_when_door_closed=0; // time by when door is completly closed
int MILLIS_COUNT_DOOR_IDLE = 2000;       // ms door stays opened or
int millis_cycle_start;

int cycles_to_perform = 3;
int cycle_counter=0;

// ==> Funcitons to controll door movement
void open_door(){
    route_air_open();
    open_supply_valve();
}

void close_door(){
    route_air_close();
    open_supply_valve();
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
    Serial.begin(57600);

    // make sure supply valve is shut at first
    Serial.println("reset valves");
    reset_valves();

    // wait to start until button "open" is pressed
    Serial.println("Input 'start' to start");
    while (!start)
    {
        if (Serial.available())
        {
            input_serial=Serial.readStringUntil('\n');
            if (input_serial.equals("start"))
            {
                start=true;
            }
        }
    }
    Serial.println("START");
    
   

    // *****main loop*****
    // loop until the last cycle has ended (aka door closed)
    while (cycle_counter<cycles_to_perform){
        // ==> start first cycle
        // door is still closed yet
        open_door();
        door_is_opening=true;
        millis_cycle_start=millis();
        // <==

        // loop for excatly x seconds for each cycle so the number of datapoints
        // collectet is the same for all cycles
        while (millis()-millis_cycle_start<TIMEFRAME)
        {
            // read sensor values here, but not yet!

            // update door status
            // just one or none can be true at the same time
            door_is_open=(digitalRead(switch_door_open)==HIGH);
            door_is_closed=(digitalRead(switch_door_closed)==HIGH);

            // trigger new movement if the door has been given time to settle
            // after it's been opened/closed
            if ((millis()-millis_when_door_opened>MILLIS_COUNT_DOOR_IDLE)&(millis_when_door_opened!=0))
            {
                close_door();
                door_is_closing=true;
                millis_when_door_opened=0; 
            }
            if (millis_when_door_closed!=0)
            {
                // just wait for timeframe to be completed and read sensors for
                // the rest of the time
            }

            // take time when door opens or closes
            if ((door_is_open)&(door_is_opening))
            {
                //.. door just opened
                millis_when_door_opened=millis();
                reset_valves();
                door_is_opening=false;
            }
            if ((door_is_closed)&(door_is_closing))
            {
                //.. door just closed
                millis_when_door_closed=millis();
                reset_valves();
                
                door_is_closing=false;
            }
        }
        Serial.println("Increment counter");
        cycle_counter++;
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
