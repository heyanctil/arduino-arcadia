#include <Servo.h>

const long  ATTACK_MODE_LENGTH = 40000;       // The lenght in time the attack mode will last.

const int ATTACK_BUTTON_PIN = 7;            // Arduino pin connected to the attack button's pin.
const int CANNON1_PIN = 9;                  // Arduino pin connected to cannon servo motor's pin.
const int LASER1_PIN = 3;

Servo servoCannon1;                         // Create servo cannon 1 object.

int initialAngle = 90;                      // The initial angle of cannons.
int attackButtonState;                      // Attack button state.

void setup() {
  Serial.begin(9600);                       // Initialize serial
  
  pinMode(ATTACK_BUTTON_PIN, INPUT_PULLUP); // Set attack button pin to input pull-up mode.
  
  servoCannon1.attach(CANNON1_PIN);         // Attaches the cannon 1 on pin 9.
  servoCannon1.write(initialAngle);

  pinMode(LASER1_PIN, OUTPUT);              // Set laser 1 pin to output mode
}

void loop() {
 attackButtonState = digitalRead(ATTACK_BUTTON_PIN); // Read attack button state.

  // Attack mode activated.  
  if(attackButtonState == LOW) {
    long attackLength = 0;
    
    delay(500);

    // Initialize attack.
    initializeAttack(); 

    // TODO Start attack music!
    
    // Launch attack sequence.
    while (attackLength < ATTACK_MODE_LENGTH) {
      // TODO Add stop mechanism
      
      // TODO Should move in a range based on the attack. 
      int angleOfAttackCannon1 = random(10, 170);
      
      // Control cannon servo 1.
      servoCannon1.write(angleOfAttackCannon1);

      // Wait a little bit before firing.
      delay(500);

      // TODO Should add mechanism to fire or not cannons. 
      // Fire cannons.
      fire();

      attackLength += 1000; // Increment attack loop.
      delay(1000);
    } 

    // Reinitialize the cannons.
    servoCannon1.write(initialAngle);
  }
}

void fire() {
  // Fire!!
  for (int brightness = 0 ; brightness <= 255; brightness += 5) {
    analogWrite(LASER1_PIN, brightness);
    delay(1);
  }

  // Fading out cannons.
  for (int brightness = 255 ; brightness >= 0; brightness -= 5) {
    analogWrite(LASER1_PIN, brightness);
    delay(20);
  }
}

void initializeAttack() {
  int attack = random(1, 5);

  delay(8000);
  
  // Prepare the cannons.
  switch (attack) {
    case 1: // Enemy on left.
      servoCannon1.write(10);
      break;
      
    case 2: // Enemy at 10 o'clock.
      servoCannon1.write(55);
      break;

    case 3:
      // Enemy at 12 o'clock.
      servoCannon1.write(90);
      break;

    case 4:
      // Enemy at 2 o'clock.
      servoCannon1.write(125);
      break;

    case 5:
      // Enemy on right.
      servoCannon1.write(170);
      break;
  }

  // Wait for enemy!
  delay(16000);
}
