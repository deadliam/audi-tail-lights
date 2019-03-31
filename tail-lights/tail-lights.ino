
#define brakeSwitch_pin 4
#define parkingSwitch_pin 7
#define stopLight_pin 3
#define tailLight_pin1 5
#define tailLight_pin2 9

boolean butt_flag = 0;      // флажок нажатия кнопки
boolean butt;               // переменная, харнящая состояние кнопки
int brakeSwitchState = 0;
int parkingSwitchState = 0;
boolean flag = 0;           // флажок режима
boolean isParkingLightsOn = 0;
unsigned long last_press;   // таймер для фильтра дребезга
int highBrightness = 255;
int lowBrightness = 20;
int flameBrightness = 180;

void setup() {
  pinMode(brakeSwitch_pin, INPUT_PULLUP); //INPUT_PULLUP
  pinMode(parkingSwitch_pin, INPUT_PULLUP);
  pinMode(stopLight_pin, OUTPUT);
  pinMode(tailLight_pin1, OUTPUT);
  pinMode(tailLight_pin2, OUTPUT);
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
    for (int i = 0; i < flameBrightness; i = i + 5) {
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
    for (int i = flameBrightness; i > 0; i = i - 5) {
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
  for (int i = 0; i < flameBrightness; i = i + 2) {
    analogWrite(section, i);
    delay(lightSpeed);
  }
  analogWrite(section, highBrightness);
}

void flameDown(int section) {
  int lightSpeed = 15;
  for (int i = flameBrightness; i > 0; i = i - 2) {
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
    // turn LED on:
//    parkingLightsActionSlow("On");
    parkingLightsAction("On");
  } else {
    // turn LED off:
//    parkingLightsActionSlow("Off");
    parkingLightsAction("Off");
  }
}

void loop() {
  
  brakeLightOnSwitch();
  parkingLightOnSwitch();
  
}
