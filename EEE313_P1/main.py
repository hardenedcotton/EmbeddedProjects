# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
import collections
from time import sleep

from machine import Pin

led = Pin('LED', Pin.OUT)
pins = {
    'p0': 0,
    'p1': 1,
    'p2': 2,
    'p3': 3,
    'p4': 4,
    'p5': 5,
    'p6': 6,
    'p7': 7,
    }

pins_ordered = collections.OrderedDict(sorted(pins.items()))
pins_reversed = collections.OrderedDict(sorted(pins.items(), reverse=True))

for pin in pins_ordered:
    pins[pin] = Pin(pins[pin], Pin.OUT)
    pins[pin].off()
print(pins_ordered, reversed(pins_ordered))
while True:
    for pin in pins_ordered:
        print(pin)
        pins[pin].on()
        sleep(.15)
        pins[pin].off()
    for pin in pins_reversed:
        print(pin)
        pins[pin].on()
        sleep(.15)
        pins[pin].off()
