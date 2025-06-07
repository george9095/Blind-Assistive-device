#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Servo.h>  // Include the Servo library

// Create software serial for DFPlayer Mini communication
SoftwareSerial mySerial(9, 10); // RX, TX connected to Pins 9 and 10
DFRobotDFPlayerMini myDFPlayer;

// Ultrasonic Sensor Pins
const int trigPin = 2;
const int echoPin = 3;

// Servo Pin
Servo myServo;  // Create Servo object
const int servoPin = 6; // Pin connected to the servo

// Initialize DFPlayer Mini, Ultrasonic sensor, and Servo
void setup() {
  // Start serial communication
  Serial.begin(9600);
  mySerial.begin(9600);

  // Initialize DFPlayer Mini
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer not detected.");
    while (true); // Stay here if the DFPlayer is not detected
  }

  myDFPlayer.volume(30); // Set volume (0-30)
  Serial.println("DFPlayer Initialized.");

  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize Servo
  myServo.attach(servoPin);  // Attach the servo to the specified pin
  myServo.write(90);  // Set the initial position to the center (90 degrees)
}

// Main loop
void loop() {
  long distance = measureDistance(); // Get distance from ultrasonic sensor

  // Print the distance for debugging
  Serial.print("Distance: ");
  Serial.println(distance);

  // Condition 1: If the distance is very close (<= 40 cm), stop
  if (distance <= 40) {
    myDFPlayer.play(3); // Play "Stop"
    myServo.write(90);  // Set servo to neutral position (stop)
    delay(2000); // Wait to avoid immediate repetition
  }
  // Condition 2: If the distance is in range (15 cm < distance <= 100 cm), go back
  else if (distance > 15 && distance <= 100) {
    myDFPlayer.play(2); // Play "Go back"
    myServo.write(0);   // Move servo to a position indicating go back
    delay(2000); // Wait to avoid immediate repetition
  }
  // Condition 3: If the distance is greater than 100 cm, move forward
  else if (distance > 100) {
    myDFPlayer.play(1); // Play "Move forward"
    myServo.write(90);  // Set servo to neutral position (center)
    delay(2000); // Wait to avoid immediate repetition
  }

  delay(200); // Delay before the next check
}

// Function to measure distance using ultrasonic sensor
long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Convert time to distance (in cm)
  return distance;
}
