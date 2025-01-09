const int TRIGPININ = 10, ECHOPININ =11, BUTTONPININ = 9, RSI = 53, ENI = 51, D4I = 43, D5I = 45, D6I = 47, D7I = 49, SERVOPININ = 8; // Pin setting for entering lane
const int TRIGPINOUT = 6, ECHOPINOUT = 7, BUTTONPINOUT = 5, RSO = 36, ENO = 34, D4O = 26, D5O = 28, D6O = 30, D7O = 32, SERVOPINOUT = 12; // Pin setting for exiting lane

#include <Servo.h>
#include <LiquidCrystal.h>
//#include <MFRC522.h>

// Motor
Servo servoIn; // Entering lane motor
Servo servoOut; // Exiting lane motor
const int servoInPin = SERVOPININ; // Assigns entry motor pin
const int servoOutPin = SERVOPINOUT; // Assigns exiting motor pin

// RFID Sensor variables
/*
#define SSIN_PIN 
#define RSTIN_PIN
#define SSOUT_PIN 
#define RSTOUT_PIN
byte readCard [4];
string TagIDin;
string TagIDout;
accessGranted[3] = {"","",""};
int accessGrantedSize = 3;
bool granted;
*/
//LabView Control
String ControlMode;
String command;
String Entering;
String Exiting;
int firstIndex = 0;
int secondIndex = 0;
// Ultrasonic Sensor
const int PROXIMITY = 7; // Distance reading of no vehicle
const float cmRatio = 0.01723; // Conversion ration to cm
String USInStr;
String USOutStr;
int USIn;
int distanceIN; // Distance measurement of the entrance lane
int USOut;
int distanceOUT; //. Distance measurement of the exiting lane
// Pin Setting of the US Sensor
const int trigPinIn = TRIGPININ;
const int echoPinIn = ECHOPININ;
const int trigPinOut = TRIGPINOUT;
const int echoPinOut = ECHOPINOUT;
// Push Button
const int buttonIn = BUTTONPININ;
const int buttonOut = BUTTONPINOUT;
int buttonInState;
int buttonOutState;
// LCD Screen
LiquidCrystal lcdIN(RSI, ENI, D4I, D5I, D6I, D7I);
LiquidCrystal lcdOUT(RSO, ENO, D4O, D5O, D6O, D7O);
//MFRC522 RFIDin (SSIN_PIN,RSTIN_PIN);
//MFRC522 RFIDout (SSOUT_PIN,RSTOUT_PIN);
// Capacity
const int MAXCAPACITY = 30;
float currentCapacity;
float percentCap;

void setup(){ // Program setup
Serial.begin(9600);
//entering lane sensors and motor
pinMode(trigPinIn, OUTPUT);
pinMode(echoPinIn, INPUT);
servoIn.attach(servoInPin);
pinMode(buttonIn, INPUT);
lcdIN.begin(16, 2);
//exiting lane sensors and motor
pinMode(trigPinOut, OUTPUT);
pinMode(echoPinOut, INPUT);
servoOut.attach(servoOutPin);
pinMode(buttonOut, INPUT);
lcdOUT.begin(16, 2);
// RFIDin.PCD_init();
// delay(4);
// RFIDin.PCD_DumpVersiontoSerial();
// RFIDout.PCD_init();
// delay(4);
// RFIDout.PCD_DumpVersiontoSerial();
}

bool carPresenceCheck(int distance){ // Detection of a vehicle in lane
return distance <= PROXIMITY;
}

void setPercentCapacity(){ // Sets the percent capacity of the lot.
percentCap = (currentCapacity / MAXCAPACITY)*100;
}

// Payment Validation
bool checkRFID(String tagID){ // Checking if the RFID scan is met + Valid in member IDs.
/*
for (int i = 0; i<=(acessGrantedSize-1);i++) 
  if(accessGranted[i] == tagID){
    return true;
  }
else{
  return false
}
*/
}

bool checkManualPayment(String side){ // Checking if manual payment (Button Press) is made.
  if (side == "front"){
    buttonInState = digitalRead(buttonIn);
    return buttonInState == HIGH; // Checks if the button is pressed
  }
  else{
    buttonOutState = digitalRead(buttonOut);
    return buttonOutState == HIGH; // Checks if the button is pressed
  }
}

void mOpen(String side){ // Open the gate from the closed position
  if (side == "front"){
    for (int pos = 90; pos <= 180; pos += 1) { // Opens the gate incrementally
      servoIn.write(pos); 
      delay(15); 
    }
  }
  else{
    for (int pos = 90; pos <= 180; pos += 1) { // Opens the gate incrementally
     servoOut.write(pos); 
     delay(15); 
    }
  }
}

void mClose(String side){ // Close the gate from the opened position
if (side == "front"){
  for (int pos = 180; pos >= 90; pos -= 1) { // Closes the entrace gate incrementally
    servoIn.write(pos); 
    delay(15); 
  }
}
  else{
    for (int pos = 180; pos >= 90; pos -= 1) { // Closes the rear gate incrementally
      servoOut.write(pos); 
      delay(15); 
    }
  }
}


long getUSReading(String side){ // Returns the current proximity read by the ultrasonic sensor
    if (side == "front"){
      digitalWrite(trigPinIn, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPinIn, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPinIn, LOW);
      return pulseIn(echoPinIn, HIGH); // Returns the distance read at the entrace lane
    }
    else{ 
      digitalWrite(trigPinOut, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPinOut, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPinOut, LOW);
      return pulseIn(echoPinOut, HIGH); //. Returns the distance read at the exiting lane
    }

 }

void loop(){ // Main Function
  setPercentCapacity();
  distanceIN = cmRatio * getUSReading("front"); // Retrieve the entering lane US reading
  distanceOUT = cmRatio * getUSReading("back"); // Retrieve the exiting lane US reading
 //RFID Scanning
 /*
if(RFIDin.findCard(PICC_REQIDL, str) == MI_OK){
  TagIDin = "";
  if (RFIDin.anticoll(str) == MI_OK){
    for(int i = 0; i <4; i++){
      TagIDin = TagIDin + (0x0F & (str[i]>>4));
      TagIDin = TagIDin + (0x0F & str[i]);
    }
  }
}
if(RFIDout.findCard(PICC_REQIDL, str) == MI_OK){
  TagIDout = "";
  if (RFIDout.anticoll(str) == MI_OK){
    for(int i = 0; i <4; i++){
      TagIDout = TagIDout + (0x0F & (str[i]>>4));
      TagIDout = TagIDout + (0x0F & str[i]);
    }
  }
}
 */
  //Labview Control
  if (Serial.available()>0){
    command = Serial.readStringUntil('\n');
    firstIndex = command.indexOf(',');
    secondIndex = command.indexOf(',',firstIndex + 1);
    
    ControlMode = command.substring(0,firstIndex);
    Entering = command.substring(firstIndex+1,secondIndex);
    Exiting = command.substring(secondIndex+1);
    
    if(ControlMode == "1"){
      if(Entering == "1"){
          servoIn.write(180);
          currentCapacity++;
    
      } 
        if(Entering == "-1"){
          servoIn.write(90); 
         
      }
     
      if(Exiting == "1"){
            servoOut.write(180);
            currentCapacity--;
       
      }
       if(Exiting == "-1"){
            servoOut.write(90); 
            
        }
    }
  }

    // Displays the message on the entrance LCD
    lcdIN.setCursor(0, 0);
    lcdIN.print("Welcome to Lot M!");
    lcdIN.setCursor(0, 1);
    lcdIN.print(String(percentCap)+"% " + String(currentCapacity) +"/" + String(MAXCAPACITY));
    // Displays the message on the exit LCD
    lcdOUT.setCursor(0, 0);
    lcdOUT.print("Thank You!");
    lcdOUT.setCursor(0, 1);
    lcdOUT.print(String(percentCap)+"% " + String(currentCapacity) +"/" + String(MAXCAPACITY));

    // Detect if vehicle is in lane -> Check if payment is made -> Open Gate
    if (carPresenceCheck(distanceIN) == true && (checkManualPayment("front") == true /*|| checkRFID(TagIDin) == true*/)){ 
        mOpen("front");
        while(carPresenceCheck(distanceIN)){ // Loop unil the car is no longer present
          distanceIN = cmRatio * getUSReading("front");
        }
        delay(2000); // Wait 2 seconds after the vehicle has left
        mClose("front"); // Closes the gate
        currentCapacity++; // Increments the capacity
        } 
    else if (carPresenceCheck(distanceOUT) == true && (checkManualPayment("back") == true /*|| checkRFID(TagIDout) == true*/)){ 
       mOpen("back");
        while(carPresenceCheck(distanceOUT)){ // Loop unil the car is no longer present
          distanceOUT = cmRatio * getUSReading("back");
        }
        delay(2000); // Wait 2 seconds after the vehicle has left
        mClose("back"); // Closes the gate
        currentCapacity--; // Decrements the capacity
        }
    }
