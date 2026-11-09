//Ultrasonic Sensor
const int trig = 4;
const int echo = 4;

//Motor 1 and Enable pin
const int In1 = A5;
const int En1 = 5;

//Motor 2 and Enable pin
const int In4 = 2;
const int En2 = 3;

// Colour Sensor
#define S0 A3
#define S1 A4
#define S2 A1
#define S3 A0
#define sensorOut A2
#define OE 0
int frequency = 0;

int redMin = 10,   redMax = 200;
int greenMin = 10, greenMax = 180;
int blueMin = 10,  blueMax = 200;


int redValue, greenValue, blueValue;


//Line Sensor Array
#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t SensorCount = 4;
uint16_t sensorValues[SensorCount];

#include <SD.h>

File myFile;

const int CSPin = 10;

void setup() {

  //Serial Monitor
  Serial.begin(9600);
  
  // ULTRASONIC Sensor
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  // Motor 1 and Enable pin
  pinMode(In1, OUTPUT);
  pinMode(En1, OUTPUT);
  
  // Motor 2 and Enable pin
  pinMode(In4, OUTPUT);
  pinMode(En2, OUTPUT);
  
  // Colour Sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  digitalWrite(OE, LOW);  //This may or may not be needed...
  
  //Line Sensor Array
  //Configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){9,8,7,6}, SensorCount);
  
  for (int i = 0; i < 500; i++){
    qtr.calibrate();
  }
  

 //SD Card
 Serial.print("Initializing SD card...");
 pinMode(CSPin, OUTPUT);

 if (!SD.begin(CSPin)) {
   
   Serial.println("initialization failed!");
   return;
 }

 Serial.println("initialization done.");

 myFile = SD.open("test.txt", FILE_WRITE);


}

void loop() {


  // Motors
  

  
  // Colour Sensor
  //Setting red filtered pohotodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  //Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Flipping from pulse width duration to frequency
  redValue = map(frequency, redMin, redMax, 255, 0);
  redValue = constrain(redValue, 0, 255);
  // Printing the value on the serial monitor
 Serial.print("R= "); // Printing name
 Serial.print(redValue); // printing Red colour frequency
 Serial.print("  ");

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  //Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Flipping from pulse width duration to frequency
  greenValue = map(frequency, greenMin, greenMax, 255, 0);
  greenValue = constrain(greenValue, 0, 255);
  // Printing the value on the serial monitor
 Serial.print("G= "); // Printing name
 Serial.print(greenValue); // printing Green colour frequency
 Serial.print("  ");

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  //Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Flipping from pulse width duration to frequency
  blueValue = map(frequency, blueMin, blueMax, 255, 0);
  blueValue = constrain(blueValue, 0, 255);
  Printing the value on the serial monitor
 Serial.print("B= "); // Printing name
 Serial.print(blueValue); // printing Blue colour frequency
 Serial.println("");
 
 if(redValue > 180 & greenValue > 165 & blueValue > 180){
   Serial.print("White ");
   goStraight();
 }else if (redValue > 100 & greenValue > 160 & blueValue > 100){
   Serial.print("Yellow ");
   goLeft();
 }else if (redValue > 50 & 20 > greenValue & 100 > blueValue){
   Serial.print("Purple ");
   goRight();
 }

goStraight();
  
  // Line Sensor Array
  //read raw sensor values
  int Line = qtr.readLineBlack(sensorValues);
 

//  //print the sensor values as numbers from 0 to 1023, where 0 means maximum
//  // reflectance and 1023 means minimum reflectance
//  for (uint8_t i = 0; i < SensorCount; i++) {
//    Serial.print(sensorValues[i]);  
//    Serial.print('\t');
//  }
  Serial.print(Line);
  Serial.println();
  
 // Ultrasonic Sensor
 long duration, cm;

 digitalWrite(trig, LOW);
 delayMicroseconds(2);
 digitalWrite(trig, HIGH);
 delayMicroseconds(5);
 digitalWrite(trig, LOW);

 duration = pulseIn(echo, HIGH);
 cm = microsecondsToCentimeters(duration);

 Serial.print(cm);
 Serial.print("cm");
 Serial.println();
 delay(100);

  //SD Card Reader
}

float microsecondsToCentimeters(long microseconds){
  return float(microseconds) / 29 / 2;
}

void goStraight() {
  
  //Turn on Motor 1
  digitalWrite(In1, HIGH);
  // Set speed between 150 and 255
  analogWrite(En1, 75);

  //Turn on Motor 2
  digitalWrite(In4, HIGH);
  // Set speed between 150 and 255
  analogWrite(En2, 63);
}

void goLeft(){
  //Turn on Motor 1
  digitalWrite(In1, HIGH);
  //Set speed
  analogWrite(En1, 75);

  //Turn off Motor 2
  digitalWrite(In4, LOW);
}

void goRight(){
  //Turn off Motor 1
  digitalWrite(In1, LOW);

  //Turn on Motor 2
  digitalWrite(In4, HIGH);
  //Set speed
  analogWrite(En2, 75);
}

void Stop(){
  //Turn off Motor 1
  digitalWrite(In1, LOW);

  //Turn off Motor 2
  digitalWrite(In4, LOW);
}
