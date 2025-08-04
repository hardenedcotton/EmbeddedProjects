# type: ignore ///// This line ignores the faulty machine import due to missing micropython interpreter
import collections
import math
from machine import Pin
from machine import PWM
from time import sleep

def hsv_to_rgb(hsv_color):
    '''
    Source:
    https://github.com/Warringer/micropython-rgbled/blob/master/rgbled.py
    '''
    (h, s, v) = hsv_color
    i = math.floor(h*6)
    f = h*6 - i
    p = v * (1-s)
    q = v * (1-f*s)
    t = v * (1-(1-f)*s)

    r, g, b = [
        (v, t, p),
        (q, v, p),
        (p, v, t),
        (p, q, v),
        (t, p, v),
        (v, p, q),
    ][int(i%6)]
    r = int(255 * r)
    g = int(255 * g)
    b = int(255 * b)
    return r, g, b

def light_rgb(hue):
    [h, s, v] = [hue, 1, 1]
    [r, g, b] = hsv_to_rgb((h, s, v))
    #print('RGB: ', r, g, b)

    r = pwm_max-int((r/255)*pwm_max)
    g = pwm_max-int((g/255)*pwm_max)
    b = pwm_max-int((b/255)*pwm_max)

    print('DUTY: ', r, g, b)

    r_pin.duty_u16(r)
    g_pin.duty_u16(g)
    b_pin.duty_u16(b)
    sleep(0.05)

c_pin = Pin(14, Pin.OUT)
r_pin = PWM(Pin(2, Pin.OUT))
g_pin = PWM(Pin(4, Pin.OUT))
b_pin = PWM(Pin(6, Pin.OUT))

pins = [r_pin, g_pin, b_pin]

pwm_max = 65_536

for i in range(len(pins)):
    pins[i].freq(1_000)
    pins[i].duty_u16(pwm_max)

c_pin.on()

while True:
    for f in range(100):
        hue = f / 100.0
        light_rgb(hue)
