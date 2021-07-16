
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

// GATEWAY#AGV_01#TANK#transmitted_to_gateway#
void loop() {
  while (Serial.available()) {
    String dataInput = Serial.readStringUntil('\n');
//    Serial.println(dataInput);
//    LoRa_sendMessage(dataInput);
    
    LoRa_Message = dataInput;
//    int ind1      = LoRa_Message.indexOf('#');
//    int ind2      = LoRa_Message.indexOf('#', ind1+1 );
//    int ind3      = LoRa_Message.indexOf('#', ind2+1 );
//    GATEWAY_NAME  = LoRa_Message.substring(0, ind1);
//    MACHINE_NAME  = LoRa_Message.substring(ind1+1, ind2);
//    MACHINE_TO    = LoRa_Message.substring(ind2+1, ind3);
//    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
//    LoRa_kirimPesan(LoRa_kirimPesanData);
//    LoRa_kirimPesanData = "";
  }
  if(LoRa_kirimPesanStatus == true){
    Serial.println(LoRa_Message);
    LoRa_sendMessage(LoRa_Message);
    LoRa_Message = "";
    LoRa_kirimPesanStatus = false;
  }
}
