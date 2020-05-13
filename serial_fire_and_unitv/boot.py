import image
import lcd
import sensor
import sys
import time
import KPU as kpu
from fpioa_manager import *

import KPU as kpu


lcd.init()
lcd.rotation(2)

try:
    from pmu import axp192
    pmu = axp192()
    pmu.enablePMICSleepMode(True)
except:
    pass

try:
    img = image.Image("/sd/startup.jpg")
    lcd.display(img)
except:
    lcd.draw_string(lcd.width()//2-100,lcd.height()//2-4, "Error: Cannot find start.jpg", lcd.WHITE, lcd.RED)


task = kpu.load("/sd/56757a56e89ddd8e_mbnet10_quant.kmodel")

labels=["1","2","3","4"] #You can check the numbers here to real names.

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.run(1)

lcd.clear()

# ここから下をコピペして書き換える
# for serial
from machine import UART
fm.register(34, fm.fpioa.UART1_TX, force=True)
fm.register(35, fm.fpioa.UART1_RX, force=True)
uart = UART(UART.UART1, 9600,8,0,0, timeout=1000, read_buf_len=4096)
# end for serial

# for button
from Maix import GPIO
from modules import ws2812
class_ws2812 = ws2812(8, 1)
fm.register(18, fm.fpioa.GPIO1)
ButonA=GPIO(GPIO.GPIO1, GPIO.IN, GPIO.PULL_UP)
fm.register(19, fm.fpioa.GPIO2)
ButonB=GPIO(GPIO.GPIO2, GPIO.IN, GPIO.PULL_UP)
# end for button


# LED color
led_color_list = [(50,0,10), (0,50,50)]
led_color_mode = 0
b = class_ws2812.set_led(0, led_color_list[led_color_mode])
b = class_ws2812.display()
# end LED color

is_uart_mode = False

while(True):
    img = sensor.snapshot()
    fmap = kpu.forward(task, img)
    plist=fmap[:]
    pmax=max(plist)
    max_index=plist.index(pmax)
    a = lcd.display(img)
    msg_to_fire = ""
    if pmax > 0.95:
        lcd.draw_string(40, 60, "Accu:%.2f Type:%s"%(pmax, labels[max_index].strip()))
        print(labels[max_index])  # Print the identified object name through the serial port.
        msg_to_fire += "Object detected: " + str(labels[max_index])
    else:
        print("No objects detected")
        msg_to_fire += "No objects detected"

    if is_uart_mode:
        uart.write(msg_to_fire + "\n")
        uart.read()

    if ButonA.value() == 0:
        # if ButonA is pushed, change color of LED and change serial mode
        led_color_mode = (led_color_mode + 1) % 2
        b = class_ws2812.set_led(0, led_color_list[led_color_mode])
        b = class_ws2812.display()
        is_uart_mode = not is_uart_mode

    if ButonB.value() == 0:
        # memo
        pass

a = kpu.deinit(task)
