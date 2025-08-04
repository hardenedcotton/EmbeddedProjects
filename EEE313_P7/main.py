# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
from machine import Pin
from machine import PWM
from time import sleep

ADC_MAX = 65_535

buzzerPIN = 16
buzzer = PWM(Pin(buzzerPIN))
buzzer.duty_u16(int(ADC_MAX/10))

while True:

        buzzer.freq(500)
        print('low')
        sleep(1)
        buzzer.freq(800)
        print('high')
        sleep(1)
