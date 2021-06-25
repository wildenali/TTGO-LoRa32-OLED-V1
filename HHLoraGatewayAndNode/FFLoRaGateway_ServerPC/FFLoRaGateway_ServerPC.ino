#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include "SSD1306Wire.h" // https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/SSD1306Wire.h

//#define MISO    19   // GPIO19 -- lora MISO
//#define MOSI    27   // GPIO27 -- lora MOSI
//#define SS      18   // GPIO18 -- lora CS

#define SCK     5    // GPIO5  -- lora SCK
#define RST     23   // GPIO23 atau GPIO12 atau GPIO14 -- RESET (If Lora does not work, replace it with GPIO14)
#define DI0     26   // GPIO26 -- IRQ(Interrupt Request)
#define BAND    915E6

/*-----replace default pin  OLED_SDA=4, OLED_SCL=15 with  OLED_SDA=21, OLED_SCL=22----*/
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16

// #define LED_BUILTIN 25 /*--LED bawaan LoRa-----*/

// #define OFF 0  /*----untuk tes LED-------------*/
// #define ON 1

const int blueLED = 25; 
int counter = 0;
String statusKonveyor;
int pinSensorKonveyor = 15;
//String message;

/*-----Initialize the OLED display using Wire library----*/
SSD1306Wire  display(0x3c, OLED_SDA, OLED_SCL); // OLED_SDA=4, OLED_SCL=15

// String id, Data, Data1, Data2;
bool kirimBalik;
String dataKirimBalik = "";

void setup() {
/*---------START  Oled ******** ------*/
  // pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);    /*-----set GPIO16 low to reset OLED----------------------*/
  delay(50);
  digitalWrite(OLED_RST, HIGH);   /*-----while OLED is running, must set GPIO16 in high----*/
  
  /*-----Initialising the UI will init the display too.------------------------------------*/
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  /*-----clear the display-----------------------------------------------------------------*/
  display.clear();
/*----------END Oled ******** ---------*/

  Serial.begin(115200);
  while (!Serial); //If just the the basic function, must connect to a computer

//  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(BAND)) 
      {
        Serial.println("Starting LoRa failed!");
        while (1);
      }

  Serial.println("LoRa Initial OK!");
  pinMode(pinSensorKonveyor, INPUT_PULLUP);

  LoRa.onReceive(onReceive);
  LoRa_rxMode();
}

void loop() {
/*------display Oled-------------*/ 
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "SeTiYo Gateway");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 42, "Get: " + dataKirimBalik);
  display.display();
  display.clear();

  callbackMessage(kirimBalik, dataKirimBalik);
  
  while (Serial.available()) {
    String dataInput = Serial.readStringUntil('\n');
    Serial.println(dataInput);
    LoRa_sendMessage(dataInput);
  }
  
}


void LoRa_rxMode(){
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}
 
void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signals
}
 
void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket();                     // finish packet and send it
  LoRa_rxMode();                        // set rx mode
}
 
void onReceive(int packetSize) {
  String message = "";
 
  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  
  dataKirimBalik = message;
  kirimBalik = true;
//  Serial.println(dataKirimBalik);
}

void callbackMessage(bool statusKirimBalik, String message){
  if(statusKirimBalik == true){
    kirimBalik = false;
    if(message.substring(0, 7) == "GATEWAY"){
      delay(5);
      LoRa_sendMessage(message);
      Serial.println(message);
    }
  }
}
