#include "CountDown.h"
#include "parameters.h"
#include <LoRa.h>
#include "SSD1306Wire.h" // https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/SSD1306Wire.h
#include "tombol.h"
#include "OledSSD130.h"
#include "LoRa32.h"


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
// GATEWAY#AGV_01#TANK#

  callbackMessage(kirimBalik, dataKirimBalik);
  
  while (Serial.available()) {
    String dataInput = Serial.readStringUntil('\n');
    LoRa_Message = dataInput;
    Serial.println(LoRa_Message);
    int ind1      = LoRa_Message.indexOf('#');
    int ind2      = LoRa_Message.indexOf('#', ind1+1 );
    int ind3      = LoRa_Message.indexOf('#', ind2+1 );
    GATEWAY_NAME  = LoRa_Message.substring(0, ind1);
    MACHINE_NAME  = LoRa_Message.substring(ind1+1, ind2);
    MACHINE_TO    = LoRa_Message.substring(ind2+1, ind3);
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesan(LoRa_kirimPesanData);
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesanData = "";
  }
}

//GATEWAY#AGV_01#TANK#executed_by_machine_to#T1:0#T2:0#T3:0#T4:0#T5:0#T6:0#T7:0#T8:0#T9:0#T10:0#T11:0#T12:0#T13:0#T14:0#T15:0#T16:0#T17:0#T18:0#T19:0#T20:0#T21:0#T22:0#T23:0#T24:0#
void callbackMessage(bool statusKirimBalik, String message){
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
      if(kata2 == MACHINE_NAME){
        if(kata3 != MACHINE_NAME){
          if(kata4 == "transmitted_to_gateway"){
            message = kata1 + "#" + kata2 + "#" + kata3 + "#" + "received_by_machine_to" + "#";
            Serial.println(message);
            LoRa_sendMessage(message);
//            delay(1000);
//
//            // Your program below here
//            String TANK_1 = "T1:0";
//            String TANK_2 = "T2:0";
//            String TANK_3 = "T3:0";
//            String TANK_4 = "T4:0";
//            String TANK_5 = "T5:0";
//            String TANK_6 = "T6:0";
//            String TANK_7 = "T7:0";
//            String TANK_8 = "T8:0";
//            String TANK_9 = "T9:0";
//            String TANK_10 = "T10:0";
//            String TANK_11 = "T11:0";
//            String TANK_12 = "T12:0";
//            String TANK_13 = "T13:0";
//            String TANK_14 = "T14:0";
//            String TANK_15 = "T15:0";
//            String TANK_16 = "T16:0";
//            String TANK_17 = "T17:0";
//            String TANK_18 = "T18:0";
//            String TANK_19 = "T19:0";
//            String TANK_20 = "T20:0";
//            String TANK_21 = "T21:0";
//            String TANK_22 = "T22:0";
//            String TANK_23 = "T23:0";
//            String TANK_24 = "T24:0";
//            message = kata1 + "#" + kata2 + "#" + kata3 + "#" + "executed_by_machine_to" + "#" + TANK_1 + "#" + TANK_2 + "#" + TANK_3 + "#" + TANK_4 + "#" + TANK_5 + "#" + TANK_6 + "#" + TANK_7 + "#" + TANK_8 + "#" + TANK_9 + "#" + TANK_10 + "#" + TANK_11 + "#" + TANK_12 + "#" + TANK_13 + "#" + TANK_14 + "#" + TANK_15 + "#" + TANK_16 + "#" + TANK_17 + "#" + TANK_18 + "#" + TANK_19 + "#" + TANK_20 + "#" + TANK_21 + "#" + TANK_22 + "#" + TANK_23 + "#" + TANK_24 + "#";
//            Serial.println(message);
//            LoRa_sendMessage(message);
          }
        }
      }
    }
  }
}
