#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#define PIN_CE  9
#define PIN_CSN 10

uint8_t pipeNumber;
uint8_t payloadSize;

const uint64_t pipeNum = 0x7878787878LL;

RF24 radio(PIN_CE, PIN_CSN);

void setup() {

  Serial.begin(115200);
  radio.begin();
  radio.setChannel(5);
  radio.setDataRate (RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
    radio.setAutoAck(true);
  radio.enableDynamicPayloads();
  radio.setRetries(15, 15); 
  
  radio.openWritingPipe(pipeNum);

}

char text[] = "Hello world";
  char ackData[24];
  
void loop() {

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
      String FIFO;
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
}
