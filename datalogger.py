"""
This script run reads serial input from arduino and logs it into an csv file.

solange ein input kommt sollten datenpunkte in die csv Datei geschrieben werden
stell

input:
jedes Mal nachdem Sensorwerte aufgenommen wurden, werden sie direkt an Pi gesendet. (1xDruck, 3xBeschleunigung, 3xRot.-Gesch.)
Wenn ein Zyklus beendet wurde wird zusätzlich die Dauer des Zykluses übermittelt.

Solange ein Zyklus noch nicht beendet wurde werden die zusammen gehörigen Werte in einzelnen Listen gesammelt. Die Werte werden in einer festgelegten Reihenfolge übertragen:
1. Druck
2. Beschl. x
3. Beschl. y
4. Beschl. z
5. Rotgesch. x
6. Rotgesch. y
7. Rotgesch. z
8. Dauer

Werden die Werte in dieser Reihenfolge verschickt, können sie in der gleichen REihenfolge wieder ausgelesen werden. (Denke ich zumindest)

Wenn das Signal übermittelt wird, dass ein Zyklus beendet wurde, werden die Daten in der richtigen Reihenfolge in eine neue Zeile in der csv Datei geschrieben.



"""
"""
function read_values
    pass in: nothing
    while values are available
        put values at the end of a list
    return: list of values [p,ax,ay,az,gx,gy,gz,(t)]
endfunction
"""

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

""" 
function add_datapoint()
    pass in: list of values (including duration)
    setzt alle Listen mit Sensorwerten und die Zeitdauer zu einer Liste zusammen (in der richtigen Reihenfolge)
    setze alle Werte zu einem String zusammen und trenne sie dabei je weils durch ein Komma
    füge an Ende des Strings einen Zeilen umbruch ein
    hänge strings als neue zeile an die csv Datei an
    return: nothing
"""
