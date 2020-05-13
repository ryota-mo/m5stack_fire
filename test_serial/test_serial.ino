#include <M5Stack.h>

void setup() {
  Serial.begin(115200);

  // Initialize the M5Stack object
  M5.begin();
  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();

}

int i = 0;

void loop() {
  if(i%15==0){
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
  }
  if ( Serial.available() > 0 ) {
    String str = Serial.readStringUntil('\n');
    Serial.print("talk:");
    Serial.println(str);
    M5.Lcd.print("talk:");
    M5.Lcd.println(str);
  } else {
    M5.Lcd.println("Nothing");
  }
  i++;
  delay(1000);
  M5.update();
}
