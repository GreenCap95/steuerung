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