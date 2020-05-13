from machine import UART
from board import board_info
from fpioa_manager import fm
from Maix import GPIO
import sensor, lcd
from modules import ws2812


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
# sensor.set_windowing((64,160))
sensor.set_windowing((160, 120))  #QVGA = 320 * 240
sensor.set_vflip(1)
sensor.set_hmirror(1)
sensor.run(1)
sensor.skip_frames()


fm.register(34, fm.fpioa.UART1_TX, force=True)
fm.register(35, fm.fpioa.UART1_RX, force=True)

uart = UART(UART.UART1, 921600,8,0,0, timeout=1000, read_buf_len=4096)

# LED setting
class_ws2812 = ws2812(8,1)
class_ws2812.set_led(0, (50, 0, 10))
class_ws2812.display()

while(True):
    img = sensor.snapshot()
    uart.write(img)
    uart.read()
    print("In loop")
