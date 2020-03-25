//#include <SPI.h>
#include <LoRa.h>
#include "SSD1306Wire.h" // https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/SSD1306Wire.h
#include "tombol.h"
#include "OledSSD130.h"
#include "LoRa32.h"

const int blueLED = 25;

void setup() {
  Serial.begin(115200);
  pinMode(blueLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  OledSSD130_setup();
  LoRa32_setup();
}


void loop() {
//  LoRa_Message = "001,";    // id
//  LoRa_Message += millis()/100;   // kirim data millis,
//  LoRa_Message += "#";             // tanda akhir data

  
  if(tombol(buttonPin) == true){
    LoRa_Message = "001,";    // id
    LoRa_Message += "agv1siap";   // kirim data millis,
    LoRa_Message += "#";             // tanda akhir data
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
  }
  LoRa_kirimPesan(LoRa_kirimPesanData);

  if(LoRa_callbackMessage() == "002,agv1jalan#"){
    digitalWrite(blueLED, HIGH);
  }
  else{
    digitalWrite(blueLED, LOW); 
  }
/*-------------Display Oled--------------------------------*/
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Setiyo_AGV");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 26, "Message   : " + LoRa_Message);
  display.drawString(0, 36, "Sent Data : " + LoRa_kirimPesanData);
  display.drawString(0, 46, "Callback    : " + LoRa_callbackMessage());
  display.display();
  display.clear();
  
}


