#include <SPI.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include <RF24.h>
#include<string>
SSD1306Wire display(0x3c, 4 , 5);   // ADDRESS, SDA, SCL  -  SDA and SCL in the ESP8266 is connected to GPIO4 (Pin D2) and GPIO5 (Pin D1) pins.
#define PIN_CE  2 // NRF24l01's Chip enable is connected to GPIO2 pin (Pin D4).
#define PIN_CSN 0 // NRF24l01's Chip Selection pin is connected to GPIO0 pin (Pin D3).
#define row_size 180
#define col_size 140
#define screen_row 127
#define screen_col 63
const uint64_t pipeNum = 0x7878787878LL; //

uint8_t A[row_size][col_size] = {0}; // The array to store all the co-ordinates received from the strap module.

struct screenmove // The array A can store upto 180 x and 140 y coordinates. But the OLED display can only show 128 x and 64 y co-ordinates.
            // So to show the x and y co-ordinates which exceed the screen limit, we are moving the display to the latest co-ordinates in A.
            // In this struct values are stored to move the screen according to the latest co-ordinates received in array A.
{
  uint8_t r = 0, // Value to store the right shift in Screen display
  l = 0, // Value to store the left shift in Screen display
  d = 0, // Value to store the down shift in Screen display
  u = 0; // Value to store the top shift in Screen display
}sm;

struct data // The data received from strap module and split into 4 parts
{
  String a, // The first part of the received data which represents the movement
  b, //The second part of the received data which represents the x co-ordinate
  c, // The third part of the received data which represents the y co-ordinate
  d; // The fourth part of the received data which represents the strap condition
}dt;

bool button; //To store the push button datas

RF24 radio(PIN_CE, PIN_CSN); // Initializing the RF24 radio library.

void setup() // Function in which Radio and Display are setup
{

  Serial.begin(115200); // Starting the serial communication with 115200 baudrate
  radio.begin(); // To set parameters for the radio communication
  radio.setChannel(5); // Selecting the freqeuncy for communication
  radio.setDataRate (RF24_1MBPS); // Data rate is set to 1 MBPS
  radio.setPALevel(RF24_PA_HIGH); // Setting the power amplifier (dB) as High
  radio.enableDynamicPayloads(); // Enable dynamically-sized payloads. 
  radio.setRetries(15, 15); // Set the number and delay of retries upon failed submit. Here number and delay are 15
   radio.enableAckPayload(); // Enable custom payloads on the acknowledge packets. 

  radio.openWritingPipe(pipeNum); // Open a pipe for writing. 
  

  display.init(); // Initializing the display
  display.flipScreenVertically(); // Flipping the screen
  display.setPixel(80, 32); // Draw a single pixel in white at 
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display(); // Show initial display buffer contents on the screen
  delay(2000); // Delay for 2 seconds
  display.clear(); // Clear the buffer
  }


char text[1]; // Enable and disable instructions for sending to strap module.
char ackData[24]; // The data received from strap module

String Radio() // Function for sending and receiving data through NRF24L01
{

  String FIFO;
 switch_state(); // Calling switch_state function.
  if(button == 0) 
  {
    text[0] = '1';
    
    Serial.println("text 1");
  }
  else if(button == 1)
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
      //read data from the buffer into the ackData array indicating how many bytes can fit into the array 
      radio.read(&ackData, sizeof(ackData));
  
      for (uint8_t i = 0; i < sizeof(ackData); i++) {
       FIFO += ackData[i];
      }
      Serial.println(ackData);
      
      // Если все три буфера FIFO заполнены то очищаем
      if (radio.rxFifoFull()) {
        Serial.println("Buffer full");
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

void rangeText ()

{
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "OUT OF RANGE");
    display.display();
}

void switch_state ()

{

  if(digitalRead(15))
  {
    button =1;  //switch on
    Serial.println("Pin D8");
  }
  if(digitalRead(16))
  {
    button = 0;  //switch off
    Serial.println("Pin D0");
  }
  
}  


void drawDisplay(int row, int col) {
      // Draw a pixel at given position
        

int row_m = map(row,-1000,1000,-200,200); //mapping
int col_m = map(col,-1000,1000,-200,200);
int row_ar = row_m + (row_size/2); // To put the x = 0 at at the middle of the array A.
int col_ar = col_m + (col_size/2); // To put the y = 0 at at the middle of the array A.
int l = ((row_size/2)-(screen_row/2))+sm.d-sm.u; // To get the x = 0 at the middle of the screen
int m = ((col_size/2)-(screen_col/2))+sm.r-sm.l; // To get the y = 0 at the middle of the screen
if(row_ar >= 0 && row_ar <=row_size && col_ar >= 0 && col_ar <= col_size) // If the mapped x and y co-ordinate doesnot exceed the limit
{
  A[(row_ar)][(col_ar)] = 1; // The received x, y co-ordinates is plotted in array A.
  
  if((row_ar) > ((row_size/2)+(screen_row/2)+sm.d-sm.u)) // Checking if the mapped x co-ordinate is at down below of the screen
{
sm.r++;
}
 if((col_ar) > ((col_size/2)+(screen_col/2)+sm.r-sm.l))

{
  sm.r++;
}

if((row_ar) < ((row_size/2)-(screen_row/2)+sm.d-sm.u))
{
  sm.u++;
}

if((col_ar) < ((col_size/2)-(screen_col/2)+sm.r-sm.l))
{
  sm.l++;
}

for(int i=0; i < screen_row;i++)
{
for(int j=0; j< screen_col;j++)
{
 
 if(A[(i+m)][(j+l)] ==1)
 {
  display.setPixel(i, j);
  display.display();
}
}
}
display.clear();
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
