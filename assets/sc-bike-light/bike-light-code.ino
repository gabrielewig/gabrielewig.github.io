/*
 * Speed Controlled Bike Light
 * Mechatronics Individual Project, Gabriel Ewig gre27
 * 
 * This project is a prototype of a speed-controlled bike light.
 * Using a small wheel and a series of low-power LEDs,
 * the setup demonstrates the measurement of speed with a magnet and hall sensor,
 * and response of the LEDs by turning on a “brake light” when deceleration is detected.
 * 
 * This code has two main components: a main loop that sets the servo speed and LED state,
 * and a interrupt that triggers when the hall sensor detects the magnet and exceeds 1.1V.
 * The arduino keeps track of the period between these "pings", and activates the brake light
 * if the period increases, indicating deceleration.
 */

#include <Servo.h>

// Setup servo
Servo wheel_servo;

// Create global variables for the wheel speed, and state triggering
int period[2]; // Stores past two period measurements
int last_ping; // Temporary storage of last ping time to filter double-count
int brake_trigger; // Count down to keep brake on after trigger
String state = "normal"; // State name, for serial reference
bool led = false; // Helps toggle LEDs on/off


// Define digital pins that are connected
int led1 = 9;
int led2 = 3;
int led3 = 5;

void setup() {
  Serial.begin(9600); // Start debugging info to serial
  
  // Set pin mode
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(A0, INPUT);

/* Setup an analog interrupt on pin 7.
 * This detects when the output of the hall sensor is rising, and triggers the ISR function.
 * This uses the internal arduino comparator, and triggers if the voltage exceeds 1.1V.
 * This method of interrupts allows the speed to be more responsive, while also continuously updating the light behavior.
 */
  pinMode(7,INPUT);
   ACSR = B01011010;
  
  // Attach servo
  wheel_servo.attach(11);
  
  // Initially, set all LEDs to full brightness.
  analogWrite(led1, 255);
  analogWrite(led2, 255);
  analogWrite(led3, 255);
}

ISR(ANALOG_COMP_vect) {
  // This is an interrupt function that is called when the the magnet begins to be sensed.
  // The function compares the current time to the last recorded time, and adds that period to the period list.
  int temp = millis() - last_ping;
  //Serial.println("Hall!");
  if (temp > 200){ // Prevents double counts if magnet is slow. At these speeds, the period is always > 200ms.
    last_ping = millis();     
    period[1] = period[0];
    period[0] = temp;
  }
}

void loop() {
  /*
   * STEP 1: Read potentiometer input and set servo speed proportionally.
   *
   * This potentiometer has non-linear resistance (see datasheet).
   * By dividing the range at 200 (about 60% of slide), the speed response is more natural feeling.
   * Each range has a different multiplying factor, to translate the pot reading (0-1024) to a servo speed (0-180).
   * Below pot=110, the very end of the range, the servo is stopped.
   * 
   */
  
  int pot = analogRead(A0);
  int servo_speed;
  if (pot < 110) {
    servo_speed = 90;
  } else if (pot < 200) {
    servo_speed = 90 + pot*0.3;
  } else {
    servo_speed = 150 + (pot-200)*0.09;
  }
  wheel_servo.write(servo_speed);

  /*
   * STEP 2: Set light behavior based on recorded speed of wheel
   * 
   * First, this checks if the brake was recently triggered, and continues to show the brake for a moment.
   * Otherwise, this checks if the last period increased from the previous one by over 80ms, and triggers a brake event if this is the case.
   * This sets the red lights to solid, and sets brake_trigger to 8 so that the light remains on for the next 8 loops.
   * In all other situations, the LEDs are toggled to create a flashing pattern.
   */
   
  if (brake_trigger > 0) { // Continued Braking State
    state = "braking-cont";
    analogWrite(led1, 255);
    analogWrite(led2, 255);
    analogWrite(led3, 255);
    brake_trigger--;
  } else if (period[1] - period[0] < -80){ // Detects deceleration and activates brake
    state = "braking";
    brake_trigger = 8;
    analogWrite(led1, 255);
    analogWrite(led2, 255);
    analogWrite(led3, 255);
  } else { // Normal flashing
    state = "normal";
    led = !led;
    if (led){
      analogWrite(led1, 255);
      analogWrite(led2, 255);
      analogWrite(led3, 255);
    } else {
      analogWrite(led1, 0);
      analogWrite(led2, 0);
      analogWrite(led3, 0);
    }
  }
  

  // Some helpful serial outputs
  //Serial.println("Pot: " + String(pot) + ", Servo: " + String(servo_speed));
  Serial.println("Speed: " + String(period[0]) + ", Last Speed: " + String(period[1]) + ", State: " + state);
  delay(250); // Delay. Effects flashing rate of LEDs, and slows down serial output.
}
