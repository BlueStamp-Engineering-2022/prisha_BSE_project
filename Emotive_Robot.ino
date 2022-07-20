// DEFINE STUFF
#define echoPin 8 
#define trigPin 9 

//INCLUDE LIBRARIES
#include <SoftwareServo.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

//DECLARE STUFF
LiquidCrystal_I2C lcd(0x27,16,2); 
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();
SoftwareServo browoneservo;  
SoftwareServo browtwoservo;
SoftwareServo panservo;
SoftwareServo tiltservo;

//BLUETOOTH STUFF
const unsigned long BAUD_RATE = 9600;
const int BLUETOOTH_TX = 1;
const int BLUETOOTH_RX = 0;

// SET VARIABLES
int brow1pos = 0; 
int brow2pos = 0;
int tiltpos  = 0;
int panpos = 0;  
int antipos =0;
long duration; 
int distance; 
int var = 0;
int button;

String bluetoothVal = "";

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(Serial);

bool wasSad = false;
bool wasAngry = false;
bool wasNuetral = true;

// MAKE BIT MAPS FOR EYES
static const uint8_t PROGMEM
  neutral_eyes_bmp[] =
  { B00000000,
    B01111110,
    B01000010,
    B01011010,
    B01011010,
    B01000010,
    B01111110,
    B00000000 },
   angry_eye1_bmp[] = 
   { B00100000,
     B00110000,
     B01001000,
     B01011100,
     B01011010,
     B01000010,
     B00111100,
     B00000000 },
    angry_eye2_bmp[] = 
    { B00000100,
      B00001100,
      B00010010,
      B00111010,
      B01011010,
      B01000010,
      B00111100,
      B00000000 },
     sad_eye1_bmp[] =
     {B00000000,
      B00001100,
      B00010010,
      B00111010,
      B01011010,
      B01000010,
      B00111100,
      B00000000 },
      sad_eye2_bmp[] =
      {B00000000,
       B00110000,
       B01001000,
       B01011100,
       B01011010,
       B01000010,
       B00111100,
       B00000000 },
      laugh_eye1_bmp [] =
      {B01110000,
       B00111000,
       B00011100,
       B00001110,
       B00011100,
       B00111000,
       B01110000,
       B11100000 },
      laugh_eye2_bmp[] = 
      {B00001110,
       B00011100,
       B00111000,
       B01110000,
       B00111000,
       B00011100,
       B00001110,
       B00000111},
      love_eyes_bmp[] =
      {B01100110,
       B10011001,
       B10000001,
       B10000001,
       B10000001,
       B01000010,
       B00100100,
       B00011000 },
      dead_eyes_bmp[] =
       {B11000001,
        B01100011,
        B00110110,
        B00011100,
        B00111100,
        B01100110,
        B11000011,
        B10000001 },
       sleep_eyes_bmp[] =
       {B00000000,
        B00000000,
        B00000000,
        B10000001,
        B01000010,
        B00111100,
        B00000000,
        B00000000 };

void setup() {
  //SET UP
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor HC-SR04 Test");
  Serial.println("with Arduino UNO R3");
  Serial.begin(BAUD_RATE);
    
  //bluetooth.begin(BAUD_RATE);

  matrix.begin(0x70);  
  browoneservo.attach(A2); 
  browtwoservo.attach(A3); 
  tiltservo.attach(A1);
  panservo.attach (A0);
  lcd.init(); 
  delay(100);

}



void loop() {
  // ULTRASONIC SENSOR STUFF
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0344 / 2; 
  Serial.println(distance);

  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0,1);

  button = phone.getButton();

    if (button != -1) {
        bluetoothVal = button;
    }
  
  // SET VARIABLES FOR SWITCH FUNCTION
  if (1 <= distance && distance < 10){
    var = 1;
    Serial.println("SAD SAD SAD SAD SAD");
  }
  else if (10 <= distance && distance < 20){
     var = 2;
     Serial.println("HAPPY HAPPY HAPPY HAPPY");
  }

  // START SWITCH 
  switch (var) {
      
    case 1:
    //SAD CASE 
    Serial.println("SAD");
      //EYES 1
      matrix.drawBitmap(0, 0, sad_eye1_bmp, 8, 8, LED_ON);
      matrix.drawBitmap(0, 8, sad_eye2_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();

      //BROWS 
      servos(50, 10, 120, 60);
      //pan, tilt, left, right
      
      //LCD 1
      lcd.clear();
      lcd.print("Sad );");
      delay(500);
      
      matrix.clear();
      break;
      
    case 2:
      // ANGRY CASE
      Serial.println("ANGRY");
      ////EYES  2
      matrix.drawBitmap(0, 0, angry_eye1_bmp, 8, 8, LED_ON);
      matrix.drawBitmap(0, 8, angry_eye2_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();

      //BROWS & HEAD START 2`
      servos(95, 90, 60, 120);
      //pan, tilt, left, right

      //LCD 1
      
      lcd.clear();
      lcd.print("Angry >:(");
      delay(500);
      
      matrix.clear();
      break;
        
    default:
      
      if (bluetoothVal == "0"){
        /// Case laughing
        
      Serial.println("LAUGH");
      //EYES 3
      matrix.drawBitmap(0, 0, laugh_eye1_bmp, 8, 8, LED_ON);
      matrix.drawBitmap(0, 8, laugh_eye2_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();

      //BROWS & HEAD START 3
      servos(95, 90, 60, 120);
      //pan, tilt, left, right
      
      //LCD 1
      lcd.print("lol");
      delay(500);
      lcd.clear();

      matrix.clear();

      break;
        
    }else if (bluetoothVal == "1"){
      ///CASE LOVE
       Serial.println("LOVE");
      //EYES 3
      matrix.drawBitmap(0, 0, love_eyes_bmp, 8, 8, LED_ON);
      matrix.drawBitmap(0, 8, love_eyes_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();

      //BROWS & HEAD START 3
      servos(95, 60, 120, 60);
      //pan, tilt, left, right
      
      //LCD 1
      lcd.print("<3 ");
      delay(500);
      lcd.clear();

      matrix.clear();

      break;
        
    }else if (bluetoothVal == "2"){
      /// CASE DEAD
      Serial.println("dEAD");
      //EYES 3
      matrix.drawBitmap(0, 0, dead_eyes_bmp, 8, 8, LED_ON);
      matrix.drawBitmap(0, 8, dead_eyes_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();

      //BROWS & HEAD START 3
      servos(95, 90, 90, 90);
      //pan, tilt, left, right
      
      //LCD 1
      lcd.print("x_x");
      delay(500);
      lcd.clear();

      matrix.clear();

      break;
        
    }else if (bluetoothVal == "3"){
      ///CASE SLEEP
       Serial.println("SLEEP");
      //EYES 3
      matrix.drawBitmap(0, 0, sleep_eyes_bmp, 8, 8, LED_ON);
      matrix.drawBitmap(0, 8, sleep_eyes_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();

      //BROWS & HEAD START 3
      servos(50, 10, 90, 90);
      //pan, tilt, left, right
      
      //LCD 1
      lcd.print("Zzz");
      delay(500);
      lcd.clear();

      matrix.clear();

      break;
        
    }

    // NEUTRAL CASE
    
      Serial.println("NUETRAL");
      //EYES 3
      matrix.drawBitmap(0, 0, neutral_eyes_bmp, 8, 8, LED_ON);
      matrix.drawBitmap(0, 8, neutral_eyes_bmp, 8, 8, LED_ON);
      matrix.writeDisplay();

      //BROWS & HEAD START 3
      servos(95, 60, 90, 90);
      //pan, tilt, left, right
      
      //LCD 1
      lcd.print("O-O");
      delay(500);
      lcd.clear();

      matrix.clear();

      break;
}
var=0;
  }



void servos(int pangoal, int tiltgoal, int lbrowgoal, int rbrowgoal) {
  while ((tiltservo.read() != tiltgoal) || (panservo.read() != pangoal) || (browoneservo.read() != lbrowgoal) || (browtwoservo.read() != rbrowgoal)) {
    if (panservo.read() > pangoal) { 
      panservo.write(panservo.read() - 1);
    } else if (panservo.read() < pangoal) {
      panservo.write(panservo.read() + 1);
    }
 
    if (tiltservo.read() > tiltgoal) {
      tiltservo.write(tiltservo.read() - 1);
    } else if (tiltservo.read() < tiltgoal) {
      tiltservo.write(tiltservo.read() + 1);
    }
   
    if (browoneservo.read() > lbrowgoal) {
      browoneservo.write(browoneservo.read() - 1);
    } else if (browoneservo.read() < lbrowgoal) {
      browoneservo.write(browoneservo.read() + 1);
    }
 
    if (browtwoservo.read() > rbrowgoal) {
      browtwoservo.write(browtwoservo.read() - 1);
    } else if (browtwoservo.read() < rbrowgoal) {
      browtwoservo.write(browtwoservo.read() + 1);
    }
   
    delay(15);
    SoftwareServo::refresh();
  }
}
