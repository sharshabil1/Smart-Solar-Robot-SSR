#include <Servo.h> 

Servo horizontal; 
int servohori = 90; 
int servohoriLimitHigh = 175;
int servohoriLimitLow = 5;

Servo vertical; 
int servovert = 45; 
int servovertLimitHigh = 100; 
int servovertLimitLow = 1;

//LDR Pin Connection
int ldrlt = A0; // Top Left 
int ldrrt = A3; // Top Right 
int ldrld = A1; // Bottom Left 
int ldrrd = A2; // Bottom Right 

//Settings
int tol = 50;       // Tolerance: Higher = less sensitive/jittery
int dtime = 20;     // Delay time: Higher = slower movement
int nightThresh = 50; // Threshold for darkness (0-1023)

void setup() {
  horizontal.attach(2); 
  vertical.attach(13);
  
  // neutral position
  horizontal.write(90);
  vertical.write(45);
  delay(2000); 
}

void loop() {
  int lt = readAvg(ldrlt);
  int rt = readAvg(ldrrt);
  int ld = readAvg(ldrld);
  int rd = readAvg(ldrrd);

  int avt = (lt + rt) / 2; // Top
  int avd = (ld + rd) / 2; // Down
  int avl = (lt + ld) / 2; // Left
  int avr = (rt + rd) / 2; // Right
  
  int totalLight = (avt + avd + avl + avr) / 4;

  if (totalLight < nightThresh) {
    toSunrise();
    return; 
  }

  int dvert = avt - avd;  
  int dhoriz = avl - avr; 

  //Vertical Movement
  if (abs(dvert) > tol) { 
    if (avt > avd) {
      // Top is brighter -> Move servo UP
      servovert--; 
      if (servovert < servovertLimitLow) servovert = servovertLimitLow;
    } else {
      // Down is brighter -> Move servo DOWN
      servovert++; 
      if (servovert > servovertLimitHigh) servovert = servovertLimitHigh;
    }
    vertical.write(servovert);
  }

  //Horizontal Movement 
  if (abs(dhoriz) > tol) { 
    if (avl > avr) {
      // Left is brighter -> Move LEFT
      servohori++; 
      if (servohori > servohoriLimitHigh) servohori = servohoriLimitHigh;
    } else {
      // Right is brighter -> Move RIGHT
      servohori--; 
      if (servohori < servohoriLimitLow) servohori = servohoriLimitLow;
    }
    horizontal.write(servohori);
  }

  delay(dtime);
}

// --- Helper Function: Read Average ---
// Takes 10 readings and returns the average to reduce noise
int readAvg(int pin) {
  long total = 0;
  for(int i=0; i<10; i++) {
    total += analogRead(pin);
    delay(2);
  }
  return total / 10;
}

// --- Helper Function: Return to Sunrise ---
void toSunrise() {
  while(servohori > 0) {
    servohori--;
    horizontal.write(servohori);
    delay(20);
  }
  vertical.write(45);
  

  delay(60000); 
}
