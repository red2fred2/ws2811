#define DIGITAL_PIN 8
#define PORT PORTB
#define PORT_PIN PORTB0

byte patternLength = 1;
byte pattern[1][3] = {
 {20,20,20}
};

byte stripLength = 150;
byte strip[150][3];

void send0(void) {
  asm volatile(
    "sbi  %0, %1\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "cbi   %0, %1\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    ::
    "I" (_SFR_IO_ADDR(PORT)),
    "I" (PORT_PIN)
  );
}

void send1(void) {
  asm volatile(
    "sbi  %0, %1\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "nop\n\t"
    "cbi   %0, %1\n\t"
    "rjmp .+0\n\t"
    "rjmp .+0\n\t"
    "nop\n\t"
    ::
    "I" (_SFR_IO_ADDR(PORT)),
    "I" (PORT_PIN)
  );
}

void sendByte(byte value) {
  cli();
  for(int i = 7; i > -1; i--) {
    if(bitRead(value, i)) {
      send1();
    } else send0();
  }
  sei();
}

void sendRGB(byte r, byte g, byte b) {
  sendByte(g);
  sendByte(r);
  sendByte(b);
}

void fillPattern(byte pattern[][3], byte length) {
  for(byte i = 0; i < stripLength; i++) {
    strip[i][0] = pattern[i % length][0];
    strip[i][1] = pattern[i % length][1];
    strip[i][2] = pattern[i % length][2];
  }
}

void sendFrame(byte values[][3], byte length) {

  for(int i = 0; i < length; i++) {
    sendRGB(values[i][0], values[i][1], values[i][2]);
  }
  
  delay(1);
}

void setup() {
  pinMode(DIGITAL_PIN, OUTPUT);
  digitalWrite(DIGITAL_PIN, 0);
  Serial.begin(9600);
}

void loop() {
  fillPattern(pattern, patternLength);
  sendFrame(strip, stripLength);
}
