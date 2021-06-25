//#include <SPI.h>
#include "CountDown.h"
CountDown CD(CountDown::MILLIS);

#include <LoRa.h>
#include "SSD1306Wire.h" // https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/SSD1306Wire.h
#include "tombol.h"
#include "OledSSD130.h"

bool kirimBalik;
String dataKirimBalik = "";
#include "LoRa32.h"

const int blueLED = 25;
const int release_RelayNano1 = 2;



void setup() {
  Serial.begin(115200);
  pinMode(blueLED, OUTPUT);
  pinMode(release_RelayNano1, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  OledSSD130_setup();
  LoRa32_setup();

  Serial.print("COUNTDOWN_LIB_VERSION: ");
  Serial.println(COUNTDOWN_LIB_VERSION);
    
}


void loop() {
//  GATEWAY#nodeA#nodeC#         #conveyor=nyala

  callbackMessage(kirimBalik, dataKirimBalik);
  
  while (Serial.available()) {
    String dataInput = Serial.readStringUntil('\n');
//    Serial.println(dataInput);
    LoRa_Message = dataInput;
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesan(dataInput);
  }
}


void callbackMessage(bool statusKirimBalik, String message){
  String GATEWAY_NAME = "GATEWAY";
  String MACHINE_FROM = "nodeE";
  
  if(statusKirimBalik == true){
    kirimBalik = false;
    int ind1      = message.indexOf('#');
    String kata1  = message.substring(0, ind1);
    int ind2      = message.indexOf('#', ind1+1 );
    String kata2  = message.substring(ind1+1, ind2);
    int ind3      = message.indexOf('#', ind2+1 );
    String kata3  = message.substring(ind2+1, ind3);
    int ind4      = message.indexOf('#', ind3+1 );
    String kata4  = message.substring(ind3+1, ind4);

//    Serial.println(message);
    
    if(kata1 == GATEWAY_NAME){
      if(kata3 == MACHINE_NAME){
//        delay(50);
        String TANK_1 = "T1:0";
        String TANK_2 = "T2:0";
        String TANK_3 = "T3:0";
        String TANK_4 = "T4:0";
        String TANK_5 = "T5:0";
        String TANK_6 = "T6:0";
        String TANK_7 = "T7:0";
        String TANK_8 = "T8:0";
        String TANK_9 = "T9:0";
        String TANK_10 = "T10:0";
        String TANK_11 = "T11:0";
        String TANK_12 = "T12:0";
        String TANK_13 = "T13:0";
        String TANK_14 = "T14:0";
        String TANK_15 = "T15:0";
        String TANK_16 = "T16:0";
        String TANK_17 = "T17:0";
        String TANK_18 = "T18:0";
        String TANK_19 = "T19:0";
        String TANK_20 = "T20:0";
        String TANK_21 = "T21:0";
        String TANK_22 = "T22:0";
        String TANK_23 = "T23:0";
        String TANK_24 = "T24:0";
        message = kata1 + "#" + kata3 + "#" + kata2 + "#" + "received_by_machine_to" + "#" + TANK_1 + "#" + TANK_2 + "#" + TANK_3 + "#" + TANK_4 + "#" + TANK_5 + "#" + TANK_6 + "#" + TANK_7 + "#" + TANK_8 + "#" + TANK_9 + "#" + TANK_10 + "#" + TANK_11 + "#" + TANK_12 + "#" + TANK_13 + "#" + TANK_14 + "#" + TANK_15 + "#" + TANK_16 + "#" + TANK_17 + "#" + TANK_18 + "#" + TANK_19 + "#" + TANK_20 + "#" + TANK_21 + "#" + TANK_22 + "#" + TANK_23 + "#" + TANK_24 + "#";
        LoRa_sendMessage(message);
        Serial.println(message);
      }
    }
  }
}
