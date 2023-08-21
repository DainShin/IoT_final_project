

/*** Pet Potty Alarm System  ***/
// Name : Dain Shin
// Stu# : 200535561
 


#include <LiquidCrystal.h>
#include <Wire.h>
#include<Servo.h>


Servo myservo;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int trig = 7; 
int echo = 6;  
int maxTime = 1; // Time to detect feces and unrine( I set this 1 because time goes slower in Tinkercad). In real life it will be 10seconds
int maxTime2 = 2; // I set the time as 2 seconds for the same reason. In real life, it will be 20 seconds
int count = 0;
int motion = 0;

int maxDistance = 20; // Max distance

void setup()
{
  myservo.attach(13);
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  lcd.begin(16, 2);
  myservo.write(0);
  pinMode(motion, INPUT);
}

void loop()
{
  digitalWrite(trig, LOW);  // At the very first, the ultrasonic signal is Low
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); // Generating the ultrasonic signal
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH); // Receiving the ultrasonic signal
  long distance = duration * 0.034 / 2.0;
  
  int state = 0;  // 0: the pet is on the pad / 1: the pet is out side of the pad
  int state1 = 0;  
  int start_time = 0; // For storing the moment of the pet on the pad  
  int end_time = 0;   // For storing the moment of the pet out of the pad 
  int elapse_time = 0; 

  // if the distance of pet from the sensor is less than "maxDistance"
  // the time will be recorded and "state is changed to 1" 
  if(distance <= maxDistance) {
    Serial.print("state: ");
    Serial.println(state);
    start_time = millis()/1000;
    Serial.print("Start time: ");
    Serial.println(start_time);
  }

  // while "state is 1", which means that pet is on the pad
  while (state == 1) 
  {
    digitalWrite(trig, LOW); // at the very first, the ultrasonic status should be LOW
    delayMicroseconds(2);
    digitalWrite(trig, HIGH); // To generate the ultrasonic signal 
    delayMicroseconds(10); 
    digitalWrite(trig, LOW);  // the signal will be low again to detect the time   
    duration = pulseIn(echo, HIGH);  // from the time of starting high signal to low signal, it detect the time 
    distance = duration * 0.034 / 2.0; // this is the fomula to change digit from "micromilliseconds" to "cm"

    // To detect the moment when the pet leaves the pad
    if (distance > maxDistance) 
    {
      end_time = millis()/1000;
      Serial.print("End time: ");
      Serial.println(end_time);
      elapse_time = end_time - start_time;
      Serial.print("Elapse time: ");
      Serial.println(elapse_time);
      
      // start_time - end_time = elase_time
      // if elapse time is longer than maxTime, then it will print out the alarm message
      if(elapse_time >=  maxTime && elapse_time < maxTime2)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Urine");
        lcd.setCursor(1,1);
        lcd.print("Detected");
        count ++;
        delay(2000);
        myservo.write(90);
        delay(1000);
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Giving Snack"); // it gives the snack
        delay(1000);
        myservo.write(0);  
      }
      else if(elapse_time >= maxTime2)   // if elapse time is longer than maxTime2, then it will print out another alarm message
      {
      	lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Feces");
        lcd.setCursor(1,1);
        lcd.print("Detected");
        count ++;
        delay(2000);
        myservo.write(90);  // Servo motor will move to give the snack as a prize
        delay(1000);
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Giving Snack"); 
        delay(1000);
        myservo.write(0); 
      }     
      state = 0; 
    } 
   
  }
  
  // if the dog used the pad twice, then we need to change the pad
  if(count == 2)
  {	
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Change the Pad");
    delay(1000);
  	count = 0;
  } 
} // void loop