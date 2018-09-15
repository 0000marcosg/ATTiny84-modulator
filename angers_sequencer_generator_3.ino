byte jj, ff, gg, latch1;
uint32_t ct, pt0, pt1, pt2, pt3, pulse_time;
int16_t pot0;
byte pulse1;
//Const dosen't seem to be working on the attiny? It's still using flash
const byte logtable[257] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 14, 14, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 21, 22, 23, 23, 24, 25, 26, 26, 27, 28, 28, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40, 41, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 102, 103, 104, 105, 106, 107, 108, 109, 110, 112, 113, 114, 115, 116, 117, 119, 120, 121, 122, 123, 124, 126, 127, 128, 129, 130, 132, 133, 134, 135, 136, 138, 139, 140, 141, 142, 144, 145, 146, 147, 149, 150, 151, 152, 154, 155, 156, 158, 159, 160, 161, 163, 164, 165, 167, 168, 169, 171, 172, 173, 174, 176, 177, 178, 180, 181, 182, 184, 185, 187, 188, 189, 191, 192, 193, 195, 196, 197, 199, 200, 202, 203, 204, 206, 207, 209, 210, 211, 213, 214, 216, 217, 218, 220, 221, 223, 224, 226, 227, 228, 230, 231, 233, 234, 236, 237, 239, 240, 242, 243, 245, 246, 248, 249, 251, 252, 254, 255, 255};

byte index1;
int  index2, i2d;

void setup() {
  pinMode(1, OUTPUT);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, INPUT_PULLUP);

  TCCR0B = TCCR0B & B11111000 | B00000001;
  /*
     WGM10, WGM12: Fast PWM, 8-bit TOP=255
     CS10: No prescaler
     COM1A1: Pin 6 to LOW on match with OCR1A and to HIGH on TOP
  */
  TCCR1A = _BV(COM1A1) | _BV(WGM10);
  TCCR1B = _BV(CS10) | _BV(WGM12);
  /*
     50% duty cycle
     32 kHz with 8MHz CPU clock
  */
  OCR1A = 127;

}


void loop() {
  ct = millis();

  pot0 = logtable[(analogRead(0) >> 2)] << 3 ;

  //ds = digitalRead(10); use digital pins to change rate in future versions?


  if (ct - pt0 > pot0 << 5) {
    latch1 = !latch1;
    digitalWrite(3, latch1);
    pt0 = ct;
    jj++;
    pt0 = ct;
    analogWrite(5, (rng() >> 10) << 3);
    byte rh1 = 0;
    byte r1 = (rng() >> 14);

    if (r1 == 0) {
      rh1 = 127;
    }
    if (r1 == 1) {
      rh1 = 192;
    }
    if (r1 == 2) {
      rh1 = 254;
    }
    analogWrite(8, rh1);
    pulse1 = bitRead(rng(), 4);
    pulse_time = ct;
    digitalWrite(4, pulse1);

  }

  if (pulse1 == 1) {

    if (ct - pulse_time > 1000) {
      digitalWrite(4, 0);
      pulse1 = 0;
    }
  }

  if (ct - pt2 > (pot0 + 100) << 4) {
    pt2 = ct;


  }

  if (ct - pt1 > pot0) {
    pt1 = ct;

    index1++;
    analogWrite(6, logtable[index1]);


    if (i2d == 1) {
      index2 += 3;
    }
    else {
      index2 -= 3;
    }
    if (index2 > 255) {
      i2d = 0;
      index2 = 255;
    }
    if (index2 < 1) {
      i2d = 1;
      index2 = 1;
    }

    analogWrite(7, logtable[index2]);

  }

}

// arduino's random() is ver slow. This is very fast and works enough for our purposes 
unsigned int rng() {
  static unsigned int y = 0;
  y += micros(); // seeded with changing number
  y ^= y << 2; y ^= y >> 7; y ^= y << 7;
  return (y);
}
