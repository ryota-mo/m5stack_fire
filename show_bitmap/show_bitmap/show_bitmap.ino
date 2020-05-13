#include <M5Stack.h>

// int x = 64, y = 160;
// uint8_t rx_buffer[20480];
// uint16_t rx_buffer2[10240]; // 10240 = 160 * 64

int x = 160, y = 120;  // QVGA = 320*240
uint8_t rx_buffer[38400];
uint16_t rx_buffer2[19200]; // 19200 = 160 * 120


uint16_t color;

void setup() {
  M5.begin();
  // M5.Axp.ScreenBreath(13);

  Serial.begin(115200);
  Serial2.begin(921600, SERIAL_8N1, 16, 17);
  M5.Power.begin();
}

int i = 0;

void loop() {
  if(i%15==0){
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0,10);
    }

  
  if (Serial2.available()> 0) {
    int rx_size = Serial2.readBytes(rx_buffer, 2*x*y);
    Serial.println("RX!");

    for(int i = 0; i < x*y; i++){
      rx_buffer2[i] = (rx_buffer[2*i] << 8) + rx_buffer[2*i+1];
    }
    M5.Lcd.drawBitmap(0, 0, x, y, (uint16_t*)rx_buffer2);
    // M5.Lcd.drawBitmap(0, 0, x, y, rx_buffer2);
  }else{
    Serial.println("Receive Nothing from UnitV");
    M5.Lcd.println("Receive Nothing from UnitV");
    }
  i++;
  M5.update();
  delay(1);
}
