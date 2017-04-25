//#include <SoftwareSerial.h>
const int DELAY_TIME = 50;
unsigned long sampleNumber=0;
//SoftwareSerial mySerial(8,9); //RX,TX
//RX goes to Dout, TX goes to Din

void setup() {
  Serial1.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  sampleNumber+=1;
  Serial1.print(sampleNumber);
  Serial1.print('B');
  Serial1.println();
  delay(DELAY_TIME);
}
