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

//DECLARE STUFF
LiquidCrystal_I2C lcd(0x27,16,2); 
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();
SoftwareServo browoneservo;  
SoftwareServo browtwoservo;
SoftwareServo panservo;
SoftwareServo tiltservo;

// SET VARIABLES
int brow1pos = 0; 
int brow2pos = 0;
int tiltpos  = 0;
int panpos = 0;  
int antipos =0;
long duration; 
int distance; 
int var = 0; 

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
      sad_eye2_bmp [] =
      {B00000000,
       B00110000,
       B01001000,
       B01011100,
       B01011010,
       B01000010,
       B00111100,
       B00000000 };
    

void setup() {
  //SET UP
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor HC-SR04 Test");
  Serial.println("with Arduino UNO R3");

  matrix.begin(0x70);  
  browoneservo.attach(A2); 
  browtwoservo.attach(A3); 
  tiltservo.attach(A1);
  panservo.attach (A0);
  lcd.init(); 

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
    // SET VARIABLES FOR SWITCH FUNCTION
    if (1 < distance&&distance < 10){
      var = 1;
      Serial.println("SAD SAD SAD SAD SAD");
    }
    else if (10 < distance&&distance < 20){
       var = 2;
       Serial.println("ANGRY ANGRY ANGRY ANGRY ANGRY");
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

        for (brow1pos; brow1pos >= 150; brow1pos--) { 
          browoneservo.write(brow1pos);
          browtwoservo.write(brow1pos);              
          delay(5);                      
          SoftwareServo::refresh();                        
        }
        for (brow1pos; brow1pos <= 150; brow1pos++) { 
           browoneservo.write(brow1pos);
          browtwoservo.write(brow1pos);              
          delay(5);                      
          SoftwareServo::refresh();;                       
        }

        //HEAD 
        
        for (tiltpos; tiltpos >= 20; tiltpos--) { 
          Serial.println("top");
          Serial.println(tiltpos);
          tiltservo.write(tiltpos);              
          delay(50);
          SoftwareServo::refresh();                       
        }
        for (tiltpos; tiltpos <= 20; tiltpos++) { 
          Serial.println("bottom");
          Serial.println(tiltpos);
          tiltservo.write(tiltpos);              
          delay(50);
          SoftwareServo::refresh();                       
        }
        
        //LCD 1
        lcd.clear();
        lcd.print("Sad );");
        delay(50);
        
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
        for (brow1pos; brow1pos >= 300; brow1pos--) { 
          browoneservo.write(brow1pos);
          browtwoservo.write(brow1pos);              
          delay(5);                      
          SoftwareServo::refresh();                        
        }
        for (brow1pos; brow1pos <= 300; brow1pos++) { 
           browoneservo.write(brow1pos);
          browtwoservo.write(brow1pos);              
          delay(5);                      
          SoftwareServo::refresh();;                       
        }
        //HEAD
        Serial.println(tiltpos);
        for (tiltpos; tiltpos <= 90; tiltpos++) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          tiltservo.write(tiltpos);             
          delay(15);    
          SoftwareServo::refresh();                  
        }
        for (tiltpos; tiltpos >= 90; tiltpos--) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          tiltservo.write(tiltpos);             
          delay(15);   
          SoftwareServo::refresh();                   
        }

        //LCD 1
        lcd.backlight();
        lcd.begin(16, 2);
        lcd.setCursor(0,1);
        lcd.print("Angry >:(");
        delay(50);
        lcd.clear();
        
        matrix.clear();
        break;
          
      default:
        // NEUTRAL CASE
        Serial.println("NUETRAL");
        //EYES 3
        matrix.drawBitmap(0, 0, neutral_eyes_bmp, 8, 8, LED_ON);
        matrix.drawBitmap(0, 8, neutral_eyes_bmp, 8, 8, LED_ON);
        matrix.writeDisplay();

        //BROWS & HEAD START 3
        for (brow1pos; brow1pos >= 180; brow1pos--) { 
          browoneservo.write(brow1pos);
          browtwoservo.write(brow1pos);              
          delay(5);                      
          SoftwareServo::refresh();                        
        }
        for (brow1pos; brow1pos <= 180; brow1pos++) { 
           browoneservo.write(brow1pos);
          browtwoservo.write(brow1pos);              
          delay(5);                      
          SoftwareServo::refresh();;                       
        }

        for (tiltpos; tiltpos < 60; tiltpos++) { // goes from 180 degrees to 0 degrees
          tiltservo.write(tiltpos);
          SoftwareServo::refresh();
          delay(50);   
          //Serial.println("toptoptoptop");
        }

        for (tiltpos; tiltpos > 60; tiltpos--) { // goes from 180 degrees to 0 degrees
          tiltservo.write(tiltpos);
          SoftwareServo::refresh();
          delay(50); 
          //Serial.println("ncvo nffovfjnbdafofhbfluih ");  
        }
        
        //LCD 1
        lcd.print("Hello, world!");
        delay(5000);
        lcd.clear();

        matrix.clear();
  }
var=0;
    }
