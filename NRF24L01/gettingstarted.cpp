
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <stdint.h>
#include <string>
#include <stdio.h>
#include <time.h> 
#include <fstream>

using namespace std;


RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ); // Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
const uint64_t pipes[7] = {0xF111111111LL, 0xF777777777LL, 0xF333333333LL, 0xF444444444LL, 0xF555555555LL, 0xF999999999LL, 0xF888888888LL};
int id;
string dataA;

	

int main(int argc, char** argv){


  radio.begin(); // Setup and configure rf radio
  radio.setRetries(15,15); // optionally, increase the delay between retries & # of retries
  radio.printDetails();  // Dump the configuration of the rf unit for debugging
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
	

	std::string nazwa = "pusty";
	
	
  string line;
  ifstream myfile ("file.txt");
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      cout << line << '\n';
	  nazwa = line;
    }
    myfile.close();
  }
  else cout << "Unable to open file"; 

	
	
	
	while (1)
	{
		for (id = 1; id < 8; id++)   
		{
			radio.stopListening();                                   //zatrzymanie nasłuchiwania
			radio.openWritingPipe(pipes[id]);                    //otwarcie kanału nadawczego dla satelity id
			printf("ID");
			printf("%d", id);
			printf (" %ld ", time (NULL));

			unsigned long time = millis();
			radio.write( &time, sizeof(unsigned long) );
			
			radio.startListening(); // Now, continue listening

			unsigned long started_waiting_at = millis();// Wait here until we get a response, or timeout (250ms)
			bool timeout = false;
			while ( ! radio.available() && ! timeout ) 
			{
				if (millis() - started_waiting_at > 1000 )
					timeout = true;
			}
			
			if ( timeout )// Describe the results
			{
				printf("Failed\n");
				std::cout<<nazwa<<'\n';
			}
			else
			{
			char tab2[30] = {0};                                 //czyszczenie wiadomości
			radio.read(&tab2, sizeof(tab2));                     //odczyt wiadomości
			printf("%s\n",tab2);
			}
			sleep(1);
		}

	}

  return 0;
}

