//#include <nRF24L01.h>
//#include <RF24.h>
#include <RF24.h>
#include<string>

#define PIN_CE  4
#define PIN_CSN 2

uint8_t pipeNumber;
uint8_t payloadSize;
String FIFO;
const uint64_t pipeNum = 0x7878787878LL;
struct data
{
  String a,b,c,d;
}dt;

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

}

char text[] = "Hello world";
char ackData[24];

void Radio()
{
  
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
      String FIFO;
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
}
void loop() {

  int16_t w;
  int16_t x;
  int16_t y;
  int16_t z;
  int8_t q = 0;
int8_t j = 0; 
String check = "25.25 -26 .55 -0.4";
char  text[] = "0";
  Radio();
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
delay(500);

}  

      
  //delay(1000);


