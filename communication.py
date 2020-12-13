import serial

ser=serial.Serial('', 115200, timeout=1)

while True:
    # send 2 when ready to recieve values
    ser.write(str(2).encode('utf-8'))
    # check if arduino is sending values next
    if ser.read_until().decode('utf-8')==2:
        # arduino is now sending values
        a=ser.read_until().decode('utf-8')
        b=ser.read_until().decode('utf-8')
        c=ser.read_until().decode('utf-8')

        # show what pi recieved
        print(f"a={a}\t {type(a)}")
        print(f"a={b}\t {type(b)}")
        print(f"a={c}\t {type(c)}")