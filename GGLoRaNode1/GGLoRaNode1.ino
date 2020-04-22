#include <SPI.h>
#include <LoRa.h>
#include "SSD1306Wire.h" // https://github.com/ThingPulse/esp8266-oled-ssd1306/blob/master/src/SSD1306Wire.h
#include "tombol.h"
#include "OledSSD130.h"
#include "LoRa32.h"

const int blueLED = 25;
int detik = 0;

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

  
//  if(tombol(buttonPin) == true){
//    LoRa_Message = "Rx_1";    // id
//    LoRa_kirimPesanData = LoRa_Message;
//    LoRa_kirimPesanStatus = true;
//  }
//  LoRa_kirimPesan(LoRa_kirimPesanData);
  
  while(digitalRead(buttonPin) == LOW){
    detik++;
    /*-------------Display Oled--------------------------------*/
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Setiyo_AGV");
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 16, "Ke        : " + String(detik));
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 26, "Message   : " + LoRa_Message);
    display.drawString(0, 36, "Sent Data : " + LoRa_kirimPesanData);
    display.drawString(0, 46, "Callback    : " + LoRa_callbackMessage());
    display.display();
    display.clear();
    delay(1000);
  }


/*-------------Display Oled--------------------------------*/
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Setiyo_AGV");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 16, "Ke        : " + String(detik));
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 26, "Message   : " + LoRa_Message);
  display.drawString(0, 36, "Sent Data : " + LoRa_kirimPesanData);
  display.drawString(0, 46, "Callback    : " + LoRa_callbackMessage());
  display.display();
  display.clear();

  if(detik == 1){
    detik = 0;
    LoRa_Message = "Rx_1";    // id
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesan(LoRa_kirimPesanData);
  }
  else if(detik == 2){
    detik = 0;
    LoRa_Message = "Rx_2";    // id
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesan(LoRa_kirimPesanData);
  }
  else if(detik == 3){
    detik = 0;
    LoRa_Message = "Rx_3";    // id
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesan(LoRa_kirimPesanData);
  }
  else if(detik == 4){
    detik = 0;
    LoRa_Message = "Rx_4";    // id
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesan(LoRa_kirimPesanData);
  }
  else if(detik == 5){
    detik = 0;
    LoRa_Message = "Rx_5";    // id
    LoRa_kirimPesanData = LoRa_Message;
    LoRa_kirimPesanStatus = true;
    LoRa_kirimPesan(LoRa_kirimPesanData);
  }
  
}

