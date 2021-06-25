//#include <SPI.h>
#include "CountDown.h"
CountDown CD(CountDown::MILLIS);

#include <LoRa.h>
#include "SSD1306Wire.h" // https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/SSD1306Wire.h
#include "tombol.h"
#include "OledSSD130.h"
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
  while (Serial.available()) {
    String dataInput = Serial.readStringUntil('\n');
//    Serial.println(dataInput);
    LoRa_Message = dataInput;
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesan(dataInput);
  }
  
  if(tombol(buttonPin) == true){
    String kata1  = GATEWAY_NAME;
    String kata2  = MACHINE_FROM;
    String kata3  = MACHINE_TO;
    String kata4  = "transmitted_to_gateway";
    LoRa_Message = kata1 + "#" +kata2 + "#" + kata3 + "#" + kata4 + "#";
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesan(LoRa_kirimPesanData);
//    Serial.println(LoRa_kirimPesanData);
    delay(50);
  }
}
