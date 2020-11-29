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
import serial
import csv

# ==> set up variables
TIMEFRAME=30 # duration of every datapoint, cycle duration must be shorter
STEP=0.2 # time in s between two sensor readings
values_count=TIMEFRAME//STEP # number of readings for each values during a cycle
values=[] # holds current sensor readings
measurements={}
feature_count=500 
# TODO add serial device name for Arduino
ser=serial.Serial('', 115200, timeout=1)
ser.flush()
csv_file="data.csv"
# <==

# => set up header in csv file
header=["id"]
# add column header for all presure, accel and gyro values
readings=["p","ax","ay","az","gx","gy","gz"] # values read from sensors
for reading in readings:
    for i in range(feature_count):
        header.append(f"{reading}_{i}")
header.append("t") # column holds duration of cycle

# write header to csv file
with open(csv_file,'a') as f:
        writer=csv.writer(f,delimiter=",")
        writer.writerow(header)
# <=
# MAINLOOP
while True:
    # => recieve sensor values and store them
    # signal Arduino that Pi is ready to recieve sensor values
    ser.write(str(1)).encode('utf-8') # 1 means Pi is ready
    # check if Arduino is ready to transmit data
    if ser.in_waiting>0:
        # Arduino is ready to transmit
        # Arduino transmits every value individualy p,ax,ay,az,gx,gy,gz,(t)
        # every values is send in a line of its own
        p=ser.read_until().decode('utf-8')
        measurements['p'].append(p)

        ax=ser.read_until().decode('utf-8')
        measurements['ax']=ax

        ay=ser.read_until().decode('utf-8')
        measurements['ay']=ay

        az=ser.read_until().decode('utf-8')
        measurements['az']=az

        gx=ser.read_until().decode('utf-8')
        measurements['gx']=gx

        gy=ser.read_until().decode('utf-8')
        measurements['gy']=gy

        gz=ser.read_until().decode('utf-8')
        measurements['gz']=gz
    # <=

    # => recieve duration of cycle
    # signale arduino that all sensor values for this cycle are available
    # except duration!!
    # signal Arduino that Pi is ready to recieve duration if all measurements
    # for this datapoint are ready to be written to the csv file
    if len(measurements['p'])==values_count:
        ser.write(str(2).encode('utf-8')) # 2 means all values there, ready to recieve duration
        if ser.in_waiting>0:
            # duration gets send from the arduino
            t=ser.read_until().decode('utf-8')

            # => write datapoint to csv
            # prepare row. combine all lists of measurements to one list
            row=[]
            for key in measurements.keys():
                row.append(measurements[key])
            row.append(t)
            # insert new datapoint in csv
            with open(csv,'a') as f:
                writer=csv.writer(f)
                writer.writerow(row)
            # empty measurments lists for next datapoint
            measurements={}
            # <=
    # <=