String GATEWAY_NAME   = "GATEWAY";
String MACHINE_NAME   = "TANK";
String MACHINE_TO;

bool kirimBalik;
String dataKirimBalik = "";

const int blueLED = 25;
const int release_RelayNano1 = 2;

CountDown CD(CountDown::MILLIS);
