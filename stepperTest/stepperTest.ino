#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, 14, 12, 13, 5);
Stepper myStepper(stepsPerRevolution, 13, 14, 5, 12);

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  myStepper.setSpeed(50);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one step:
  myStepper.step(200);
  Serial.print("steps:");
  Serial.println(stepCount);
  stepCount++;
  delay(8);
}
