#include <LiquidCrystal.h>
#define BASEFREQUENCY 440.0
#define SHARP 1
#define NATURAL 0
#define FLAT -1
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);

const int buttonPins[8] = {6, 7, 8, 9, 10, 11, 12, 13};
const int stickPinY = A0;
const int stickPinX = A1;
const int buzzerPin = A2;
int valueY = 0;
int valueX = 0;

int frequencyIndex = -1;
float frequency[8];
float f = 0.0;
char noteName[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'C'};

void setup() {
  for (int i = 0; i < 8; ++i){
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(stickPinY, INPUT);
  pinMode(stickPinX, INPUT);

  int index = 0;
  for (int i = 0; i < 12; ++i){
    if (i==1 || i==3 || i==6 || i==8 || i==10){
      continue;
    }
    frequency[index] = BASEFREQUENCY * pow(2, (i-9)/12.0);
    ++index;
  }
  frequency[7] = shiftHigh(frequency[0]);
  
  lcd.begin(16, 2);
  lcd.print("Buttons&Joystick");
  lcd.setCursor(0, 1);
  lcd.print("    Keyboard    ");
  
  delay(1000);  
  lcd.clear();
}

void loop() {
  
  valueY = analogRead(stickPinY);
  valueX = analogRead(stickPinX);
  
  lcd.setCursor(0, 0);
  lcd.print(" X=");
  lcd.print(valueX);
  
  lcd.setCursor(7, 0);
  lcd.print(" Y=");
  lcd.print(valueY);
  
  for (int i = 0; i < 8; ++i){
    if (digitalRead(buttonPins[i]) == 1){
      frequencyIndex = i;
      break;
    }
  }

  if (frequencyIndex == -1){
    printFrequency(0.00);
    noTone(buzzerPin);
  }else{
    makeSoundAndLcd(frequencyIndex);
    frequencyIndex = -1;
  }
  
  delay(20);
  lcd.clear();
}


void makeSoundAndLcd(int index){
  f = frequency[index];
  if (valueX > 562 && valueY < 462){
    f = shiftHighSharp(f);
    printLcd(f, index, SHARP);
  }else if (valueX > 562 && valueY > 562) {
    f = shiftHighFlat(f);
    printLcd(f, index, FLAT);
  }else if (valueX < 462 && valueY < 462) {
    f = shiftLowSharp(f);
    printLcd(f, index, SHARP);
  }else if (valueX < 462 && valueY > 562) {
    f = shiftLowFlat(f);
    printLcd(f, index, FLAT);
  }else if (valueX > 562){
    f = shiftHigh(f);
    printLcd(f, index, NATURAL);
  }else if (valueX < 462) {
    f = shiftLow(f);
    printLcd(f, index, NATURAL);
  }else if (valueY < 462) {
    f = sharp(f);
    printLcd(f, index, SHARP);
  }else if (valueY > 562) {
    f = flat(f);
    printLcd(f, index, FLAT);
  }else {
    f = frequency[index];
    printLcd(f, index, NATURAL);
  }
  tone(buzzerPin, int(f), 50);
}

float shiftHighSharp(float f){
  f = shiftHigh(f);
  f = sharp(f);
  return f;
}

float shiftHighFlat(float f){
  f = shiftHigh(f);
  f = flat(f);
  return f;
}

float shiftLowSharp(float f){
  f = shiftLow(f);
  f = sharp(f);
  return f;
}

float shiftLowFlat(float f){
  f = shiftLow(f);
  f = flat(f);
  return f;
}

float shiftHigh(float f){
  return 2.0*f;
}

float shiftLow(float f){
  return 0.5*f;
}

float sharp(float f){
  return pow(2, 1/12.0)*f;
}

float flat(float f){
  return pow(2, -1/12.0)*f;
}

void printLcd(float f, int index, int state){
  printFrequency(f);
  printNoteName(index, state);
}

void printFrequency(float f){
  lcd.setCursor(0, 1);
  lcd.print(" f = ");
  lcd.print(f);
}

void printNoteName(int index, int state){
  lcd.setCursor(13, 1);
  lcd.print(noteName[index]);
  if (state == 1){
    lcd.print('#');
  }
  if (state == -1){
    lcd.print('b');
  }
}
