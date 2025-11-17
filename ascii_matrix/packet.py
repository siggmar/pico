import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200)
time.sleep(2)

packet = b'A' * 64 # 64 bytes, test

ser.write(packet)
print("Data send")
