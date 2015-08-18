# GpioRF
Save and reproduce RF signal on Gpio

This 2 little programmes are made to reproduce the signal of a RF remote.


The programme RFsave will sniff the signal sent by the remote and save it into a file.

The programme RFsend will reproduce the signal from the saved file.

## Compile

No need of wiringPi!!

g++ -o RFsave RFsave.cpp

g++ -o RFsend RFsend.cpp

## Board / RF module

You will need a RF receiver and RF transmitter. For both connect Gcc on 1 Gcc pin of the board, Vcc on 1 5V pin of the board. For the receiver connect the data (digital) to 1 Gpio pin of the board, eg. Gpio17 pin 11. For the transmitter  connect the data to another Gpio pin of the board, eg. Gpio4 pin 7.

## How to use it

I made my test on OrangePi 2 but it should work as well for Raspberry or whatever...

On OrangePi 2, the path for Gpio 17 is /sys/class/gpio_sw/PA1

and the path for Gpio 4 is/sys/class/gpio_sw/PA6


Set Gpio 17 in mode read: echo 0 > /sys/class/gpio_sw/PA1/cfg

On RPI it should be something like: echo "in" /sys/class/gpio/gpio17/direction

While pressing the button you want to record, run the command: ./RFsave /sys/class/gpio_sw/PA1/data data.txt

On RPI it should be something like: ./RFsave /sys/class/gpio/gpio17/value data.txt

The signal is save in data.txt, you can repeat the method for different button and save them in different files.

Now we want to send the RF signal, run the command: ./RFsend /sys/class/gpio_sw/PA6/data data.txt

On RPI it should be something like: ./RFsend /sys/class/gpio/gpio4/value data.txt




