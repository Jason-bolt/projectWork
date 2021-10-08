from machine import Pin, ADC
import time

pH = ADC(Pin(33))
gradient = -0.114285714
intercept = 29.771428522


while True:
    voltage = (gradient * pH.read()) + intercept
    print("With intercept:", voltage)
    print("Raw analog:", pH.read())
    time.sleep_ms(500)
