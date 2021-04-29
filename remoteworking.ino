#include <SPI.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include <RF24.h>
#include<string>
SSD1306Wire display(0x3c, 4 , 5);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h
#define PIN_CE  2
#define PIN_CSN 0

uint8_t pipeNumber;
uint8_t payloadSize;
const uint64_t pipeNum = 0x7878787878LL;

uint8_t A[180][140] = {0};
int P[2] = {90,70};
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
display.flipScreenVertically();
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
  display.clear();
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

void alarmText()

{
  Serial.println("running alarm text");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "STRAP BREACHED");
  display.display();
delay(1000);
}

/*
void continuity (int row, int col, int row_n, int col_n)

{
  int i = 90;
  int j = 70;
  int m = 70;
  int n = 90;
  if (row > row_n) 
 {
  i = row_n;
  n = row;
 }
  else
 {
  i = row;
  n = row_n;
}
  if (col > col_n) 
 {
  j = col_n;
  m = col;
 }
  else
 {
  j = col;
  m = col_n;
}

  Serial.println("continuity");
  for(int a = i;a<=n;a++)
  for(int b = j;b<=m;b++)
  {
    A[a][b] = 1;
    Serial.println("continuity loop");
  }
}
*/
void rangeText ()

{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "OUT OF RANGE");
    display.display();
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
uint8_t  l= 39+a-b;
uint8_t  m= 27+c-d;
int row_m = map(row,-200,200,-64,64);
int col_m = map(col,-200,200,-32,32);
int row_ar = row_m + 90;
int col_ar = col_m + 70;
A[(row_ar)][(col_ar)] = 1;
//Serial.println("test");
if(row_ar >= 0 && row_ar <=180 && col_ar >= 0 && col_ar <= 140)
{
  
  A[(row_ar)][(col_ar)] = 1;
/*//continuity (P[0], P[1], row_ar, col_ar);
//P[0] = row_ar;
//P[1] = col_ar;
if((row_ar + m) > (153+c-d))
{
c++;
Serial.println("right side");
}
 if((col_ar + l)> (102+a-b))
a++;

if((row_ar + m) < (27+c-d))
d++;

if((col_ar + l) < (39+a-b))
b++;
*/
for(int i=0; i<127;i++)
{
for(int j=0; j<63;j++)
{
 
 if(A[(i+l)][(j+m)] ==1)
 {
  display.setPixel(i, j);
  display.display();
}
}
}
}

else
{
Serial.println("Out of range");

delay(100);
}}


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
if(z == 0)
 drawDisplay(x,y);
if(z ==1)
{
  Serial.println("strap breached runnning");
  
  alarmText();
  for(int i=0;i<5000;i++){
    digitalWrite(10,HIGH);
    delay(.1);
    digitalWrite(10,LOW);
    delay(.1);
    }
}
delay(500);
}  

      
  //delay(1000);
