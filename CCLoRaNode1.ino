#include <SPI.h>
#include <LoRa.h>

#include "SSD1306Wire.h" // https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/SSD1306Wire.h

//#define MISO    19   // GPIO19 -- lora MISO
//#define MOSI    27   // GPIO27 -- lora MOSI
//#define SS      18   // GPIO18 -- lora CS

#define SCK     5    // GPIO5  -- lora SCK
#define RST     23   // GPIO23 atau GPIO12 atau GPIO14 -- RESET (If Lora does not work, replace it with GPIO14)
#define DI0     26   // GPIO26 -- IRQ(Interrupt Request)
#define BAND    915E6

/*-------replace default pin  OLED_SDA=4, OLED_SCL=15 with  OLED_SDA=21, OLED_SCL=22------*/
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16

#define OFF 0   /*----untuk tes LED-------------*/
#define ON 1

#define LED_BUILTIN 25
const int blueLED = 25;
//const int Led_aja = 15;
const int Led_ini = 13;
int pinSensorKonveyor = 15;

String readString;
char nilai = 0;

/*------Initialize the OLED display using Wire library--------*/
SSD1306Wire  display(0x3c, OLED_SDA, OLED_SCL); // OLED_SDA=4, OLED_SCL=15

String RxDataRSSI ="";
char Str1[15];
String rssi = "";
String packet = "";

String message;

void setup() {
/*---------START aktivas Oled-------*/
  pinMode(LED_BUILTIN, OUTPUT);
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

  pinMode(blueLED, OUTPUT);  /*----untuk tes feedback LED-------------*/
//  pinMode(Led_aja, OUTPUT);
  pinMode(Led_ini, OUTPUT);
  pinMode(pinSensorKonveyor, INPUT_PULLUP);

  
  Serial.begin(115200);
  while (!Serial);
//  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);

  Serial.println("LoRa Receiver Callback");

  if (!LoRa.begin(BAND)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }     
  
  LoRa.onReceive(onReceive);
  LoRa_rxMode();

}

void loop() {
  message = "001,";    // id
  message += millis()/100;   // kirim data millis,
  message += "#";             // tanda akhir data
  LoRa_sendMessage(message);  // send a message  
/*-------------Display Oled--------------------------------*/
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Setiyo_Node_1");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 26, "Kirim Data    : " + message);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 36, "Terima Data : 001," + terimaBalik);
  display.display();
  display.clear();
 
  
}

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}
 
void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
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
  Serial.print("Node Receive: ");
  Serial.println(message);
  terimaBalik = message;
}
