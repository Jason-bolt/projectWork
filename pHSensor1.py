from machine import Pin, ADC
import time

calibration = 0.00
analogInPin = ADC(Pin(33))
sensorValue = 0
buf = [] # int

while True:
    for i in range(0, 10):
#         print(buf[i])
        buf.append(analogInPin.read())
        print(analogInPin.read())
        time.sleep_ms(30)
    print("end")
    for i in range(0, 9):
        for j in range(i+1, 10):
            if (buf[i] > buf[j]):
                temp = buf[i]
                buf[i] = buf[j]
                buf[j] = temp
    
    avgValue = 0
    for i in range(2, 8):
        avgValue += buf[i]
    pHVol = (avgValue * 5.0) / 4095 / 6
    phValue = -5.70 * pHVol + calibration
    
    print("Sensor =", phValue)