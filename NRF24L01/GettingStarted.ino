#include <SPI.h>
#include "RF24.h"

RF24 radio(9,10);
//byte addresses[][6] = {11, 22, 33, 44, 55, 66, 88}; 
const uint64_t addresses[7] = {0xF111111111LL, 0xF777777777LL, 0xF333333333LL,
                               0xF444444444LL, 0xF555555555LL, 0xF999999999LL, 0xF888888888LL
                              }; 

#include <Adafruit_Sensor.h>                                //DHT
#include <DHT.h>                                            //DHT
#include <DHT_U.h>                                          //DHT
#define DHTPIN  2                                           // Pin podpięcai
//#define DHTTYPE           DHT11                             // DHT 11 czyli to niebieskie gówno
#define DHTTYPE           DHT22                           // DHT 22 (AM2302)
//#define DHTTYPE           DHT21                           // DHT 21 (AM2301)
DHT_Unified dht(DHTPIN, DHTTYPE);                           //DHT

int licz = 0;                                               //licznik komunikatów
String temp1 = "0";                                         //Termeratura zmiana ze stringa na int 
String hum1 = "0";                                          //Wilgotność zmiana ze stringa na int 
int light = 149;
int soil1 = 149;
int soil2 = 149;
int soil3 = 149;
int alarm = 149;

//---------
int id = 1;                                                 //nr Satelity to zmieniasz do testów
String nazwySatelity = "SAL";
//---------

void setup() {
  Serial.begin(115200);

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(addresses[0]);
  //radio.openReadingPipe(1,addresses[1]);// działa
  radio.openReadingPipe(1,addresses[1]);

  radio.startListening();  // Start the radio listening for data

  radio.printDetails();

  dht.begin();                                              //DHT
  sensor_t sensor;                                          //DHT

  pinMode(3, INPUT);                                        //pin do sensora ruchu

}

void loop() {
    char text0[30] = {0};//dodane
    char text2[30] = {0};
    unsigned long got_time;
    
    if( radio.available()){
      char text[30] = {0};                                    //czyszczenie wiadomości dla wiekszej ilosci informacji
                                                                    
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
      }

           pomiary();
      radio.stopListening();                                        // First, stop listening so we can talk   

      String dataA = nazwySatelity + ":" + temp1 + ":" + hum1 + ":" + light + ":" + soil1 + ":" + soil2 + ":" + soil3 + ":" + alarm + "::";;
      dataA.toCharArray(text0, dataA.length());             //konwersja stringa do tablicy znakó
      Serial.print(text0);
      bool ok0 = radio.write(&text0, sizeof(text2));        //wysłanie z zadaniem potwierdzenia
      if (ok0) Serial.print("  OK  ");
      else Serial.print(" ERROR");

      //radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      //Serial.print(F("Sent response "));
      //Serial.println(got_time);  

   }

} // Loop

void pomiary() 
{

    Serial.println("Odczyty z  DHT11");
    sensors_event_t event;                                    //DHT
    dht.temperature().getEvent(&event);                       //DHT

    if (isnan(event.temperature)) {
      temp1 = "ER";
      Serial.println("Blad odczytu temeratury");
    }
    else {
      temp1 = event.temperature;// + String("*C");
      Serial.print("Temperatura [°C]:\t\t"); Serial.println(temp1);
    }
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      hum1 = "ER";
      Serial.println("Blad odczytu wilgotnosci"); Serial.println();
    }
    else {
      hum1 = event.relative_humidity;// + String("%");            //DHT
      Serial.print("Wilgotność [%]:\t\t\t"); Serial.println(hum1);
    }

    
}
