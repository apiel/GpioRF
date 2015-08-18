#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>
#include <sstream>


#include <unistd.h> // usleep
#include <fstream> // readfile
#include <string> // readfile

using namespace std;

void scheduler_realtime() {
	struct sched_param p;
	p.__sched_priority = sched_get_priority_max(SCHED_RR);
	if( sched_setscheduler( 0, SCHED_RR, &p ) == -1 ) {
		perror("Failed to switch to realtime scheduler.");
	}
}

void scheduler_standard() { // Is it really necessary
	struct sched_param p;
	p.__sched_priority = 0;
	if( sched_setscheduler( 0, SCHED_OTHER, &p ) == -1 ) {
		perror("Failed to switch to normal scheduler.");
	}
}

void digitalWrite(string gpioPath, string value)
{
	ofstream myfile;
	myfile.open (gpioPath.c_str());
	myfile << value;
	myfile.close();
}

int main (int argc, char** argv)
{
	if (argc < 3 ) {
		cout << endl << "Usage: " << argv[0];
		cout << " gpio_path(eg:/sys/class/gpio_sw/PA6/data) data_destination_file(eg:./data.txt)" << endl << endl;
		cout << "Don't forget to configure the GPIO in writing mode, eg: echo 1 > /sys/class/gpio_sw/PA6/cfg" << endl << endl;
	}
	else {	
		string gpioPath = argv[1];
		string destPath = argv[2];

		cout << "GPIO path: " << gpioPath << endl;
		cout << "Transmitter start" << endl;

		struct timeval tSignalStart, tSignalEnd;
		string value, _signal;
		long signal, tSignal;
		ifstream myfile (destPath.c_str());
		if (myfile.is_open()) {
			scheduler_realtime();
			while (getline(myfile, value) && getline(myfile, _signal)) {
				signal = atol(_signal.c_str());
				digitalWrite(gpioPath, value);
				//cout << value << ':' << signal << endl;
				gettimeofday(&tSignalStart, NULL);
				do {
					gettimeofday(&tSignalEnd, NULL);
					tSignal = (tSignalEnd.tv_sec > tSignalStart.tv_sec?1000000L:0) 
						+ tSignalEnd.tv_usec - tSignalStart.tv_usec;

					//cout << tSignalStart.tv_sec << ":" << tSignalStart.tv_usec << endl;
					//cout << tSignalEnd.tv_sec << ":" << tSignalEnd.tv_usec << endl;
					//cout << tSignal << "<?" << signal << endl;
				} while (tSignal < signal);
			}
		    	myfile.close();
			scheduler_standard();
			cout << "Transmitter end" << endl;
		}
		else {
			perror("Could not open file.");
		}
	}
}
