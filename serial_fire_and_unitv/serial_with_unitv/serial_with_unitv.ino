#include <M5Stack.h>

void setup() {
  Serial.begin(115200);
  // UnitVとの通信ではPort Cにつなぐ 
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

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
bool communication_with_unitv = false;
bool communication_with_pc = false;

void loop() {
  if(i%15==0){
    // Fireの画面が埋まるのでリセットする
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
  }

  if ( Serial.available() > 0 ) {
    // PCとシリアル通信
    // Arduino IDEのシリアルモニタなどで見れる
    String str = Serial.readStringUntil('\n');
    Serial.print("From PC:");
    Serial.println(str);
    M5.Lcd.print("Send to PC:");
    M5.Lcd.println(str);
  }
  String str = "";
  for(int j=0; j<25; j++){
    // UnitVから何かくるまで待機
    if ( Serial2.available() > 0 ) {
      // 受信      
      str = Serial2.readStringUntil('\n');
      break;
    }
    delay(10);
  }

  if(str != ""){
    // UnitVからの受け取りに成功しているとき
    if(communication_with_pc){
      Serial.print("From UnitV:");
      Serial.println(str);
    }
    M5.Lcd.print("From UnitV:");
    M5.Lcd.println(str);
    Serial2.println(1);
  } else {
    if(communication_with_pc){
      Serial.println("Receiving Nothing from UnitV");
    }
    M5.Lcd.println("Receiving Nothing from UnitV");
    delay(250);
  }
  if (M5.BtnB.wasReleased()) {
    // BボタンでPCとの通信有無を変更
    communication_with_pc = not communication_with_pc;
  }
  i++;
  M5.update();
}
