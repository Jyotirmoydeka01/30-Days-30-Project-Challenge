import serial
import pyvjoy

SERIAL_PORT = 'COM4'
BAUD_RATE = 9600

# Setup serial and vJoy
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
vj = pyvjoy.VJoyDevice(1)

def map_value(val, in_min, in_max, out_min, out_max):
    return int((val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

while True:
    try:
        line = ser.readline().decode().strip()
        if line:
            # Expected: 4 analogs + 3 digitals
            values = list(map(int, line.split(',')))
            if len(values) == 7:
                # Set analog axes
                for i in range(4):
                    vj.set_axis(pyvjoy.HID_USAGE_X + i, map_value(values[i], 0, 1023, 0, 32768))
                
                # Set buttons
                vj.set_button(1, values[4])  # Button 1
                vj.set_button(2, values[5])  # Button 2
                vj.set_button(3, values[6])  # Button 3
    except Exception as e:
        print("Error:", e)
