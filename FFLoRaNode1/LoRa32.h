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
}

String LoRa_callbackMessage(){ // Terima data dati gateway
  return LoRa_TerimaBalik;
}

void LoRa_kirimPesan(String LoRa_kirimPesanDatanya){
  if(LoRa_kirimPesanStatus == true) {
    LoRa_sendMessage(LoRa_kirimPesanDatanya);
    Serial.print("sent: ");
    Serial.println(LoRa_kirimPesanDatanya);
  }
  
  if(LoRa_callbackMessage() == LoRa_kirimPesanDatanya){
    LoRa_kirimPesanStatus = false;
  }
}

