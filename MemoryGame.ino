
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include <EEPROM.h>

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1
SSD1306AsciiAvrI2c oled;

#define NOTE_A3  220
#define NOTE_CS5 554
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_D4  294

int level = 0;
byte sounds[150];
int speakerPin = 11;

int GreenLEDPin = 2;
int GreenInputPin = 3; 
int RedLEDPin = 4;
int RedInputPin = 5; 
int YellowLEDPin = 6;
int YellowInputPin = 7; 
int BlueLEDPin = 8;
int BlueInputPin = 9; 

long clkTime;

int address = 0;
int bestScore;



void setup() {
  Serial.begin(115200);
  Serial.print("init oled");
  #if RST_PIN >= 0
  Serial.print("reset pin");
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    Serial.print(" no reset pin");
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0
    // Call oled.setI2cClock(frequency) to change from the default frequency.
  Serial.print("set font");
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.set2X();

  oled.println("  Memory");
   oled.println();
  oled.println("   Game!");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(GreenLEDPin, OUTPUT);
  pinMode(GreenInputPin, INPUT);
  pinMode(RedLEDPin, OUTPUT);
  pinMode(RedInputPin, INPUT);
  pinMode(YellowLEDPin, OUTPUT);
  pinMode(YellowInputPin, INPUT);
  pinMode(BlueLEDPin, OUTPUT);
  pinMode(BlueInputPin, INPUT);

  Serial.print("makeShow();");
  bestScore = EEPROMReadlong(address);
 
  makeShow();
}
void sillySound(){
    digitalWrite(GreenLEDPin, HIGH);
    digitalWrite(RedLEDPin, HIGH);
    digitalWrite(YellowLEDPin, HIGH);
    digitalWrite(BlueLEDPin, HIGH);
    for(int i= 0;i<500;i++){
    tone(speakerPin, (i+200)%600, 300);
    delay(2);
    tone(speakerPin, (i+111)%600, 300);
  }
  
}




void makeShow(){
  clkTime = millis();
  bool flip = false;
  sillySound();
  //display_PressAny();
  while(true){
    for(int i= 0; i<4 ; i++){
      if(detectButtonPress(1) != -1){
        startGame();
        oled.clear();
      }
      switchOn(i,75);
      if(millis()-clkTime > 2000){
        clkTime = millis();
        if(flip){
          flip = false;
          display_BestScore();
        }else{
          flip = true;
          display_PressAny();
        }
        
      }
    }
  }
}
void startGame(){
  level = 0;
  for(int i=0;i<3;i++){
    display_GetReady();
    tone(speakerPin, NOTE_A3, 300);
    digitalWrite(GreenLEDPin, HIGH);
    digitalWrite(RedLEDPin, HIGH);
    digitalWrite(YellowLEDPin, HIGH);
    digitalWrite(BlueLEDPin, HIGH);
    delay(300);
    oled.clear();
    digitalWrite(GreenLEDPin, LOW);
    digitalWrite(RedLEDPin, LOW);
    digitalWrite(YellowLEDPin, LOW);
    digitalWrite(BlueLEDPin, LOW);
    delay(300);
  }
  display_Go();
  tone(speakerPin, NOTE_CS5, 400);
  digitalWrite(GreenLEDPin, HIGH);
  digitalWrite(RedLEDPin, HIGH);
  digitalWrite(YellowLEDPin, HIGH);
  digitalWrite(BlueLEDPin, HIGH);
  delay(500);
  digitalWrite(GreenLEDPin, LOW);
  digitalWrite(RedLEDPin, LOW);
  digitalWrite(YellowLEDPin, LOW);
  digitalWrite(BlueLEDPin, LOW);
  delay(500);
  playSequence();
}
// the loop function runs over and over again forever

void switchOn(int lightNo, int duration){
    switch (lightNo) {
    case 0:
      digitalWrite(GreenLEDPin, HIGH);
      delay(duration);
      digitalWrite(GreenLEDPin, LOW);
      break;
    case 1:
      digitalWrite(RedLEDPin, HIGH);
      delay(duration);
      digitalWrite(RedLEDPin, LOW);
      break;
    case 2:
      digitalWrite(YellowLEDPin, HIGH);
      delay(duration);
      digitalWrite(YellowLEDPin, LOW);
      break;
    case 3:
      digitalWrite(BlueLEDPin, HIGH);
      delay(duration);
      digitalWrite(BlueLEDPin, LOW);
      break;
    default:
      // statements
      break;
    }
}
int detectButtonPress(int duration){
  int returned_val = -1;
  if(digitalRead(GreenInputPin) && returned_val == -1)
    {
      tone(speakerPin, NOTE_A3, duration);
      clkTime = millis();
      switchOnNoWait(0);
      returned_val =  0;
      delay(50);
      
    }
    if(digitalRead(RedInputPin)&& returned_val == -1)
    {
      tone(speakerPin, NOTE_B3, duration);
      clkTime = millis();
      switchOnNoWait(1);
      returned_val = 1;
      delay(50);
    }
    if(digitalRead(YellowInputPin)&& returned_val == -1)
    {
      tone(speakerPin, NOTE_C4, duration);
      clkTime = millis();
      switchOnNoWait(2);
      returned_val =  2;
      delay(50);
    }
    if(digitalRead(BlueInputPin)&& returned_val == -1)
    {
      tone(speakerPin, NOTE_D4, duration);
      clkTime = millis();
      switchOnNoWait(3);
      returned_val = 3;
      delay(50);
    }
    while((digitalRead(BlueInputPin) or digitalRead(YellowInputPin) or digitalRead(RedInputPin) or digitalRead(GreenInputPin)) and returned_val!= -1 ){
      delay(50);
      Serial.println("Stuck");
    }
   

    return returned_val;
}

void gameOver(int correct,int pressed){
  if(correct == -1){
      oled.clear();
      oled.println();
      oled.println("!!Timeout!!");
      for(int i=200;i>100;i--){
      tone(speakerPin, i, i%50);
      digitalWrite(GreenLEDPin, HIGH);
      digitalWrite(RedLEDPin, HIGH);
      digitalWrite(YellowLEDPin, HIGH);
      digitalWrite(BlueLEDPin, HIGH);
      delay(i%50);
      digitalWrite(GreenLEDPin, LOW);
      digitalWrite(RedLEDPin, LOW);
      digitalWrite(YellowLEDPin, LOW);
      digitalWrite(BlueLEDPin, LOW);
      delay(i%50);
    }
  }else{
    for(int j = 0;j<10;j++){
      for(int i=700;i<800;i++){
        tone(speakerPin,i);
        delay(1);
        switchOnNoWait(correct);
      
      }
      for(int i=800;i>700;i--){
        tone(speakerPin,i);
        delay(1);
        switchOnNoWait(pressed);
      }
    } 
  }
  if (level-1 > bestScore){
    EEPROMWritelong(address, level-1);
    bestScore = level-1;
    bestScoreShow();
  }
  makeShow();
}
void switchOnNoWait(int lightNo){
  digitalWrite(GreenLEDPin, LOW);
  digitalWrite(RedLEDPin, LOW);
  digitalWrite(YellowLEDPin, LOW);
  digitalWrite(BlueLEDPin, LOW);
    switch (lightNo) {
    case 0:
      digitalWrite(GreenLEDPin, HIGH);      
      break;
    case 1:
      digitalWrite(RedLEDPin, HIGH);
      break;
    case 2:
      digitalWrite(YellowLEDPin, HIGH);
      break;
    case 3:
      digitalWrite(BlueLEDPin, HIGH);
      break;
    default:
      // statements
      break;
    }
}
void bestScoreShow(){
  oled.clear();
  oled.println("Best Score");
  oled.println();
  oled.println("    "+String(bestScore));
  for(int i =0; i<10 ;i++){
    tone(speakerPin, 1000, 300);
    for(int j=0; j< 4;j++){
        switchOn(j,150);
        if(j>1){
          tone(speakerPin, 800, 300);
        }
    }
    
  }
}
void switchOnWithSound(int lightNo, int duration){
    switch (lightNo) {
    case 0:
      tone(speakerPin, NOTE_A3, duration);
      digitalWrite(GreenLEDPin, HIGH);
      delay(duration);
      digitalWrite(GreenLEDPin, LOW);
      break;
    case 1:
      tone(speakerPin, NOTE_B3, duration);
      digitalWrite(RedLEDPin, HIGH);
      delay(duration);
      digitalWrite(RedLEDPin, LOW);
      break;
    case 2:
      tone(speakerPin, NOTE_C4, duration);
      digitalWrite(YellowLEDPin, HIGH);
      delay(duration);
      digitalWrite(YellowLEDPin, LOW);
      break;
    case 3:
      tone(speakerPin, NOTE_D4, duration);
      digitalWrite(BlueLEDPin, HIGH);
      delay(duration);
      digitalWrite(BlueLEDPin, LOW);
      break;
    default:
      // statements
      break;
    }
}
void playSequence(){
  display_ShowLevel();
  Serial.print("Level:");
  Serial.println(level);
 // delay(450);
  randomSeed(analogRead(0));
  byte randomNumber =   random(4);
  sounds[level]= randomNumber;
  for(int i=0; i<= level; i++){
    Serial.print(sounds[i]);
    Serial.print(",");
    switchOnWithSound(sounds[i], 300);
    delay(300);
  }
  Serial.println("");
  level++;
  
  confirmSequence();
}
void confirmSequence(){
  int i = 0;
  int button = -1;
  clkTime = millis();
  while(i<level){
    button =  detectButtonPress(300);
    if(millis()-clkTime > 30000) { // timeout
      gameOver(-1,-1);
    }
    if(millis()-clkTime > 300) { // timeout
      switchOnNoWait(55);// to switch the lights off
    }
    
    
    if (button != -1){
      Serial.print(button);
      Serial.print(",");
   //   clkTime = millis();
      if(sounds[i]!=button){
        display_Gameover();
        gameOver(sounds[i],button);
      }else{
        i++;
      }
    }
  }
  delay(500);
  switchOnNoWait(55);// to switch the lights off
  Serial.println("");
  display_Correct();
  delay(400);
  playSequence();
}

long EEPROMReadlong(int address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
 
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
 
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

void display_ShowLevel(){
  String l = String(level+1);
  oled.clear();
  oled.println("   Level");
  oled.println();
  oled.println("     "+l);
}
void display_GetReady(){
  oled.clear();
  oled.println("   Get");
  oled.println();
  oled.println("  Ready!!");
}

void display_PressAny(){
  oled.clear();
  oled.println(" Press any");
  oled.println();
  oled.println("  Button!");
}

void display_Correct(){
  oled.clear();
  oled.println();
 
  oled.println(" Correct!");
}

void display_Go(){
  oled.clear();
  oled.println();
 
  oled.println("    Go!");
}
void display_Gameover(){
  oled.clear();
  oled.println("   Game");
  oled.println();
  oled.println("   Over!");
}
void display_BestScore(){
  oled.clear();
  oled.println("Best Score");
  oled.println();
  oled.println("    "+String(bestScore));
}
