#include <gfxfont.h>
#include <glcdfont.h>
#include <Matrix.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

const int buttonRightPin = 9;
const int buttonLeftPin  = 7;

const int RIGHTBUTTON = 0;
const int LEFTBUTTON  = 1;

const int TOP    = 0;
const int RIGHT  = 1;
const int BOTTOM = 2;
const int LEFT   = 3;
const int MAX_SNAKE_LENGTH = 10;

Matrix matrix1(20,21);
Matrix matrix2(18,19);
Matrix matrix3(16,17);
Matrix matrix4(14,15);

int direction = TOP;                               
int snakeX[MAX_SNAKE_LENGTH];                     
int snakeY[MAX_SNAKE_LENGTH];                      
int snakeLength = 1;                               
boolean buttonRead = false;                        
unsigned long prevTime = 0;                        
unsigned long delayTime = 500;                     

int fruitX, fruitY;
unsigned long fruitPrevTime = 0;
unsigned long fruitBlinkTime = 1000/250;
int fruitLed = LED_ON;

void setup(){
  Serial.begin(9600);
  Serial.println("Snake is started");
  randomSeed(analogRead(0));
  // Init led matrix
  matrix1.begin(0x70);
  matrix2.begin(0x70);
  matrix3.begin(0x70);
  matrix4.begin(0x70);
  // init buttons
  int buttonpins[] = {buttonRightPin, buttonLeftPin};
  initButtons(buttonpins, 2);
  // init snake
  snakeX[0] = 4;
  snakeY[0] = 7;
  for(int i=1; i<MAX_SNAKE_LENGTH; i++){
    snakeX[i] = snakeY[i] = -1;
  }
  makeFruit();
}

void loop(){
  checkButtons();
  unsigned long currentTime = millis();
  if(currentTime - prevTime >= delayTime){
    nextstep(); 
    buttonRead = false;
    prevTime = currentTime;
  }
  draw();
}

void checkButtons(){
  if(!buttonRead){
    int currentDirection = direction;
    if(buttonClicked(LEFTBUTTON)){
      direction--;
      if(direction < 0){
        direction = LEFT;
      }
    }
    else if(buttonClicked(RIGHTBUTTON)){
      direction++;
      if(direction > 3){
        direction = TOP;
      }
    }
    buttonRead = (currentDirection != direction);
  }
}

void draw(){
  matrix1.clear();
  matrix2.clear();
  matrix3.clear();
  matrix4.clear();
  drawSnake();
  drawFruit();
  matrix1.writeDisplay();
  matrix2.writeDisplay();
  matrix3.writeDisplay();
  matrix4.writeDisplay();
}

void drawSnake(){
  for(int i=0; i<snakeLength; i++){
        if (snakeX[i] >= 0 && snakeX[i] < 8) {
    if (snakeY[i] >= 0 && snakeY[i] < 8){
      snakeX[i] = snakeX[i]-1;
      matrix1.drawPixel(snakeX[i], snakeY[i], LED_ON);   
      matrix1.writeDisplay();
      snakeX[i] = snakeX[i]+1;
      }
    else if (snakeY[i] > 7 && snakeY[i] < 15){
      snakeX[i] = snakeX[i]-1;
      snakeY[i]= snakeY[i]-8;
      matrix2.drawPixel(snakeX[i], snakeY[i], LED_ON);   
      matrix2.writeDisplay();
      snakeX[i] = snakeX[i]+1;
      snakeY[i] = snakeY[i]+8;
      }
    }
  else if (snakeX[i] > 7 && snakeX[i] < 15) {
    if (snakeY[i] >= 0 && snakeY[i] < 8){
      snakeX[i] = snakeX[i]-1;
      matrix3.drawPixel(snakeX[i], snakeY[i], LED_ON);   
      matrix3.writeDisplay();
      snakeX[i] = snakeX[i]+1;
      }
    else if (snakeY[i] > 7 && snakeY[i] < 15){
      snakeX[i] = snakeX[i]-9;
      snakeY[i] = snakeY[i]-8;
      matrix4.drawPixel(snakeX[i], snakeY[i], LED_ON);   
      matrix4.writeDisplay();
      snakeX[i] = snakeX[i]+9;
      snakeY[i] = snakeY[i]+8;
      }
    } 
  }
}

void drawFruit(){
  if(inPlayField(fruitX, fruitY)){
    unsigned long currenttime = millis();
    if(currenttime - fruitPrevTime >= fruitBlinkTime){
      fruitLed = (fruitLed == LED_ON) ? LED_OFF : LED_ON;
      fruitPrevTime = currenttime;
    }
    if (fruitX >= 0 && fruitX < 8) {
    if (fruitY >= 0 && fruitY < 8){
      fruitX = fruitX-1;
      
      matrix1.clear();
      matrix1.drawPixel(fruitX, fruitY, fruitLed);   
      matrix1.writeDisplay();
      fruitX = fruitX+1;
      }
    else if (fruitY > 7 && fruitY < 15){
      fruitX = fruitX-1;
      fruitY= fruitY-8;
      matrix2.clear();
      matrix2.drawPixel(fruitX, fruitY, fruitLed);   
      matrix2.writeDisplay();
      fruitX = fruitX+1;
      fruitY = fruitY+8;
      }
    }
  else if (fruitX > 7 && fruitX < 15) {
    if (fruitY >= 0 && fruitY < 8){
      fruitX = fruitX-1;
      matrix3.clear();
      matrix3.drawPixel(fruitX, fruitY, fruitLed);   
      matrix3.writeDisplay();
      fruitX = fruitX+1;
      }
    else if (fruitY > 7 && fruitY < 15){
      fruitX = fruitX-9;
      fruitY = fruitY-8;
      matrix4.clear();
      matrix4.drawPixel(fruitX, fruitY, fruitLed);   
      matrix4.writeDisplay();
      fruitX = fruitX+9;
      fruitY = fruitY+8;
      }
    } 
  }
}

boolean inPlayField(int x, int y){
  return (x>=0) && (x<15) && (y>=0) && (y<15);
}

void nextstep(){
  for(int i=snakeLength-1; i>0; i--){
    snakeX[i] = snakeX[i-1];
    snakeY[i] = snakeY[i-1];
  }
  switch(direction){
    case TOP:
      snakeY[0] = snakeY[0]-1;
      break;
    case RIGHT:
      snakeX[0] = snakeX[0]+1;
      break;
    case BOTTOM:
      snakeY[0] = snakeY[0]+1;
      break;
    case LEFT:
      snakeX[0]=snakeX[0]-1;
      break;
  }
  if((snakeX[0] == fruitX) && (snakeY[0] == fruitY)){
    snakeLength++;
    if(snakeLength < MAX_SNAKE_LENGTH){      
      makeFruit();
    } 
    else {
      fruitX = fruitY = -1;
    }
  }
}

void makeFruit(){
  int x, y;
  x = random(0, 15);
  y = random(0, 15);
  while(isPartOfSnake(x, y)){
    x = random(0, 15);
    y = random(0, 15);
  }
  fruitX = x;
  fruitY = y;
}

boolean isPartOfSnake(int x, int y){
  for(int i=0; i<snakeLength-1; i++){
    if((x == snakeX[i]) && (y == snakeY[i])){
      return true;
    }
  }
  return false;
}


int byting(int i, int j) {
  if (i >= 0 && i < 8) {
    if (j >= 0 && j < 8){
      i = i-1;
      matrix1.drawPixel(i, j, 1);   
      matrix1.writeDisplay();
      i = i+1;
      }
    else if (j > 7 && j < 15){
      i = i-1;
      j = j-8;
      matrix2.drawPixel(i, j, 1);   
      matrix2.writeDisplay();
      i = i+1;
      j = j+8;
      }
    }
  else if (i > 7 && i < 15) {
    if (j >= 0 && j < 8){
      i = i-1;
      matrix3.drawPixel(i, j, 1);   
      matrix3.writeDisplay();
      i = i+1;
      }
    else if (j > 7 && j < 15){
      i = i-9;
      j = j-8;
      matrix4.drawPixel(i, j, 1);   
      matrix4.writeDisplay();
      i = i+9;
      j = j+8;
      }
    } 
}