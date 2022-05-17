/// The Doorman ///

////
    #include <Servo.h>
    
    const int servo1 = 3; 
    const int servo2 = 5; 
    const int servo3 = 6; 
    const int servo4 = 9;
    const int servo5 = 10;  
    
    int servoVal; 
    
    Servo myservo1;
    Servo myservo2;
    Servo myservo3;
    Servo myservo4;
    Servo myservo5;
    
////     
    int ledPin = 13;                // choose the pin for the LED
    int ledPin2 = 12;
    int inputPin = 2;               // choose the input pin (for PIR sensor)
    int pirState = LOW;             // we start, assuming no motion detected
    int val = 0;                    // variable for reading the pin status
////     
    void setup() {
      
      myservo1.attach(servo1); 
      myservo2.attach(servo2);  // attaches the servo
      myservo3.attach(servo3);  // attaches the servo
      myservo4.attach(servo4);  // attaches the servo
      myservo5.attach(servo5);  // attaches the servo
      
      pinMode(13, OUTPUT);
      pinMode(12, OUTPUT);
      pinMode(ledPin, OUTPUT);      // declare LED as output
      pinMode(inputPin, INPUT);     // declare sensor as input
     
      Serial.begin(9600);
    }
     
    void loop(){

/////// ALIGNMENT ///////

         myservo1.write(90);
         myservo2.write(90);
         myservo3.write(90);
         myservo4.write(90);
         myservo5.write(90);
         

      delay(1000);
////
      val = digitalRead(inputPin);  // read input value
      if (val == HIGH) {            // check if the input is HIGH
        digitalWrite(ledPin, HIGH);  // turn LED ON
        if (pirState == LOW) {
          // we have just turned on
          Serial.println("Motion detected!");
          // We only want to print on the output change, not state
          pirState = HIGH;

/////////////// FIRST MOVEMENT /////////
               
         myservo1.write(150);  // OPEN EYES
         delay(2000);          // Wait 5 seconds
         myservo2.write(75);  // TURN EYE 1 LEFT
         myservo3.write(75);  // TURN EYE 2 LEFT

         delay(1000); 

////////////// SECOND MOVEMENT //////////

         myservo2.write(105);  // TURN EYE 1 RIGHT
         myservo3.write(105);  // TURN EYE 2 RIGHT

         delay(1000); 

/////////////// THIRD MOVEMENT /////////
          
         myservo2.write(105);  // TURN EYE 1 LEFT
         myservo3.write(105);  // TURN EYE 2 LEFT

         delay(1000); 

////////////// FOURTH MOVEMENT //////////

         myservo2.write(75);  // TURN EYE 1 RIGHT
         myservo3.write(75);  // TURN EYE 2 RIGHT

         delay(2000); 
         
////////////// FIFTH MOVEMENT //////////

         myservo2.write(75);  // TURN EYE 1 LEFT
         myservo3.write(75);  // TURN EYE 2 LEFT

         delay(1000); 

////////////// SIXTH MOVEMENT //////////  

         myservo2.write(105);  // TURN EYE 1 RIGHT
         myservo3.write(105);  // TURN EYE 2 RIGHT

         delay(1000); 
       
        }
      } else {
        digitalWrite(ledPin, LOW); // turn LED OFF
        if (pirState == HIGH){
          // we have just turned of
          Serial.println("Motion ended!");
          // We only want to print on the output change, not state
          pirState = LOW;
   
         delay(3000); 
         
         myservo1.write(90);   // CLOSE EYES

         delay(1000);          // Wait 1 second

        }
      }
    }
