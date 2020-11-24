"""
======= What? =======
- This script reads serial input from arduino and logs it into an csv file.

- The input comes in regular intervalls. jedes Mal nachdem Sensorwerte aufgenommen wurden, werden sie direkt an Pi gesendet. (1xDruck, 3xBeschleunigung, 3xRot.-Gesch.)
Wenn ein Zyklus beendet wurde wird zusätzlich die Dauer des Zykluses übermittelt.

- input werte werden einzelnt der Reihe nach übermittelt

- inputs werden in Listen bzw. Variable für die Zeitdauer für jeden Datenpunt gesammelt

- eine zu beginn initierte csv Datei liegt im selben Verzeichnis wie dieses Skript

- immer wenn eine definierte Anzahl an Werten je Messwert vorhanden ist, wird eine neue Zeile/Datenpunkt in die csv-Datei geschrieben. Die Variablen werden anschließend geleert.

- Die Werte werden in einer festgelegten Reihenfolge übertragen:
    1. Druck
    2. Beschl. x
    3. Beschl. y
    4. Beschl. z
    5. Rotgesch. x
    6. Rotgesch. y
    7. Rotgesch. z
    8. Dauer

======= How? =======
- eine Funktion liest alle aktuellen Werte der Sensoren für einen Zeitpunkt aus und legt sie in der selben Reihenfolgen in ein einer Liste ab

- eine Funktion sortiert die Werte aus der Liste der aktuellen Werte in die entsprechenden Liste von Sensorwerten für diesen Datenpunkt ein

- eine Funktion schreibt alle gesammelten Werte für einen Datenpunkt aka einen Zyklus in die csv-Datei

-

======= Next =======
Do it, test it, repeat.
"""
#===============================================================================
import serial
import csv
"""
function read_values
    pass in: nothing
    while not all values have been send
        assign read line from serial input to variable
        put values at the end of a list
    return: list of values [p,ax,ay,az,gx,gy,gz,(t)]
endfunction
"""
def read_values():
    """Uses a serial object to read values while data is coming in.
    returns a list of values recieved.
    Read input need to be floats.
    Only recieves sensor values, not cycle time!"""
    values=[]
    global ser
    # arduino needs to send seven values for every sensor reading
    while ser.in_waiting>0:
        value=ser.readline().decode('utf-8')
        values.append(value)
        # only read next values in line if not all values for current sensor
        # reading have been collected
        if len(values)<7:
            continue
        else:
            break
    return values

""" 
function collect_values()
    pass in: nothing
    append pressure value to list of pressure values
    append accel x value to  list of accel x values
    append accel y value to  list of accel y values   
    append accel z value to  list of accel z values
    append gyro x value to list of gyro x values
    append gyro y value to list of gyro y values
    append gyro z value to list of gyro z values
    
    if time value is available
        assign time value to variable
    return: nothing
endfunction
"""
def collect_mesurements(values,measurements):
    """This function takes values read from the serial input and stores them in lists of corresponding measurements. returns dict with lists of sensor values
    The order of measurements in "values" need to be rigid
    Parameters:
    values=[p,ax,ay,az,gx,gy,gz]
    measurements={'p':[],'ax':[],'ay':[],'az':[],'gx':[],'gy':[],'gz':[]}"""
    for key in measurements.keys():
        measurements[key].append(values.pop(index=0))
    return measurements

""" 
function add_datapoint()
    pass in: list of values (including duration)
    setzt alle Listen mit Sensorwerten und die Zeitdauer zu einer Liste zusammen (in der richtigen Reihenfolge)
    setze alle Werte zu einem String zusammen und trenne sie dabei je weils durch ein Komma
    füge an Ende des Strings einen Zeilen umbruch ein
    hänge strings als neue zeile an die csv Datei an
    return: nothing
"""
def add_datapoint(csv, measurements):
    """ Appends all measurements belonging to one datapoint into csv file.
    csv is the path string of the csv to store values in
    measurements is dict with lists of corresponding measuremt values"""

    # prepare row. combine all lists of measurements to one list
    row=[]
    for key in measurements.keys():
        row.append(measurements[key])

    with open(csv,'a') as f:
        writer=csv.writer(f)
        writer.writerow(row)

# MAINLOOP
# TODO add serial device name for Arduino
ser=serial.Serial('', 115200, timeout=1)
ser.flush()

measurements={'p':[],'ax':[],'ay':[],'az':[],'gx':[],'gy':[],'gz':[]}

# TODO set up csv file and create variable for it

while True:
    # wait for arduino to transfer all values
    read_values()
    collect_mesurements()
    add_datapoint()