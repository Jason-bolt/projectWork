from machine import Pin, ADC
import time

ph = ADC(Pin(33))

while True:
    phValue = ph.read() * (3.0 / 4095)
    print(ph.read())
    print(phValue)
    print()
    time.sleep_ms(500)