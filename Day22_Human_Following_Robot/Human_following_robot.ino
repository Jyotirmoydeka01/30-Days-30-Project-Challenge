// Day 22 – Human-Following Robot using 2 Ultrasonic Sensors and L298N Motor Driver

// Left Ultrasonic Sensor
const int trigLeft = 4;
const int echoLeft = 5;

// Right Ultrasonic Sensor
const int trigRight = 6;
const int echoRight = 7;

// L298N Motor Driver Pins
const int IN1 = 8;  // Left Motor +
const int IN2 = 9;  // Left Motor -
const int IN3 = 10; // Right Motor +
const int IN4 = 11; // Right Motor -

// Threshold to decide if object is close
const int minDistance = 25; // in cm

void setup() {
  // Motor pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensor pins
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);

  Serial.begin(9600);
}

void loop() {
  int distLeft = getDistance(trigLeft, echoLeft);
  int distRight = getDistance(trigRight, echoRight);

  Serial.print("Left: ");
  Serial.print(distLeft);
  Serial.print(" cm | Right: ");
  Serial.print(distRight);
  Serial.println(" cm");

  // Decision-making logic
  if (distLeft <= minDistance && distRight <= minDistance) {
    moveForward();
  } else if (distLeft <= minDistance) {
    turnLeft();
  } else if (distRight <= minDistance) {
    turnRight();
  } else {
    stopMotors();
  }

  delay(200); // Small delay to stabilize readings
}

// Function to measure distance using ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  return distance;
}

// Motor Control Functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
