#define DIGITAL_PIN 8
#define PORT PORTB
#define PORT_PIN PORTB0

byte stripLength = 150;
byte strip[150][3];

//send 0 bit (takes 1 usec plus overhead)
void send0(void) {
  //do AVR assemby
  asm volatile(
    //set pin high
    "sbi %0,%1\n\t"
    //wait 4 cycles (1/4 usec)
    "rjmp .+0\n\trjmp .+0\n\t"
    //set ping high
    "cbi %0,%1\n\t"
    //wait 12 cycles (3/4 usec)
    "rjmp .+0\n\trjmp .+0\n\trjmp .+0\n\trjmp .+0\n\trjmp .+0\n\trjmp .+0\n\t"
    
    ::
    "I" (_SFR_IO_ADDR(PORT)),
    "I" (PORT_PIN)
  );
}

//send 1 bit (1 usec)
void send1(void) {
  //do AVR assemby
  asm volatile(
    //set pin high
    "sbi %0,%1\n\t"
    //wait 11 cycles (11/16 usec)
    "rjmp .+0\n\trjmp .+0\n\trjmp .+0\n\trjmp .+0\n\trjmp .+0\n\tnop\n\t"
    //set pin low
    "cbi %0,%1\n\t"
    //wait 5 cycles (5/16 usec)
    "rjmp .+0\n\trjmp .+0\n\tnop\n\t"
    
    ::
    "I" (_SFR_IO_ADDR(PORT)),
    "I" (PORT_PIN)
  );
}

//send a byte (8 usec)
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

//send an RGB color to a light (24 usec)
void sendRGB(byte r, byte g, byte b) {
  sendByte(g);
  sendByte(r);
  sendByte(b);
}

//send information for one frame of an animation (150 x 24 + 50 usec)
void sendFrame(byte values[][3], byte length) {
  for(int i = 0; i < length; i++) {
    sendRGB(values[i][0], values[i][1], values[i][2]);
  }
  
  //delay 50 usec
  unsigned long start = millis();
  unsigned long time;
  do time = millis() - start;
  while(time < 50);
}

//repeat a pattern to fill the strip[150][3] variable
void fillPattern(byte pattern[][3], byte length, int from, int to) {
  for(byte i = from; i < to; i++) {

    int position = i % length - from % length;
    if(position < 0) position += length;
    strip[i][0] = pattern[position][0];
    strip[i][1] = pattern[position][1];
    strip[i][2] = pattern[position][2];
  }
}

void setup() {
  pinMode(DIGITAL_PIN, OUTPUT);
  digitalWrite(DIGITAL_PIN, 0);
  Serial.begin(9600);
}

byte patternLength = 3;
byte pattern[3][3] = {
 {20, 0, 0},
 {20, 20, 20},
 {0, 0, 20}
};

byte mericaLength = 4;
byte mericaDelay = 0;

byte merica1[4][3] = {
  {20, 0, 0},
  {0, 20, 0},
  {0, 0, 20},
  {0, 0, 0}
};
byte merica2[4][3] = {
  {0, 0, 20},
  {0, 0, 0},
  {20, 0, 0},
  {0, 20, 0}
};
byte merica3[4][3] = {
  {0, 20, 0},
  {0, 0, 20},
  {0, 0, 0},
  {20, 0, 0}
};

void loop() {
  fillPattern(merica1, mericaLength, 0, 75);
  fillPattern(merica3, mericaLength, 75, 150);
  sendFrame(strip, stripLength);
  delay(mericaDelay);
  fillPattern(merica2, mericaLength, 0, 75);
  fillPattern(merica2, mericaLength, 75, 150);
  sendFrame(strip, stripLength);
  delay(mericaDelay);
  fillPattern(merica3, mericaLength, 0, 75);
  fillPattern(merica1, mericaLength, 75, 150);
  sendFrame(strip, stripLength);
  delay(mericaDelay);
}
