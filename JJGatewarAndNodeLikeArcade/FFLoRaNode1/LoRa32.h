#define SCK     5    // GPIO5  -- lora SCK
#define RST     23   // GPIO23 atau GPIO12 atau GPIO14 -- RESET (If Lora does not work, replace it with GPIO14)
#define DI0     26   // GPIO26 -- IRQ(Interrupt Request)
#define BAND    915E6
  
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

bool nerimaData= false;
void onReceive(int packetSize) {
  String message = "";
  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  Serial.println(message);
//  LoRa_TerimaBalik = message;
//  dataKirimBalik = message;
//
//  int ind1      = message.indexOf('#');
//  String kata1  = message.substring(0, ind1);
//  int ind2      = message.indexOf('#', ind1+1 );
//  String kata2  = message.substring(ind1+1, ind2);
//
//  if(kata1 == GATEWAY_NAME){
//    if(kata2 == MACHINE_NAME){
//      kirimBalik = true;
//    }
//  }
}

String LoRa_callbackMessage(){ // Terima data dati gateway
  return LoRa_TerimaBalik;
}

void LoRa_kirimPesan(String LoRa_kirimPesanDatanya){
  CD.start(3000);
  LoRa_sendMessage(LoRa_kirimPesanDatanya);

  int count_send_to_gateway = 0;
  while(LoRa_kirimPesanStatus == true) {
    if(CD.remaining() == 0)
    {
      count_send_to_gateway ++;
      Serial.println(LoRa_kirimPesanDatanya);
      LoRa_sendMessage(LoRa_kirimPesanDatanya);
      CD.start(3000);
      LoRa_TerimaBalik = LoRa_kirimPesanDatanya;
    }
    int ind1      = LoRa_callbackMessage().indexOf('#');
    String kata1  = LoRa_callbackMessage().substring(0, ind1);
    int ind2      = LoRa_callbackMessage().indexOf('#', ind1+1 );
    String kata2  = LoRa_callbackMessage().substring(ind1+1, ind2);
    int ind3      = LoRa_callbackMessage().indexOf('#', ind2+1 );
    String kata3  = LoRa_callbackMessage().substring(ind2+1, ind3);
    int ind4      = LoRa_callbackMessage().indexOf('#', ind3+1 );
    String kata4  = LoRa_callbackMessage().substring(ind3+1, ind4);
    
    if(kata1 == GATEWAY_NAME){
      if(kata2 == MACHINE_NAME){
        if(kata3 == MACHINE_TO){
          if(kata4 == "transmitted_to_gateway"){
            if(nerimaData == true){   // DITERIMA GATEWAY
              if(LoRa_callbackMessage() == LoRa_kirimPesanDatanya){
                nerimaData = false;
                LoRa_kirimPesanDatanya = kata1 + "#" +kata2 + "#" + kata3 + "#" + "received_by_gateway" + "#";
                Serial.println(LoRa_kirimPesanDatanya);
                LoRa_sendMessage(LoRa_kirimPesanDatanya);
              }
            }
            else if(count_send_to_gateway  == 10){   // GATEWAY TIDAK DITEMUKAN
              nerimaData = false;
              LoRa_kirimPesanDatanya = kata1 + "#" +kata2 + "#" + kata3 + "#" + "gateway_not_found" + "#";
              Serial.println(LoRa_kirimPesanDatanya);
              LoRa_sendMessage(LoRa_kirimPesanDatanya);
              LoRa_kirimPesanStatus = false;
              LoRa_TerimaBalik = "";
              LoRa_kirimPesanDatanya = "";
            }
          }
          else if(kata4 == "received_by_machine_to"){ // "DITERIMA " + MACHINE_TO
            Serial.println(LoRa_callbackMessage());
            LoRa_kirimPesanDatanya = kata1 + "#" +kata2 + "#" + kata3 + "#" + "waiting_machine_to" + "#";
            LoRa_sendMessage(LoRa_kirimPesanDatanya);
            LoRa_TerimaBalik = "";
          }
          else if(kata4 == "executed_by_machine_to"){ // DIEKSEKUSI oleh MACHINE_TO
            Serial.println(LoRa_callbackMessage());
            LoRa_kirimPesanStatus = false;
            LoRa_TerimaBalik = "";
            LoRa_kirimPesanDatanya = "";
          }
          else if(kata4 == "received_by_gateway"){  // MECHINE_TO NOT FOUND
            if(count_send_to_gateway  == 10){ // failed sent to machine to because of maybe the machine turn off, or out of area
              LoRa_kirimPesanDatanya = kata1 + "#" +kata2 + "#" + kata3 + "#" + "machine_to_not_found" + "#";
              Serial.println(LoRa_kirimPesanDatanya);
              LoRa_kirimPesanStatus = false;
              LoRa_TerimaBalik = "";
              LoRa_kirimPesanDatanya = "";
            }
          }
        }
      }
    }
  }
}

//    display.clear();
//    display.setFont(ArialMT_Plain_16);
//    display.drawString(0, 0, "Setiyo_A G V");
//    display.setFont(ArialMT_Plain_10);
//    display.drawString(0, 26, "Callback    : " + LoRa_callbackMessage());
//    display.display();
