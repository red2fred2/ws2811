#define DIGITAL_PIN 8
#define PORT PORTB
#define PORT_PIN PORTB0

void setup() {
  pinMode(DIGITAL_PIN, OUTPUT);
  digitalWrite(DIGITAL_PIN, 0);
  Serial.begin(9600);
}

byte stripLength = 150;
byte frame[150][3];

byte frames = 2;
byte animation[2][150][3];

void send0(void) {
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

void send1(void) {
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

byte Length = 1;
int Delay = 10;

//red pulse
byte red1[1][3] = {
  {0, 0, 0}
};
byte red2[1][3] = {
  {25, 0, 0}
};
byte red3[1][3] = {
  {50, 0, 0}
};
byte red4[1][3] = {
  {75, 0, 0}
};
byte red5[1][3] = {
  {100, 0, 0}
};

//green pulse

byte green1[1][3] = {
  {0, 0, 0}
};
byte green2[1][3] = {
  {0, 25, 0}
};
byte green3[1][3] = {
  {0, 50, 0}
};
byte green4[1][3] = {
  {0, 75, 0}
};
byte green5[1][3] = {
  {0, 100, 0}
};

//blue pulse
byte blue1[1][3] = {
  {25, 0, 25}
};
byte blue2[1][3] = {
  {25, 0, 0}
};
byte blue3[1][3] = {
  {25, 25, 0}
};
byte blue4[1][3] = {
  {0, 25, 0}
};
byte blue5[1][3] = {
  {0, 0, 25}
};

//r top chaser
byte rgbLength = 5;

byte rc1[5][3] = {
  {25, 0, 0},
  {50, 0, 0},
  {25, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

byte rc2[5][3] = {
  {0, 0, 0},
  {25, 0, 0},
  {50, 0, 0},
  {25, 0, 0},
  {0, 0, 0}
};
byte rc3[5][3] = {
  {0, 0, 0},
  {0, 0, 0},
  {25, 0, 0},
  {50, 0, 0},
  {25, 0, 0}
};
byte rc4[5][3] = {
  {25, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {25, 0, 0},
  {50, 0, 0}
};
byte rc5[5][3] = {
  {50, 0, 0},
  {25, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {25, 0, 0},
  
};

byte left = 60;
byte right = 88;

void loop() {
  
  fillPattern(red1, Length, 0, left);
  fillPattern(rc1, rgbLength, left, right);
  fillPattern(blue1, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red2, Length, 0, left);
  fillPattern(rc2, rgbLength, left, right);
  fillPattern(blue2, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red3, Length, 0, left);
  fillPattern(rc3, rgbLength, left, right);
  fillPattern(blue3, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red4, Length, 0, left);
  fillPattern(rc4, rgbLength, left, right);
  fillPattern(blue4, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red5, Length, 0, left);
  fillPattern(rc5, rgbLength, left, right);
  fillPattern(blue5, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red5, Length, 0, left);
  fillPattern(rc5, rgbLength, left, right);
  fillPattern(blue5, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red4, Length, 0, left);
  fillPattern(rc4, rgbLength, left, right);
  fillPattern(blue4, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red3, Length, 0, left);
  fillPattern(rc3, rgbLength, left, right);
  fillPattern(blue3, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red2, Length, 0, left);
  fillPattern(rc2, rgbLength, left, right);
  fillPattern(blue2, Length, right, 150);
  sendFrame();
  delay(Delay);
  fillPattern(red1, Length, 0, left);
  fillPattern(rc1, rgbLength, left, right);
  fillPattern(blue1, Length, right, 150);
  sendFrame();
  delay(Delay);
}
