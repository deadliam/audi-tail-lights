
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define brakeSwitch_pin 4
#define parkingSwitch_pin 7
#define stopLight_pin 3
#define tailLight_pin1 5
#define tailLight_pin2 9
#define neoLights_pin 6

#define NUMPIXELS 97

// init variables
int brakeSwitchState = 0;
int parkingSwitchState = 0;
boolean isParkingLightsOn = 0;
int highBrightness = 255;
int lowBrightness = 20;
int flameBrightness = 250;
int colorHigh = 255;
int colorLow = 20;

// NeoPixels
// 1 - 14   left corner
// 15 - 46  left center
// 83 - 96  right center
// 47 - 82  right corner

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neoLights_pin, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(brakeSwitch_pin, INPUT_PULLUP); //INPUT_PULLUP
  pinMode(parkingSwitch_pin, INPUT_PULLUP);
  pinMode(stopLight_pin, OUTPUT);
  pinMode(tailLight_pin1, OUTPUT);
  pinMode(tailLight_pin2, OUTPUT);
  pixels.begin();
}

//Action functions
void parkingLightsAction(String state) {
  if (state == "On") {
    isParkingLightsOn = 1;
    analogWrite(stopLight_pin, lowBrightness);
    analogWrite(tailLight_pin1, lowBrightness);
    analogWrite(tailLight_pin2, lowBrightness);  
  }
  if (state == "Off") {
    isParkingLightsOn = 0;
    analogWrite(stopLight_pin, 0);
    analogWrite(tailLight_pin1, 0);
    analogWrite(tailLight_pin2, 0);  
  }
}

void parkingLightsActionSlow(String state) {
  int lightSpeed = 15;
  if (state == "On") {
    isParkingLightsOn = 1;
    for (int i = 0; i < 100; i = i + 2) {
      analogWrite(stopLight_pin, i);
      analogWrite(tailLight_pin1, i);  
      analogWrite(tailLight_pin2, i); 
      delay(lightSpeed);
    }
    analogWrite(stopLight_pin, lowBrightness);
    analogWrite(tailLight_pin1, lowBrightness);
    analogWrite(tailLight_pin2, lowBrightness);  
  }
  if (state == "Off") {
    isParkingLightsOn = 0;
    for (int i = 100; i > 0; i = i - 2) {
      analogWrite(stopLight_pin, i);
      analogWrite(tailLight_pin1, i);  
      analogWrite(tailLight_pin2, i); 
      delay(lightSpeed);
    }
    analogWrite(stopLight_pin, 0);
    analogWrite(tailLight_pin1, 0);
    analogWrite(tailLight_pin2, 0);  
  }
}

void brakingLightsAction(String state) {
  if (state == "On") {
    analogWrite(stopLight_pin, highBrightness);
    if (isParkingLightsOn == 1) {
      analogWrite(tailLight_pin1, lowBrightness);
      analogWrite(tailLight_pin2, lowBrightness);
    } else {
      analogWrite(tailLight_pin1, highBrightness);
      analogWrite(tailLight_pin2, highBrightness);
    }
  } 
  if (state == "Off") {
    if (isParkingLightsOn == 1) {
      analogWrite(stopLight_pin, lowBrightness);
      analogWrite(tailLight_pin1, lowBrightness);
      analogWrite(tailLight_pin2, lowBrightness);
    } else {
      analogWrite(stopLight_pin, 0);
      analogWrite(tailLight_pin1, 0);
      analogWrite(tailLight_pin2, 0);  
    }
  }
}

void flameUp(int section) {
  int lightSpeed = 15;
  for (int i = 0; i < flameBrightness; i = i + 3) {
    analogWrite(section, i);
    delay(lightSpeed);
  }
  analogWrite(section, highBrightness);
}

void flameDown(int section) {
  int lightSpeed = 15;
  for (int i = flameBrightness; i > 0; i = i - 3) {
    analogWrite(section, i);
    delay(lightSpeed);
  }
  analogWrite(section, 0);
}

void flameLightAction(String state) {
  if (state == "Up") {
    flameUp(stopLight_pin);
    flameUp(tailLight_pin1);
    flameUp(tailLight_pin2);
  }
  if (state == "Down") {
    flameDown(stopLight_pin);
    flameDown(tailLight_pin1);
    flameDown(tailLight_pin2);
  }
}

void brakeLightOnSwitch() {  
  brakeSwitchState = digitalRead(brakeSwitch_pin);
  if (brakeSwitchState == HIGH) {
    // turn LED on:
    brakingLightsAction("On");
  } else {
    // turn LED off:
    brakingLightsAction("Off");
  }
}

void parkingLightOnSwitch() {  
  parkingSwitchState = digitalRead(parkingSwitch_pin);
  if (parkingSwitchState == HIGH) {
    // turn on:
    parkingLightsAction("On");
  } else {
    // turn off:
    parkingLightsAction("Off");
  }
}

void parkingLightsOnSlow() {
  parkingSwitchState = digitalRead(parkingSwitch_pin);
  if (parkingSwitchState == HIGH) {
    // turn on:
    if (isParkingLightsOn == 0) {
      parkingLightsActionSlow("On");
    }
  } else {
    // turn off:
    if (isParkingLightsOn == 1) {
      parkingLightsActionSlow("Off");
    }
  }
}

// NeoPixels

void pixelsOff() {
  int j = 46;
  for(int i = 47; i < 97; i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.setPixelColor(j, pixels.Color(0,0,0));
    j -= 1;
  }
  pixels.show();
}

void toOutside(int delaySpeed1, int colorH, int colorL) {
  int count = 47;
//  int delaySpeed1 = 15;
  int delaySpeed2 = delaySpeed1 + 5;
  for(int i = 48; i < 97; i++){
    pixels.setPixelColor(i + 1, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(i, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(i - 1, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(i - 2, pixels.Color(colorL, colorL, 0));
    
    pixels.setPixelColor(count, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(count + 1, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(count + 2, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(count + 3, pixels.Color(colorL, colorL, 0));
    pixels.show();
    if (i > 65) {
      delay(delaySpeed1);  
    } else {
      delay(delaySpeed2);
    }
    count -= 1;
  }
}

void toInside(int delaySpeed1, int colorH, int colorL) {
  int j = 0;
//  int delaySpeed1 = 15;
  int delaySpeed2 = delaySpeed1 + 5;
  for(int i = 97; i > 49; i--) {
    pixels.setPixelColor(j, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(j - 1, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(j - 2, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(j - 3, pixels.Color(colorL, colorL, 0));

    pixels.setPixelColor(i - 2, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(i - 1, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(i, pixels.Color(colorH, colorH, 0));
    pixels.setPixelColor(i + 1, pixels.Color(colorL, colorL, 0));
    pixels.show();
    if (i > 65) {
      delay(delaySpeed1);  
    } else {
      delay(delaySpeed2);
    }
    j += 1;
  }
  for(int i = 44; i < 52; i++) {
    pixels.setPixelColor(i + 1, pixels.Color(colorL, colorL, 0));
  }
  pixels.show();
}

void openCar() {
  toOutside(10, 250, 10);
  toInside(0, 250, 10);
  toOutside(10, 250, 250);

  flameLightAction("Up");
  delay(2000);
  parkingLightsActionSlow("Off");
  pixelsOff();
}

void closeCar() {
  brakingLightsAction("On");
  toInside(10, 250, 10);
  toOutside(0,  250, 10);
  toInside(10, 250, 250);

  flameLightAction("Down");
  brakingLightsAction("Off");
  toInside(10, 250, 0); 
}

// MAIN
void loop() {
  
  brakeLightOnSwitch();
//  parkingLightOnSwitch();
  parkingLightsOnSlow();

  pixelsOff();
  
  delay(1000);
//  
//  openCar();
//  
//  delay(5000);

  closeCar();

  delay(5000);
  
}
