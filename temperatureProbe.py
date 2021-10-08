from machine import Pin, ADC
import time

temp = ADC(Pin(33))

while True:
    print(temp.read())
    time.sleep_ms(1000)