//define all the L298N bridge pin that are used to control the 4 motor (interpreted as 2 motor for the board Left/Right)
//Remake cable to avoid 9-10 of servo blocking the left side motors
#define enA 11
#define in1 8
#define in2 4
#define enB 5
#define in3 7
#define in4 6
//include the servo library
#include <Servo.h>
Servo serv;


int rotDirection = 0;
const int TRIGGER_PIN = A0;
const int ECHO_PIN = A1;
const int inter_time = 250;
//sensor face foward
int angle = 153;

void setup() {
  //initialise the sonic sensor
  Serial.begin(9600);
  pinMode (TRIGGER_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  //initialise the motor
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  //attach the servo to the broche of the car and put it to the face
  serv.attach(9);
  serv.write(angle);
}

void turnLeft(int sped){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enA, sped);
      analogWrite(enB, sped);
}

void turnRight(int sped){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enA, sped);
      analogWrite(enB, sped);
}

void moveFoward(int sped){
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, sped);
    analogWrite(enB, sped);
}

void moveBackward(int sped){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, sped);
    analogWrite(enB, sped);
}

void stopWheels(){
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

//return distance from sensor
int getDistance(){
  unsigned long duration;
  float distance;
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  distance = (duration / 2.0) / 29.0;
  delay(inter_time);
  return distance;
}


void loop() {
  //Get information from the sonic range sensor
  float distance, dist1, dist2;
  distance = getDistance();


  //If no object at less than 30 centimeters continue to drive
  if (distance > 30) {
    moveFoward (200);
    rotDirection = 0;
  }

  //Go backward go get less close to the wall
  if (distance < 10){
    stopWheels();
    moveBackward(70);
  }
  
  //Stop motor and invert the motor direction to turn left or right
  if (distance <= 30 && distance >=10) {
    stopWheels();
    
    //check on left and right to choose the better place to go
    angle = 179;
    serv.write(angle);
    dist1 = getDistance();
    delayMicroseconds(1000);
    angle = 127;
    serv.write(angle);
    dist2 = getDistance();
    delayMicroseconds(1000);
    angle = 153;
    serv.write(angle);

    if (dist1 >= dist2){
      //turn left
      turnLeft(120);
      rotDirection = 1;
      delayMicroseconds(5000);
      delay(inter_time*3);
    } else {
      //turn right
      turnRight(120);
      rotDirection = 2;
      delayMicroseconds(5000);
      delay(inter_time*3);
    }
  }
}
