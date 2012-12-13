/*

12 leds
-6 to 12 range. starts at 0. kinda.  18 steps
 THIS EXAMPLE MERCILESSLY HACKED TOGETHER FROM: http://www.arduino.cc/en/Tutorial/ShiftOut
 I MOVED bitsToSend OUTSIDE OF registerWrite() SO THAT IT COULD CONTROL MULTIPLE PINS AT ONCE
 */

int latchPin = 8;
int clockPin = 13;
int dataPin = 11;

int xAxis = 0;
int yAxis = 0;
int pot = 0;

int range = 12;              
int threshold = range/4;      
int center = range/2;   

unsigned int bitsToSend = 0; // VERY IMPORTANT THAT THIS IS UNSIGNED. IF NOT, WE CAN'T USE ALL 15 OUTPUTS! 

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(latchPin, LOW);
  xAxis = analogRead(A1);
  xAxis = map(xAxis, 0, 678, 0, range);
  yAxis = analogRead(A3);
  yAxis = map(yAxis, 0, 678, 0, range);
  pot = analogRead(A2);
  pot = map(pot, 0, 1023, 0, 11);

  int xdistance = xAxis - center;
  int ydistance = yAxis - center;
  if (abs(xdistance) < threshold) {
    xdistance = 0;
  }
  if (abs(ydistance) < threshold) {
    ydistance = 0;
  }

  //JOYSTICK LED MAPPING:
  Serial.print ("x: ");
  Serial.print(xdistance);
  Serial.print(" y: ");
  Serial.println(ydistance);
  
    if((ydistance >= 10 && ydistance <= 12) && (xdistance >= -6 && xdistance <= -3)){ //BACK LEFT LED CLOCKWISE ORDER
    registerWrite(12,1);
  }else{
    registerWrite(12,0);
  }
  
   if((xdistance >= 0 && xdistance <= 3) && ydistance == 12){
    registerWrite(13,1);
  }else{
    registerWrite(13,0);
  }
  
    if((xdistance >= 4 && xdistance <= 7) && ydistance == 12){ //bl-0-1-br
    registerWrite(11,1);
  }else{
    registerWrite(11,0);
  }
     if((xdistance >= 8 && xdistance <= 12) && ydistance == 12){ //BACK RIGHT
    registerWrite(10,1);
  }else{
    registerWrite(10,0);
  }
        if((ydistance >= 4 && ydistance <= 12) && xdistance == 12){ //br-1-0-fr
    registerWrite(9,1);
  }else{
    registerWrite(9,0);
  }
          if((ydistance >= -3 && ydistance <= 3) && xdistance == 12){ //br-0-1-fr
    registerWrite(5,1);
  }else{
    registerWrite(5,0);
  }
          if((ydistance >= -6 && ydistance <= -4) && (xdistance >= 7 && xdistance <= 12)){ //FRONT RIGHT
    registerWrite(3,1);
  }else{
    registerWrite(3,0);
  }
           if((xdistance >= 3 && xdistance <= 8) && ydistance == -6){
    registerWrite(0,1);
  }else{
    registerWrite(0,0);
  }
             if((xdistance >= -3 && xdistance < 3) && ydistance == -6){
    registerWrite(7,1);
  }else{
    registerWrite(7,0);
  } 
         /*      if((xdistance >= -6 && xdistance <= -4) && ydistance == -6){ //FRONT LEFT
    registerWrite(7,1);
  }else{
    registerWrite(7,0);
  } */
  
  if((xdistance >= -6 && xdistance <= -4) && ydistance == -6){ //FRONT LEFT
    registerWrite(8,1);
  }else{
    registerWrite(8,0);
  } 
  
                 if((ydistance > -6 && ydistance <= 3) && xdistance == -6){
    registerWrite(15,1);
  }else{
    registerWrite(15,0);
  } 
                if((ydistance >= 4 && ydistance <= 9) && xdistance == -6){
    registerWrite(14,1);
  }else{
    registerWrite(14,0);
  } 
  
  //POT LED MAPPING:
/*
  if (pot == 0){ //FRONT LEFT LED
    registerWrite(7,1);
  }else{
    registerWrite(7,0);
  }
  if (pot == 1){ 
    registerWrite(2,1);
  }else{
    registerWrite(2,0);
  }

  if (pot == 2){ 
    registerWrite(5,1);
  }else{
    registerWrite(5,0);
  }

  if (pot == 3){ //BACK LEFT
    registerWrite(8,1);
  }else{
    registerWrite(8,0);
  }

  if (pot == 4){ 
    registerWrite(9,1);
  }else{
    registerWrite(9,0);
  }

  if (pot == 5){ 
    registerWrite(10,1);
  }else{
    registerWrite(10,0);
  }

  if (pot == 6){ //BACK RIGHT
    registerWrite(11,1);
  }else{
    registerWrite(11,0);
  }

  if (pot == 7){ 
    registerWrite(12,1);
  }else{
    registerWrite(12,0);
  }

  if (pot == 8){ 
    registerWrite(13,1);
  }else{
    registerWrite(13,0);
  }
  
  if (pot == 9){ //FRONT RIGHT
    registerWrite(14,1);
  }else{
    registerWrite(14,0);
  }


  if (pot == 10){ 
    registerWrite(15,1);
  }else{
    registerWrite(15,0);
  }


  if (pot == 11){ 
    registerWrite(0,1);
  }else{
    registerWrite(0,0);
  }
*/
  
  }


void registerWrite(int whichPin, int whichState) {
  digitalWrite(latchPin, LOW); // TURN OFF OUTPUT

  bitWrite(bitsToSend, whichPin, whichState); // SETS THE bitsToSend BITS (00000000). EACH BIT IS AN OUTPUT ON SHIFT REGISTER.

  // AN unsigned int ACTUALLY CONSISTS OF 16 BITS, SO WE NEED TO BREAK IT INTO 2 BYTES (8 BITS EACH)
  byte registerOne = highByte(bitsToSend); // LEFTMOST BITS (********00000000)
  byte registerTwo = lowByte(bitsToSend); // RIGHTMOST BITS (00000000********)

  // SEND THE BYTES OUT APPROPRIATELY
  shiftOut(dataPin, clockPin, MSBFIRST, registerTwo); // OUTPUTS 8-15
  shiftOut(dataPin, clockPin, MSBFIRST, registerOne); // OUTPUTS 0-7

  digitalWrite(latchPin, HIGH); // TURN ON OUTPUT AGAIN



}





