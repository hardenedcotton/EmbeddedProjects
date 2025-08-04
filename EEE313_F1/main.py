# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
from time import sleep

from machine import ADC, PWM, Pin

ADC_MAX = 65_535
L_RANGE = 1800
H_RANGE = 7800
range_diff = H_RANGE - L_RANGE

gnd = Pin(27, Pin.OUT)
pot = ADC(Pin(28))

servo = PWM(Pin(0))
servo.freq(50)


def mean(arr):
    sum = 0
    for i in arr:
        sum += i
    return sum/len(arr)


def analog_average(value, N=5):
    measures = []
    for _ in range(N):
        measures.append(value.read_u16())
        sleep(0.001)
    return int(mean(measures))


def set_servo_cycle(position):
    servo.duty_u16(position)
    sleep(0.01)


while True:
    pot_value = analog_average(pot)
    pos = int(((pot_value/ADC_MAX)*range_diff)+L_RANGE)

    set_servo_cycle(pos)
    print(pos)
