const int DELAY_TIME = 50; //The time to delay before running the loop again. This sets our sampling rate
//The following two lines are taken from the Adafruit code
/* Assign a unique ID to this sensor at the same time */
//Setup is like initialization. It is run once
void setup() {
  Serial.begin(9600);
  while (!Serial) {
  ;
  }
}

//Loop code runs repeatedly. It's like one giant infinite while loop
void loop() {
  //Create an array to store pressure sensor data, and read each sensor into that array
  int pressureSensor[6];
  for (int i = 0; i<6; i++) {
    pressureSensor[i]=analogRead(i);
  }
  
  //Serial.println("Writing to string");
  //Initialize a data string for sending information to the SD card
  String dataString;
  //Add the Pressure Data
  //dataString+= "\n\rP: ";
  for (int i = 0; i<6;i++) {
    dataString+=String(pressureSensor[i]);
    dataString+=",";
    //Make the list comma separated
    /* if (i<5) {
      dataString+=",";
    } else {
      dataString+="\n\r"; //Move to a new line for the accelerometer data
    }
    */
  }
  Serial.println(dataString);
  //resetLoop:
  delay(DELAY_TIME);
}

