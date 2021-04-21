
 
// Include the correct display library

// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
//#include <nRF24L01.h>
#include <RF24.h>


#define PIN_CE  9
#define PIN_CSN 10

uint8_t pipeNumber;
uint8_t payloadSize;
struct data
{
  String a,b,c,d;
}dt;

struct switchs
{
  bool dev_en;
  bool disp;
}sw;
RF24 radio(PIN_CE, PIN_CSN);
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h

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




void drawDisplay(int16_t row, int16_t col) {
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

void drawtext() {
  display.clear();

  display.println(F("Strap broken!"));
  display.display();
  delay(2000);
}


void switc() 
{
  if(digitalRead(2))sw.dev_en =1;  //switch on
  if(digitalRead(3))sw.dev_en = 0;  //switch off
  if(digitalRead(4))sw.disp = 1;;  // display clear
  
}


void loop() {
  int16_t w;
  int16_t x;
  int16_t y;
  int16_t z;
  int8_t q = 0;
int8_t j = 0;
  String FIFO;
  char sep = {' '};
    while (true)
{
 switc();
if (sw.dev_en == HIGH)
{
char text[] = "OFF";
}
else
{
char  text[] = "ON";
}
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
         if(q == 3)
  {
    dt.d[j] = FIFO[i];
    }
}
w = dt.a.toInt();
x = dt.b.toInt();
y = dt.c.toInt();
z = dt.d.toInt();
  display.clear();
if(w = HIGH){
  drawDisplay(x,y);
}
if(z = HIGH)
{
  drawtext();
}
  delay(10);
}
