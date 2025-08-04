# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
from machine import ADC
from machine import Pin
from machine import PWM
from time import sleep

ADC_MAX = 65_535

ldr = ADC(Pin(28))
led = PWM(Pin(2))


while True:
    ldr_value = ldr.read_u16()

    x = int(((ldr_value-12_000)/50_000)*ADC_MAX)
    led.duty_u16(x)
    print(x)

    sleep(0.01)
