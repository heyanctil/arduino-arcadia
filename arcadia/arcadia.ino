#include <pcmConfig.h>
#include <TMRpcm.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h> 

const long  ATTACK_MODE_LENGTH = 40000;     // The lenght in time the attack mode will last.

const int CANNON1_PIN = 9;                  // Arduino pin connected to cannon servo motor's pin.
const int SPEAKER_PIN = 8;                  // Arduino pin connected to the speaker's pin.
const int ATTACK_BUTTON_PIN = 7;            // Arduino pin connected to the attack button's pin.
const int LASER1_PIN = 3;

TMRpcm audio;                               // Audio object.
File root;                                  // File system object.
Servo servoCannon1;                         // Create servo cannon 1 object.

int initialAngle = 90;                      // The initial angle of cannons.
int attackButtonState;                      // Attack button state.

void setup() {
  Serial.begin(9600);                       // Initialize serial

  Serial.print("Initializing SD card...");  // Check if SD card is set correctly.
  if (!SD.begin()) {
    Serial.println("failed!");
    while(true);  // stay here.
  }
  Serial.println("OK!");
  root = SD.open("/");                      // Open SD card main root.
  printDirectory(root, 0);                  // Print all files names and sizes.

  audio.speakerPin = SPEAKER_PIN;           // Set speaker output pin.
  audio.setVolume(7);                       // Set volume level (0 to 7).
  audio.quality(1);                         // Set 1 for 2x oversampling Set 0 for normal
  
  pinMode(ATTACK_BUTTON_PIN, INPUT_PULLUP); // Set attack button pin to input pull-up mode.
  
  servoCannon1.attach(CANNON1_PIN);         // Attaches the cannon 1 on pin 8.
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

    // Start audio.
    audio.play("AUDIO.WAV");
    
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

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
