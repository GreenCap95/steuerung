""" 
======ALL SNIPPETS ARE MINIMAL CODES PERFORMING A CERTAIN FUNCTIONALITY=========

This code snippet is for testing if the Pi is able to to write several values to a csv file.
 """
# old data writen to the csv file will not be overwritten

import csv
SAMPLES_COUNT=60 # for testing purpose limit samples to write to csv
samples_count=0 # variable to keep track
feature_values_count=5 # number of features for each mesurement in a cycle
csv_file="data.csv"



# => set up header in csv file
header=[]
# add column header for all presure, accel and gyro values
measurements={'p':[],'ax':[],'ay':[],'az':[],'gx':[],'gy':[],'gz':[],'t':[]} # values read from sensors
for reading in measurements:
    for i in range(feature_values_count):
        header.append(f"{reading}_{i}")
header.append("t") # column holds duration of cycle
# write header to csv file
with open(csv_file,'a',newline="") as f:
    csv_writer=csv.writer(f,delimiter=",")
    csv_writer.writerow(header)
# <=

while samples_count < SAMPLES_COUNT:
    # simulate reading from the arduino
    # here its assumed that the arduino sends the sensor values in the right
    # order and the Pi is reading them correctly
    if len(measurements['p'])<feature_values_count:
        p=1
        measurements['p'].append(p)
        ax=2
        measurements['ax'].append(ax)
        ay=3
        measurements['ay'].append(ay)
        az=4
        measurements['az'].append(az)
        gx=5
        measurements['gx'].append(gx)
        gy=6
        measurements['gy'].append(gy)
        gz=7
        measurements['gz'].append(gz)    
        # <==

    # ==> recieve duration of cycle
    # to recieve duration.
    if len(measurements['p'])==feature_values_count:
        t=8
        # datapoint is missing only duration. Append to the end
        measurements['t'].append(t)
        # datapoint is now ready to be written to the csv fiel
        # <==

        # ==> write datapoint to csv
        # prepare row. combine all lists of measurements to one list
        row=[]
        for key in measurements.keys(): 
            for values in measurements[key]: # dict contains lists with values
                row.append(values)
        # row now holds all features values in correct order (see line below)
        # row=[p1,p2,...,ax1,ax2,...,gx1,...,gy1,...,t]
        # insert new datapoint in csv
        with open(csv_file,'a',newline="") as f:
            csv_writer=csv.writer(f,delimiter=",")
            csv_writer.writerow(row)
        # reset measurments lists for next datapoint
        # WARNING THIS VARAIBLE IS DEFINED TWICE (see line 57)
        measurements={'p':[],'ax':[],'ay':[],'az':[],'gx':[],'gy':[],'gz':[],'t':[]}
        # increment samples counter to limit test csv
        samples_count+=1
        # <==