# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
from machine import ADC
from machine import Pin
from time import sleep

adc_max = 65_535

pot = ADC(Pin(28))
led = Pin(2, Pin.OUT)
gnd = Pin(27, Pin.OUT)

while True:
    pot_value = pot.read_u16()
    t = pot_value/(adc_max*2)
    led.toggle()
    sleep(t)
    freq = 1/t
    print(f'Frequency: {freq}Hz')
