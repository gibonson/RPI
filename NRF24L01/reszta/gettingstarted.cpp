
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <stdint.h>
#include <string>

using namespace std;

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);



//const uint8_t pipes[][6] = {11, 22, 33, 44, 55, 66, 88 }; 

//const uint64_t pipes[2] = { 0xf0f0f0f0e1LL, 0xf0f0f0f0d2LL };
const uint64_t pipes[7] = {0xF111111111LL, 0xF777777777LL, 0xF333333333LL,
                               0xF444444444LL, 0xF555555555LL, 0xF999999999LL, 0xF888888888LL
                              };




	int id;

	
	string dataA;
	

int main(int argc, char** argv){


  cout << "TEST ODBIORU\n";

  // Setup and configure rf radio
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  // Dump the configuration of the rf unit for debugging
  radio.printDetails();





     // radio.openWritingPipe(pipes[1]);
     // radio.openReadingPipe(1,pipes[0]);

      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);



	
	radio.startListening();
	
	// forever loop
	while (1)
	{


			for (id = 1; id < 7; id++)   
			{
			radio.stopListening();                                   //zatrzymanie nasłuchiwania
			radio.openWritingPipe(pipes[id]);                    //otwarcie kanału nadawczego dla satelity id
			printf("#ID#");
			printf("%d\n", id);




			//radio.stopListening();	// First, stop listening so we can talk.

			// Take the time, and send it.  This will block until complete

			printf("Now sending...\n");
			unsigned long time = millis();
			

			
			bool ok = radio.write( &time, sizeof(unsigned long) );
			
			if (!ok){
				printf("failed.\n");
			}
			
			radio.startListening(); // Now, continue listening

			// Wait here until we get a response, or timeout (250ms)
			unsigned long started_waiting_at = millis();
			bool timeout = false;
			while ( ! radio.available() && ! timeout ) {
				if (millis() - started_waiting_at > 500 )
					timeout = true;
			}


			// Describe the results
			if ( timeout )
			{
				printf("Failed, response timed out.\n");
			}
			else
			{
				// Grab the response, compare, and send to debugging spew
				//unsigned long got_time;
				//radio.read( &got_time, sizeof(unsigned long) );
				//printf("Got response %lu, round-trip delay: %lu\n",got_time,millis()-got_time);
			
		char tab2[30] = {0};                                 //czyszczenie wiadomości
        radio.read(&tab2, sizeof(tab2));                     //odczyt wiadomości
		printf("%s\n",tab2);
			
			
			
			}
			sleep(1);

}

	} // forever loop

  return 0;
}

