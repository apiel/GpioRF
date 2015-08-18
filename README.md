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

You will need a RF receiver and RF transmitter. For both connect Gcc on 1 Gcc pin of the board, Vcc on 1 5V pin of the board. For the receiver connect the data (digital) to 1 Gpio pin of the board, eg. Gpio17 pin 11. For the transmitter 

## How to use it


