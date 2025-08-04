# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
import collections
from time import sleep
from machine import Pin

button1 = Pin(26, Pin.IN, Pin.PULL_UP)
button2 = Pin(27, Pin.IN, Pin.PULL_UP)
pins = {
    'p0': 0,
    'p1': 1,
    'p2': 2,
    'p3': 3,
    }

pins_ordered = collections.OrderedDict(sorted(pins.items()))

for i in pins_ordered:
    pins[i] = Pin(pins[i], Pin.OUT)
    pins[i].off()

pin = 0
while True:

    if ((not button1.value()) and pin < 3):
        pin += 1
    if ((not button2.value()) and pin > 0):
        pin -= 1

    for i in pins:
        pins[i].off()

    p = f'p{pin}'
    print(p)
    pins[p].on()
    sleep(0.2)
