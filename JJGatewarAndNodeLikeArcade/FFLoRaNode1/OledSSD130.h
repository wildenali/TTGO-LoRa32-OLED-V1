/*-------replace default pin  OLED_SDA=4, OLED_SCL=15 with  OLED_SDA=21, OLED_SCL=22------*/
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16
SSD1306Wire  display(0x3c, OLED_SDA, OLED_SCL); // OLED_SDA=4, OLED_SCL=15

void OledSSD130_setup(){
  /*---------START aktivas Oled-------*/
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(OLED_RST, HIGH); // while OLED is running, must set GPIO16 in high、
  /*--Initialising the UI will init the display too--------*/
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  /*--clear the display---------------*/
  display.clear();
/*---------aktivasi Oled END---------*/
}
