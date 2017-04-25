/* Datalogger program for the Ski Sense project
  Code initially written by Brendan Nagle
  Worked with Teresa Ou and John Stanley
  Project completed for Thayer School of Engineering Engs 89/90
  Sponsored by Gunnar Pope, Evan Gelfand, and their team.

  Circuit involves:
  Analog pressure sensors on analog ping 0-5
  Accelerometer on digital pins for SCL and SDA
  SD card attached to SPI bus as follows:
  MOSI - pin MOSI
  MISO - pin MISO
  CLK - pin CSCK
  CS - pin 10
*/
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_LSM303_U.h>
#include <SPI.h>
//#include <XBee.h>
#include <Wire.h>

const int CIRCUIT_ON = 4;
unsigned long sampleNumber = 1;
const int greenLED = 11;
const int redLED = 12;
boolean interrupted = false;

  /*
  * Test sampling rate
  */
  //const int fPin = 7;
  
//The following two lines are taken from the Adafruit code
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);

//Setup is like initialization. It is run once
void setup() {
  //Serial.begin(9600);
  //Set up serial communications for the Xbee
  //The baud rate is 57600, from following directions here:
  //http://www.instructables.com/id/Arduino-Wireless-Programming-with-XBee-Series-1-or/
  Serial1.begin(115200);
  //Accelerometer code is either modified or directly taken from the Adafruit sensor test program
  if(!accel.begin()) {
    while(1);
  }
  //If changing the gain, also need to change _lsm303Accel_MG_LSB to 0.001F,0.002F,0.004F, or 0.012F
  //in Adafruit_LSM303_U.cpp
  // accel.setGain(LSM303_ACCELGAIN_16);
  accel.setGain(LSM303_ACCELGAIN_08);
  // accel.setGain(LSM303_ACCELGAIN_04);
  // accel.setGain(LSM303_ACCELGAIN_02);
  
  //if(!gyro.begin(GYRO_RANGE_250DPS)) {
  if(!gyro.begin(GYRO_RANGE_500DPS)) {
    //if(!gyro.begin(GYRO_RANGE_2000DPS)) {
    /* There was a problem detecting the L3GD20 ... check your connections */
    while(1);
  }
  
  /*Set the pins for the mode indicator LEDS*/
  pinMode(greenLED,OUTPUT);
  pinMode(redLED,OUTPUT);
  
  /*
  * Test sampling rate
  */
  //pinMode(fPin,OUTPUT);
  /*Enable Timer Interupts and Configure. Code taken from
  http://www.instructables.com/id/Arduino-Timer-Interrupts/step2/Structuring-Timer-Interrupts/ */
  cli();//stop interrupts
  //set timer1 interrupt at 20Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1 = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 24999;// = (16*10^6) / (7*64) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 and CS10 bits for 64 prescaler
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}

/*Timer Interrupt Handler*/
ISR(TIMER1_COMPA_vect){
  interrupted=true;
  //goto resetLoop;
}

//Loop code runs repeatedly. It's like one giant infinite while loop
void loop() {
  beginning:
  cli();//stop interrupts
  TCNT1 = 0;//initialize counter value to 0
  sei();//allow interrupts
  interrupted=false;
  if (digitalRead(CIRCUIT_ON)==HIGH) {
    digitalWrite(greenLED,HIGH);
    digitalWrite(redLED,LOW);
    
    //Wait for the signal from the master to start data collection
    boolean flag = false;
    char c = 'N';
    while (!flag) {
      if (!interrupted) {
        c=Serial1.read();
        if (c=='G') {
          flag=true;
        }
      } else {
      goto beginning;
      }
    }
    Serial1.print('K');
    
    /*
    * Test sampling rate
    */
    //digitalWrite(fPin,HIGH);
    
    //Create an array to store pressure sensor data, and read each sensor into that array
    int pressureSensor[6];
    for (int i = 0; i<6; i++) {
      pressureSensor[i]=analogRead(i);
    }
    
    /*
    * Test sampling rate
    */
    //digitalWrite(fPin,LOW);
    
    /* Get a new sensor event */
    /* Accelerometer methods can be found here: https://learn.adafruit.com/adxl345-digital-accelerometer/library-reference */
    sensors_event_t event;
    accel.getEvent(&event);
    int accelData[3];
    accelData[0] = event.acceleration.x; //The x acceleration data is stored in position 0
    accelData[1] = event.acceleration.y; //The y acceleration data is stored in position 1
    accelData[2] = event.acceleration.z; //The z acceleration data is stored in position 2
    
    gyro.getEvent(&event);
    int gyroData[3];
    gyroData[0] = event.gyro.x;
    gyroData[1] = event.gyro.y;
    gyroData[2] = event.gyro.z;
    
    //Initialize a data string for sending information to the SD card
    String dataString=String(sampleNumber);
    
    //Add the Pressure Data
    dataString+=",";
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
    
    //Add the Accelerometer Data
    //dataString+="A: ";
    for (int i = 0; i<3;i++) {
      dataString+=String(accelData[i]);
      dataString+=",";
      /* if (i<2) {
        dataString+=",";
      } else {
        dataString+="\n\r";
      }
      */
    }
    
    //dataString+="G: ";
    for (int i = 0; i<3;i++) {
      dataString+=String(gyroData[i]);
      if (i<2) {
        dataString+=",";
      } else {
        dataString+="\n\r";
      }
    }
    
    //Send the data to the master
    Serial1.print(dataString);
    //Increment the sample number
    sampleNumber+=1;
  }
  else {
    digitalWrite(redLED,HIGH);
    digitalWrite(greenLED,LOW);
    sampleNumber=1;
  }
}

