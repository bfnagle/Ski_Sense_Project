/*  Code initially written by Brendan Nagle
  Worked with Teresa Ou and John Stanley
  Project completed for Thayer School of Engineering Engs 89/90
  Sponsored by Gunnar Pope, Evan Gelfand, Allison Beck, and Ziad Al-shamsie
*/
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(8, 9); //RX,TX
//RX goes to Dout, TX goes to Din
unsigned long sampleNumber = 1;
char inChar=-1;
char inData[2];
byte index=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial1.begin(115200);
}

void loop() {
  //Serial.print("next Line");
  //Serial.println(String(sampleNumber));
  //sampleNumber += 1;
  while(Serial1.available()>0) {
    char c = Serial1.read();
    Serial.print(c);
    //delay(10);
    //Serial.print(Serial1.read());
    //delay(1);
  }
  /* while (Serial1.available()>0) {
      inChar=Serial1.read();
      inData[index]=inChar;
      index++;
    }
  inData[index]='\0';
  index=0;
  while (inData[index]!='\0') {
    Serial.print(inData[index]);
    index++;
  }
  Serial.println();
  */
}

