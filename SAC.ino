//define all the L298N bridge pin that are used to control the 4 motor (interpreted as 2 motor for the board Left/Right)
#define enA 10
#define in1 9
#define in2 8
#define enB 5
#define in3 7
#define in4 6

int rotDirection = 0;
const int TRIGGER_PIN = A0;
const int ECHO_PIN = A1;
const int inter_time = 1000;

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
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void loop() {
  //Get information from the sonic range sensor
  unsigned long duration;
  float distance;
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(500);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  distance = (duration / 2.0) / 29.0;
  Serial.print("Data:");
  Serial.print (millis() / 1000.0);
  Serial.print(", d = ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(inter_time);


  //If no object at less than 15 centimeter continue to drive
  if (distance > 15) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    Serial.print(pwmOutput);
    analogWrite(enA, 150);
    analogWrite(enB, 150);
  }

  //Stop motor and invert the motor direction turn left
  if (distance <= 15) {
    int i = 255;
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, 100);
    analogWrite(enB, 100);
  }
}
