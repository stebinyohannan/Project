#include <SPI.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include <RF24.h>
#include<string>
SSD1306Wire display(0x3c, 4 , 5);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h
#define PIN_CE  4
#define PIN_CSN 2

uint8_t pipeNumber;
uint8_t payloadSize;
const uint64_t pipeNum = 0x7878787878LL;

uint8_t A[160][100] = {0};
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

void setup() {

  Serial.begin(115200);
  radio.begin();
  radio.setChannel(5);
  radio.setDataRate (RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.enableDynamicPayloads();
  //radio.setAutoAck(true);
  radio.setRetries(15, 15);
   radio.enableAckPayload();

  radio.openWritingPipe(pipeNum);
  

  display.init();

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clear();

  // Draw a single pixel in white
  display.setPixel(80, 32);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  }


char text[1];
char ackData[24];

String Radio()
{

  String FIFO;
 switch_state();
  if(sw.dev_en == 0)
  {
    text[0] = '1';
    
    Serial.println("text 1");
  }
  else if(sw.dev_en == 1)
  {
    text[0] = '2';
    Serial.println("text 2");
  }
  if (radio.write(&text, sizeof(text))) {
    Serial.println("Delivered " + String(sizeof(text)) + " byte");
  }
  else {
    Serial.println("Data not delivered");
  }
  if (radio.isAckPayloadAvailable()) {
      Serial.println("ack\n");
      // читаем данные из буфера в массив ackData указывая сколько всего байт может поместиться в массив
      radio.read(&ackData, sizeof(ackData));
  
      for (uint8_t i = 0; i < sizeof(ackData); i++) {
       FIFO += ackData[i];
      }
      Serial.println(ackData);
      
      // Если все три буфера FIFO заполнены то очищаем
      if (radio.rxFifoFull()) {
        //cout << "Buffer full" << endl;
        radio.flush_rx(); 
      }
}
return(FIFO);
}

void switch_state ()

{

  if(digitalRead(15))
  {
    sw.dev_en =1;  //switch on
    Serial.println("Pin D8");
  }
  if(digitalRead(16))
  {
    sw.dev_en = 0;  //switch off
    Serial.println("Pin D0");
  }
  
}  


void drawDisplay(int row, int col) {
      // Draw a pixel at given position
        

uint8_t  a= 0; //left side
uint8_t  b= 0; // right side
uint8_t  c= 0; //top side
uint8_t  d= 0; // down side
uint8_t  l= 16+a-b;
uint8_t  m= 18+c-d;
A[(row)][(col)] = 1;
Serial.println("test");
for(int i=0; i<127;i++)
{
for(int j=0; j<63;j++)
{
 if(A[(i)][(j)] ==1)
 {
  display.setPixel(i, j);
  display.display();
}
}

}
delay(100);
}


void loop() {

  int16_t w;
  int16_t x;
  int16_t y;
  int16_t z;
  int8_t q = 0;
int8_t j = 0; 
String check = "";
char  text[] = "0";
  check =Radio();
  dt.a="";
  dt.b="";
  dt.c="";
  dt.d="";
  for(int8_t i= 0; check[i] != 0; i++)
{
  if( check[i] == ' ')
  {
    
    q++;
  }
  if(q == 0)
  {
    dt.a += check[i];
    j++;
    }
    
      if(q == 1)
  {

    dt.b += check[i];
    }
         if(q == 2)
  {
    dt.c += check[i];
    }
         if(q == 3)
  {
    dt.d += check[i];
    }
}
 
w = std::atoi(dt.a.c_str());
x = std::atoi(dt.b.c_str());
y = std::atoi(dt.c.c_str());
z = std::atoi(dt.d.c_str());
 Serial.println(w);
 Serial.println(x);
 Serial.println(y);
 Serial.println(z);
 drawDisplay(x,y);
delay(500);

}  

      
  //delay(1000);
