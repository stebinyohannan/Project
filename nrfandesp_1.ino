
 
// Include the correct display library

// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
//#include <nRF24L01.h>
#include <RF24.h>


#define PIN_CE  0
#define PIN_CSN 2
#define Row 125
uint8_t pipeNumber;
uint8_t payloadSize;
struct data
{
  String a,b,c;
}dt;

RF24 radio(PIN_CE, PIN_CSN);
// OR #include "SH1106Wire.h"   // legacy: #include "SH1106.h"

// For a connection via I2C using brzo_i2c (must be installed) include:
// #include <brzo_i2c.h>        // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// OR #include "SH1106Brzo.h"

// For a connection via SPI include:
// #include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// OR #include "SH1106SPi.h"


// Optionally include custom images

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h
// SSD1306Wire display(0x3c, D3, D5);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.
// SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);  // ADDRESS, SDA, SCL, OLEDDISPLAY_GEOMETRY  -  Extra param required for 128x32 displays.
// SH1106 display(0x3c, SDA, SCL);     // ADDRESS, SDA, SCL

// Initialize the OLED display using brzo_i2c:
// SSD1306Brzo display(0x3c, D3, D5);  // ADDRESS, SDA, SCL
// or
// SH1106Brzo display(0x3c, D3, D5);   // ADDRESS, SDA, SCL

// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi display(D0, D2, D8);  // RES, DC, CS
// or
// SH1106Spi display(D0, D2);       // RES, DC

uint8_t A[160][100] = {0};
#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;
char text[] = "Hello world";
  char ackData[24];
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
{
const uint64_t pipeNum = 0x7878787878LL;
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(5);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.enableDynamicPayloads();

  radio.openWritingPipe(pipeNum);
  
  radio.printDetails();

  radio.startListening();

  Serial.println("Start listening...");

}

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}

void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hello world");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "Hello world");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "Hello world");
}


void drawTextAlignmentDemo() {
    // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

void drawRectDemo(int16_t row, int16_t col) {
      // Draw a pixel at given position
        

uint8_t  a= 0; //left side
uint8_t  b= 0; // right side
uint8_t  c= 0; //top side
uint8_t  d= 0; // down side
uint8_t  l= 16+a-b;
uint8_t  m= 18+c-d;
A[(80+row)][(50+col)] = 1;
for(int i=0; i<127;i++)
{
for(int j=0; j<63;j++)
{
 if(A[(l)][(m)] ==1)
 {
  display.setPixel(i, j);
}}}
  
}


void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}


void loop() {
  int16_t x;
  int16_t y;
  String FIFO;
  char sep = {' '};
    while (true)
{
 

  if (radio.write(&text, sizeof(text))) {
    Serial.println("Delivered " + String(sizeof(text)) + " byte");
  }
  else {
    Serial.println("Data not delivered");
  }
    if (radio.isAckPayloadAvailable()) {
        radio.read(&ackData, sizeof(ackData));
 
      for (uint8_t i = 0; i < sizeof(ackData); i++) {
        FIFO += ackData[i];
      }
      Serial.println(FIFO);
      if (radio.rxFifoFull()) {
          Serial.println("Buffer full");
        radio.flush_rx();
      }
    }
  delay(1000);

}  
int8_t q = 0;
int8_t j = 0;
// Get next command from Serial (add 1 for final 0)
for(int8_t i= 0; FIFO[i] != 0; i++)
{
  if( FIFO[i] == ' ')
  {
    j=0;
    q++;
  }
  if(q == 0)
  {
    dt.a[j] = FIFO[i];
    }
    
      if(q == 1)
  {
    dt.b[j] = FIFO[i];
    }
         if(q == 2)
  {
    dt.c[j] = FIFO[i];
    }
}

x = dt.b.toInt();
y = dt.c.toInt();
  
  // clear the display
  display.clear();
  // draw the current demo method
  //demos[demoMode]();
  drawRectDemo(x,y);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));
  // write the buffer to the display
  display.display();
/*
  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  }*/
  counter++;
  delay(10);
}
