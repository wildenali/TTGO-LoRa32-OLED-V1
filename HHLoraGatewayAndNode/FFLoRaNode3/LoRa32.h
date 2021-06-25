#define SCK     5    // GPIO5  -- lora SCK
#define RST     23   // GPIO23 atau GPIO12 atau GPIO14 -- RESET (If Lora does not work, replace it with GPIO14)
#define DI0     26   // GPIO26 -- IRQ(Interrupt Request)
#define BAND    915E6

String GATEWAY_NAME   = "GATEWAY";
String MACHINE_NAME   = "nodeE";
String MACHINE_TO     = "nodeB";
  
String LoRa_Message;
String LoRa_TerimaBalik;
String LoRa_kirimPesanData;  //DataYangDiKirim
bool   LoRa_kirimPesanStatus = false;

void LoRa_rxMode();
void LoRa_txMode();
void LoRa_sendMessage(String message);
void onReceive(int packetSize);
String LoRa_callbackMessage();
void LoRa_kirimPesan(String LoRa_kirimPesanDatanya);

void LoRa32_setup(){
  while (!Serial);
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

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}
 
void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}
void LoRa_sendMessage(String message) {
//  Serial.println("Kirim : " + message);
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket();                     // finish packet and send it
  LoRa_rxMode();                        // set rx mode
  delay(50);                            // minimal delay(50), untuk transisi menjadi rx mode
}
 
void onReceive(int packetSize) {
  String message = "";
  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  LoRa_TerimaBalik = message;
  dataKirimBalik = message;
//  kirimBalik = true;
//  Serial.println("Terima: " + LoRa_TerimaBalik);
    int ind1      = message.indexOf('#');
    String kata1  = message.substring(0, ind1);
    int ind2      = message.indexOf('#', ind1+1 );
    String kata2  = message.substring(ind1+1, ind2);

    if(kata1 == GATEWAY_NAME){
      if(kata2 != MACHINE_NAME){
        kirimBalik = true;
      }
    }
}

String LoRa_callbackMessage(){ // Terima data dati gateway
  return LoRa_TerimaBalik;
}

void LoRa_kirimPesan(String LoRa_kirimPesanDatanya){
  CD.start(3000);
  LoRa_sendMessage(LoRa_kirimPesanDatanya);

  int count_send_to_broker = 0;
  while(LoRa_kirimPesanStatus == true) {
    if(CD.remaining() == 0)
    {
      count_send_to_broker++;
      LoRa_sendMessage(LoRa_kirimPesanDatanya);
      CD.start(3000);
    }
    int ind1      = LoRa_callbackMessage().indexOf('#');
    String kata1  = LoRa_callbackMessage().substring(0, ind1);
    int ind2      = LoRa_callbackMessage().indexOf('#', ind1+1 );
    String kata2  = LoRa_callbackMessage().substring(ind1+1, ind2);
    int ind3      = LoRa_callbackMessage().indexOf('#', ind2+1 );
    String kata3  = LoRa_callbackMessage().substring(ind2+1, ind3);
    int ind4      = LoRa_callbackMessage().indexOf('#', ind3+1 );
    String kata4  = LoRa_callbackMessage().substring(ind3+1, ind4);
    int ind5      = LoRa_callbackMessage().indexOf('#', ind4+1 );
    String kata5  = LoRa_callbackMessage().substring(ind4+1, ind5);
    LoRa_kirimPesanDatanya = kata1 + "#" +kata2 + "#" + kata3 + "#" + kata4 + "#" + kata5 + "#";

    if(kata1 == GATEWAY_NAME){
      if(kata2 == MACHINE_NAME){
        if(kata3 == MACHINE_TO){
          Serial.println(kata1);
          Serial.println(kata2);
          Serial.println(kata3);
          Serial.println(kata4);
          Serial.println(kata5);
          LoRa_kirimPesanStatus = false;
          LoRa_TerimaBalik = "";
          LoRa_kirimPesanDatanya = "";
        }
      }

//      if(kata2 == MACHINE_NAME){
//        if(kata4 == "accepted"){
////          message = kata1 + "#" +kata2 + "#" + kata3 + "#" + "accepted" + "#" + kata5 + "#";
////          LoRa_sendMessage(message);
//          Serial.println(LoRa_kirimPesanDatanya);
//          LoRa_kirimPesanStatus = false;
//          LoRa_TerimaBalik = "";
//          LoRa_kirimPesanDatanya = "";
//        }
//      }
    }
    
//    // Succeeded - Ketika pesan yg di kirim node dan di terima gateway dan di balikin lagi
//    if(LoRa_callbackMessage() == LoRa_kirimPesanDatanya){        
//      LoRa_kirimPesanDatanya = kata1 + "#" +kata2 + "#" + kata3 + "#" + "trasmitted" + "#" + kata5 + "#";
//      Serial.println(LoRa_kirimPesanDatanya);
//      
//      LoRa_kirimPesanStatus = false;
//      LoRa_TerimaBalik = "";
//      LoRa_kirimPesanDatanya = "";
//    }
//    // notfound - tidak menemukan gateway, atau gateway mati
//    if(count_send_to_broker == 10){
//      LoRa_kirimPesanDatanya = kata1 + "#" +kata2 + "#" + kata3 + "#" + "notfound" + "#" + kata5 + "#";
//      Serial.println(LoRa_kirimPesanDatanya);
//      
//      LoRa_kirimPesanStatus = false;
//      LoRa_TerimaBalik = "";
//      LoRa_kirimPesanDatanya = "";
//    }
//    // accepted - diterima oleh target
//    if(kata3 == "accepted"){
//      Serial.println(LoRa_kirimPesanDatanya);
//      
//      LoRa_kirimPesanStatus = false;
//      LoRa_TerimaBalik = "";
//      LoRa_kirimPesanDatanya = "";
//    }
  }
}

//    display.clear();
//    display.setFont(ArialMT_Plain_16);
//    display.drawString(0, 0, "Setiyo_A G V");
//    display.setFont(ArialMT_Plain_10);
//    display.drawString(0, 26, "Callback    : " + LoRa_callbackMessage());
//    display.display();
