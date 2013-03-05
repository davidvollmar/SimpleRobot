#define LEFT 0
#define RIGHT 1
#define FORWARD 0
#define BACKWARDS 1
#define MENABLED 0

const int pwmA = 3;
const int pwmB = 11;
const int dirA = 12;
const int dirB = 13;
const int pingPin = 22;
int read = 0;
int command = 0;
int setpoint = 0;

volatile long coder[2] = {
  0,0};
int lastSpeed[2] = {
  0,0}; 
long duration = 0;
long cm = 0;



void setup(){
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(dirB, OUTPUT);

  Serial.begin(9600);

  attachInterrupt(2, LwheelSpeed, CHANGE);    //init the interrupt mode for the digital pin 2
  attachInterrupt(3, RwheelSpeed, CHANGE);   //init the interrupt mode for the digital pin 3
}

void loop(){
  int enc1 = digitalRead(enc1);
  //Serial.println(enc1);
  ping();
  static unsigned long timer = 0;                //print manager timer

  if(millis() - timer > 100){                  
    Serial.print("Coder value: ");
    Serial.print(coder[LEFT]);
    Serial.print("[Left Wheel] ");
    Serial.print(coder[RIGHT]);
    Serial.println("[Right Wheel]");

    lastSpeed[LEFT] = coder[LEFT];   //record the latest speed value
    lastSpeed[RIGHT] = coder[RIGHT];
    //coder[LEFT] = 0;                 //clear the data buffer
    //coder[RIGHT] = 0;
    timer = millis();
  }
  if(cm > 25){
    command = 1;
    setpoint += 20;
  }
  else if(cm < 10){
    command = 2;
    setpoint += 20;
  } 
  else{
    command = 0;
  }



  if(Serial.available()>0){
    read = Serial.read();
  }
  //Serial.println(read);
  if(read == 48){
    /*digitalWrite(pwmA,LOW);
     digitalWrite(pwmB,LOW);
     */
    //command = 0;
    Serial.flush();
    //digitalWrite(pwmA, HIGH);
    //digitalWrite(pwmB, HIGH);

  } 
  else if (read == 49){
    digitalWrite(dirA, LOW);
    digitalWrite(dirB,LOW);
    /* analogWrite(pwmA, 100);
     analogWrite(pwmB, 100);
     */
    //command = 1;
    setpoint += 20;
    Serial.flush();
    read =0;
  }  
  else if (read == 50){
    digitalWrite(dirA, HIGH);
    digitalWrite(dirB, HIGH);
    /*analogWrite(pwmA, 100);
     analogWrite(pwmB, 100);
     */
    Serial.flush();
    //command = 2;
    setpoint +=20;
    read =0;
  } 
  else if (read ==51){
    //command = 3;
    read = 0; 

  }
  if(!MENABLED) command = 0;
  switch(command){
  case 0:
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);
    break;
  case 1:
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, LOW);
    if(coder[LEFT] < setpoint){
      analogWrite(pwmA, 255);
      analogWrite(pwmB, 255);
    }
    else{
      command = 0;
    }
    break;
  case 2:
    digitalWrite(dirA, HIGH);
    digitalWrite(dirB, HIGH);
    if(coder[LEFT] < setpoint){

      analogWrite(pwmA, 100);
      analogWrite(pwmB, 100);
    }
    else{
      command = 0;
    }
    break;
  case 3:
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, HIGH);
    analogWrite(pwmA, 200);
    analogWrite(pwmB, 200);
    delay(2000);
    analogWrite(pwmA,0);
    analogWrite(pwmB,0);
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, HIGH);

    command = 0;
    break;

  }


}
void LwheelSpeed()
{
  coder[LEFT] ++;  //count the left wheel encoder interrupts
}


void RwheelSpeed()
{
  coder[RIGHT] ++; //count the right wheel encoder interrupts
}


void ping(){
  // and the distance result in inches and centimeters:


  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();


}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int driveDist(int direction, int distance){
  if(distance < 0) distance = -distance;
  setDirection(direction);
  

}

void setDirection(int direction){
  switch(direction){
  case FORWARD:
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, LOW);
    break;
  case BACKWARDS:   
    digitalWrite(dirA, HIGH);
    digitalWrite(dirB, HIGH);
    break;
  }  
}




