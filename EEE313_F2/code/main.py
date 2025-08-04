# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
'''

7SEG:   10 countdown
Button: Initianlize countdown
RGBLED: Turn green to red as countdown
Buzzer: Beep per seconde
Motor:  Chronometer
Pot:    Pace of the countdown
LDR:    Frequency of beep

'''

import collections
from time import sleep

from machine import ADC, PWM, Pin
import rgb


ADC_MAX = 65_535
SECONDS = 10

L_RANGE = 1700
H_RANGE = 8700
step = int((H_RANGE-L_RANGE)/SECONDS)

button = Pin(9, Pin.IN, Pin.PULL_UP)
vcc = Pin(1, Pin.OUT).on

pot = ADC(Pin(28))
pot_gnd = Pin(27, Pin.OUT)
ldr = ADC(Pin(26))

led = Pin(25, Pin.OUT)

servo = PWM(Pin(0))
servo.freq(50)

buzzer = PWM(Pin(14))


disp_pins = {
    'g': 2,
    'f': 3,
    'a': 4,
    'b': 5,
    'e': 6,
    'd': 7,
    'c': 8,
}

disp_dict = {
    '0': ['a', 'b', 'c', 'd', 'e', 'f'],
    '1': ['b', 'c'],
    '2': ['a', 'b', 'd', 'e', 'g'],
    '3': ['a', 'b', 'c', 'd', 'g'],
    '4': ['b', 'c', 'f', 'g'],
    '5': ['a', 'c', 'd', 'f', 'g'],
    '6': ['a', 'c', 'd', 'e', 'f', 'g'],
    '7': ['a', 'b', 'c'],
    '8': ['a', 'b', 'c', 'd', 'e', 'f', 'g'],
    '9': ['a', 'b', 'c', 'd', 'f', 'g'],
}

disp_ordered = collections.OrderedDict(sorted(disp_dict.items()))


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


def clear_disp_pins():
    for i in disp_pins:
        disp_pins[i].off()


def set_servo_cycle(position):
    servo.duty_u16(position)
    sleep(0.01)


def set_disp_pins():
    for i in disp_pins:
        disp_pins[i] = Pin(disp_pins[i], Pin.OUT)


set_disp_pins()


def get_speed():
    secs = 0.01 + analog_average(pot)/ADC_MAX
    sleep(0.01)
    return secs


def beep():
    # ldr
    buzzer.duty_u16(int(ADC_MAX/10))
    frequency = int(((analog_average(ldr)-12_000)/50_000)*1300)+500
    buzzer.freq(frequency)
    sleep(get_speed()*0.2)
    buzzer.duty_u16(0)


while True:
    # button
    pos = H_RANGE-1
    set_servo_cycle(H_RANGE)
    clear_disp_pins()
    Pin(11).off()
    led.toggle()
    sleep(get_speed()*0.1)
    if (not button.value()):
        for sec in range(SECONDS):
            # 7seg-C
            clear_disp_pins()
            for ch in disp_dict[str(sec)]:
                disp_pins[ch].on()

            # servo
            if pos in range(L_RANGE, H_RANGE):
                set_servo_cycle(pos)
                pos -= step

            # rgb-A
            hue = 1-(sec/SECONDS)
            rgb.light_rgb(10, 11, 12, 13, hue)

            # buzzer & ldr
            beep()

            # pot
            sleep(get_speed()*0.8)

            print(sec)
