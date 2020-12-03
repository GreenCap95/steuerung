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
STEP=0.2 # time in s between two sensor measurements
values_count=TIMEFRAME//STEP # number of measurements for each values during a cycle
values=[] # holds current sensor measurements
datapoint=[]
feature_count=500 
# TODO add serial device name for Arduino
ser=serial.Serial('', 115200, timeout=1)
ser.flush()
csv_file="data.csv"
# <==

# => set up header in csv file
header=["id"]
# add column header for all presure, accel and gyro values
measurements={'p':[],'ax':[],'ay':[],'az':[],'gx':[],'gy':[],'gz':[],'t':[]} # values read from sensors
for reading in measurements:
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

    # **** Anforderungen ****
    # - sende fortlaufend 2 während nur auf Zyklusdauer gewartet wird
    # ***********************

    # ==> recieve sensor values and store them
    # only signal Arduino that Pi is ready to recieve sensor measurements if values
    # are still missing in datapoint
    if len(measurements['p']<values_count):
        ser.write(str(1)).encode('utf-8') # 1 means Pi is ready
        # Arduino send sensor reading imediatly after the signal has been send
        # read values for serial input and store them in dict
        p=ser.read_until().decode('utf-8')
        measurements['p'].append(p)

        ax=ser.read_until().decode('utf-8')
        measurements['ax'].append(ax)

        ay=ser.read_until().decode('utf-8')
        measurements['ay'].append(ay)

        az=ser.read_until().decode('utf-8')
        measurements['az'].append(az)

        gx=ser.read_until().decode('utf-8')
        measurements['gx'].append(gx)

        gy=ser.read_until().decode('utf-8')
        measurements['gy'].append(gy)

        gz=ser.read_until().decode('utf-8')
        measurements['gz'].append(gz)    
        # <==

    # ==> recieve duration of cycle
    # if all sensor values are collectet keep signaling arduino that Pi is ready
    # to recieve duration.
    if len(measurements['p'])==values_count:
        # all sensor values for current cycle are collectet
        # signal Arduino that Pi is ready to recieve duration (send 2)
        # TODO prevent arduino buffer from overflowing with 2
        ser.write(str(2).encode('utf-8'))
        # arduino only sends duration now after it read the readies signal (2)
        # arduino now imediatly send duration
        t=ser.read_until().decode('utf-8') # waits for up to one sec for input
        # datapoint is missing only duration. Append to the end
        measurements['t'].append(t)
        # datapoint is now ready to be written to the csv fiel
    # <==

    # # ==> write datapoint to csv
    # # prepare row. combine all lists of measurements to one list
    # row=[]
    # for key in measurements.keys():
    #     row.append(measurements[key])
    # row.append(t)
    # # insert new datapoint in csv
    # with open(csv,'a') as f:
    #     writer=csv.writer(f)
    #     writer.writerow(row)
    # # empty measurments lists for next datapoint
    # measurements={}
    # # <==