const int buttonPin = 15;    // the pin that the pushbutton is attached to
bool lastTombolState = 1;

bool tombol(const int pin){
  bool buttonState;
  bool buttonPushCounter = false;
  buttonState = digitalRead(pin);
  if (buttonState != lastTombolState) {
    if (buttonState == HIGH){
      buttonPushCounter = true;
      Serial.println("cek");
    }
  }
  lastTombolState = buttonState;

  if (buttonPushCounter == true) {
    buttonPushCounter = false;
    return true;
  }else{
    return false;
  }
  
}
