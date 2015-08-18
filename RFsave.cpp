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

int digitalRead(string gpioPath)
{
  string line;
  ifstream myfile (gpioPath.c_str());
  if (myfile.is_open())
  {
    getline (myfile,line);
    //cout << line << '\n';
    myfile.close();
  }

  return atoi(line.c_str());
}

void saveData(string destPath, string data)
{
	ofstream myfile;
	myfile.open (destPath.c_str());
	myfile << data;
	myfile.close();

	cout << "Data saved in " << destPath << "." << endl;
}

int main (int argc, char** argv)
{
	if (argc < 3 ) {
		cout << endl << "Usage: " << argv[0];
		cout << " gpio_path(eg:/sys/class/gpio_sw/PA1/data) data_destination_file(eg:./data.txt) [wait_sec.default 3]" << endl << endl;
		cout << "Don't forget to configure the GPIO in reading mode, eg: echo 0 > /sys/class/gpio_sw/PA1/cfg" << endl << endl;
	}
	else {	
		string gpioPath = argv[1];
		string destPath = argv[2];
		int waitFor = argc > 3 ? atoi(argv[3]) : 3;

		cout << "GPIO path: " << gpioPath << endl;
		cout << "Receiver started" << endl;

		scheduler_realtime();
		
		std::ostringstream data;
		struct timeval tStart, tNow, tSignalStart, tSignalEnd;
		int curValue, value;
		long tSignal;
		gettimeofday(&tStart, NULL);
		curValue = digitalRead(gpioPath);
		do {
			data << curValue << endl;
			gettimeofday(&tSignalStart, NULL);
			do {
				value = digitalRead(gpioPath);
			} while (value == curValue);
			gettimeofday(&tSignalEnd, NULL);
			tSignal = (tSignalEnd.tv_sec > tSignalStart.tv_sec?1000000L:0) 
				+ tSignalEnd.tv_usec - tSignalStart.tv_usec;
			data << tSignal << endl;
			curValue = value;

		        //usleep(1);
			gettimeofday(&tNow, NULL);
    		} while ((tNow.tv_sec-tStart.tv_sec) < waitFor);
		scheduler_standard();
		//cout << data.str() << endl;
		saveData(destPath, data.str());
	}
}
