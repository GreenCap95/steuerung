// ***Anforderungsliste an Arduinocode***

// - Tür n-mal automatisch öffnen und schließen lassen
// - je Türzyklus Zeitreihe für Druckwerte per USB rausschicken
// - je Türzyklus Zeitreihe für Beschleunigungswerte (linear/rotatorisch) per USB rausschicken
// - Tür manuell per Knopfdruck öffnen und schließen lassen
// - Drucksensorwerte auslesen
// - B-Sensorwerte auslesen

//***Pseudo code***

/*
initialize pin number variables for mechanical parts
initialize status variables for buttons and switches
initialize variable last millis
set cycles to wanted amount

function close_door
    pass in: nothing
    call: open_supply_valve()
    set value of pin signal to route air flow to close door
    set door is closing flag to True
    return: nothing
endfunction

function open_door
    pass in: nothing
    set value of pin signal to open supply value
    set value of pin signal to route air flow to open door
    set door is opening flag to True
    return: nothing
endfunction

function send_datapoint
    pass in: noting
    write pressure values of last cycle as serial output
    write acceleration values of last cycle as serial output                    
    write duration of last cycle as serival output
    return: nothing
endfunction

function open_supply_valve
    pass in: nothing
    set value of pin signal to open supply valve
    return: nothing
endfunction

function shut_supply_valve
    pass in: nothing
    set value of pin signal to shut supply valve
    return: nothing
endfunction

function setup
    pass in: nothing
    define pinmodes for each used pin as input or output
    set button and switch statuses to LOW
    set last millis to current time
endfunction

function loop
    pass in: nothing
    if cycle counter is less than cycles wanted
        update switch statuses

        if door is closing or if door is opening of if door is open
            // dataloging cycle ends when door is closed again
            if it has been 0.2 sec since last measurement
                update list of pressure values
                update liste of acceleration values
        
        if door is closed
            call: shut_supply_valve()
            set door is closing flag to false
            add one to cycle counter
            if door has been closed for 0.5 sec
                call: send_datapoint()
                call: open_door()

        if door is open
            if supply valve is not shut
                call: shut_supply_valve()
                set door is opening flag to false
            if door has been open for 0.5 sec
                call: close_door()
    else
        terminate program
*/

// assign pin numbers
int button_close=2;
int button_open=3;
int switch_closed=5;
int switch_open=4;
int valve_close_door=7;
int valve_open_door=8;
int valve_supply=6

// define process variables
int button_close_pressed;
int button_open_pressed;
int door_is_closing;
int door_is_opening;
unsigned long millis_last;
int cycles_to_perform=2;

void close_door(){
    open_supply_valve();
    digitalWrite(valve_supply,LOW) // supply valve opens
    digitalWrite(valve_close_door,LOW) // route air to close door
}

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

void send_datapoint(){
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