#define DIGITAL_PIN 8
#define PORT PORTB
#define PORT_PIN PORTB0
#include <Wire.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//delay
const int d   = 30;
//bottom left
const byte bl = 0;
//top left
const byte tl = 60;
//middle
const byte m  = 74;
//top right
const byte tr = 88;
//bottom right
const byte br = 150;
//alliance
const byte a  = 0;
//elevator direction
const byte e  = 2;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//pulse
const byte pl = 1;
const byte pulse[3][5][1][3] = {
  {
    {{2,   0, 0}},
    {{12,  0, 0}},
    {{25,  0, 0}},
    {{100, 0, 0}},
    {{250, 0, 0}}
  }, {
    {{0, 0, 2}},
    {{0, 0, 12}},
    {{0, 0, 25}},
    {{0, 0, 100}},
    {{0, 0, 250}}
  }, {
    {{0, 2,   0}},
    {{0, 12,  0}},
    {{0, 25,  0}},
    {{0, 100, 0}},
    {{0, 250, 0}}
  }
};

//chaser
const byte cl = 5;
const byte chaser[2][5][5][3] = {
  {
    {{25,  0, 0}, {250, 0, 0}, {25,  0, 0}, {2,   0, 0}, {2,   0, 0}},
    {{2,   0, 0}, {25,  0, 0}, {250, 0, 0}, {25,  0, 0}, {2,   0, 0}},
    {{2,   0, 0}, {25,  0, 0}, {25, 0, 0}, {250,  0, 0}, {2,   0, 0}},
    {{25,  0, 0}, {2,   0, 0}, {2,   0, 0}, {25,  0, 0}, {250, 0, 0}},
    {{250, 0, 0}, {25,  0, 0}, {2,   0, 0}, {2,   0, 0}, {25,  0, 0}}
  }, {
    {{0, 0, 25},  {0, 0, 250}, {0, 0, 25},  {0, 0, 2},   {0, 0, 2}},
    {{0, 0, 2},   {0, 0, 25},  {0, 0, 250}, {0, 0, 25},  {0, 0, 2}},
    {{0, 0, 2},   {0, 0, 2},   {0, 0, 25},  {0, 0, 250}, {0, 0, 25}},
    {{0, 0, 25},  {0, 0, 2},   {0, 0, 2},   {0, 0, 25},  {0, 0, 250}},
    {{0, 0, 250}, {0, 0, 25},  {0, 0, 2},   {0, 0, 2},   {0, 0, 25}}
  }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(DIGITAL_PIN, OUTPUT);
  digitalWrite(DIGITAL_PIN, 0);
  Serial.begin(9600);
}

const byte stripLength = 150;
byte frame[150][3];

void send0(void) {
  asm volatile(
    //set pin high
    "sbi %0, %1 \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    //set pin low
    "cbi %0, %1 \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    ::
    "I" (_SFR_IO_ADDR(PORT)),
    "I" (PORT_PIN)
  );
}

void send1(void) {
  asm volatile(
    //set pin high
    "sbi %0, %1 \n"
    //wait 11 cycles (11/16 usec)
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "nop        \n"
    //set pin low
    "cbi %0, %1\n"
    //wait 5 cycles (5/16 usec)
    "rjmp .+0   \n"
    "rjmp .+0   \n"
    "nop        \n"
    ::
    "I" (_SFR_IO_ADDR(PORT)),
    "I" (PORT_PIN)
  );
}

void sendByte(byte value) {
  //disable cpu interrupts
  cli();
  
  for(int i = 7; i > -1; i--) {
    if(bitRead(value, i)) send1();
    else send0();
  }
  
  //enable cpu interrupts
  sei();
}

void sendFrame() {
  for(int i = 0; i < stripLength; i++) {
    sendByte(frame[i][1]);
    sendByte(frame[i][0]);
    sendByte(frame[i][2]);
  }
  
  //delay 50 usec
  unsigned long start = millis();
  unsigned long time;
  do time = millis() - start;
  while(time < 50);
}

void fillPattern(byte pattern[][3], byte length, int from, int to) {
  for(byte i = from; i < to; i++) {

    int position = i % length - from % length;
    if(position < 0) position += length;
    frame[i][0] = pattern[position][0];
    frame[i][1] = pattern[position][1];
    frame[i][2] = pattern[position][2];
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  for(byte i = 0; i < 5; i++) {

    fillPattern(pulse[a][i], pl, tl, tr);
    
    switch(e) {
      case 1:  //down
        fillPattern(chaser[a][4-i], cl, bl, tl);
        fillPattern(chaser[a][i], cl, tr, br);
      break;
      case 2:  //up
        fillPattern(chaser[a][i], cl, bl, tl);
        fillPattern(chaser[a][4-i], cl, tr, br);
      break;
      default: //not moving
        fillPattern(pulse[a][i], pl, bl, br);
    }
    
    sendFrame();
    delay(d);
  }
  
}

