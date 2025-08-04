# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
import collections
from time import sleep
from machine import Pin

pins = {
    'g': 0,
    'f': 1,
    'a': 2,
    'b': 3,
    'e': 4,
    'd': 5,
    'c': 6,
    }

disp = {
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
    'A': ['a', 'b', 'c', 'e', 'f', 'g'],
    'B': ['c', 'd', 'e', 'f', 'g'],
    'C': ['a', 'd', 'e', 'f'],
    'D': ['b', 'c', 'd', 'e', 'g'],
    'E': ['a', 'd', 'e', 'f', 'g'],
    'F': ['a', 'e', 'f', 'g'],
    }

disp_ordered = collections.OrderedDict(sorted(disp.items()))

def clear_pins():
    for i in pins:
        pins[i].off()

for i in pins:
    pins[i] = Pin(pins[i], Pin.OUT)

while True:
    for v, k in disp_ordered.items():
        clear_pins()
        print(f'Character is {v}')
        for ch in k:
            pins[ch].on()
        sleep(0.5)

