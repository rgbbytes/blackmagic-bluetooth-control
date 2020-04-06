#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
//Compile on a Raspberry Pi with a connected Maxbotix ultrasonic rangefinder LV-EZ connected through PWM0
//g++ readUS.cpp -lwiringPi -o readUS
int linearInterp(float value){
//the look up table will provide the distance in mm that each focus position goes to.
//to construct your own for your lens, follow bluetoothctl.txt for instructions.
//you will need to have 2 bluetoothctl shells open,
// one writing focus commands to the Outgoing Camera Control characteristic
// another receiving focus position data from the Incoming Camera Control characteristic,
//then you can construct this table
float lut[11][2]={
{0.0, 0.0},
{0.125, 250.0},
{0.25, 265.0},
{0.375, 325.0},
{0.5, 425.0},
{0.625, 645.0},
{0.75, 1560.0},
{0.8, 2975.0},
{0.825, 6790.0},
{0.875, 1000000.0},
{1.0, 1000001.0}};
int position=1;
while(value>lut[position][1]){position++;}
int returnValue=(int)((((value-lut[position-1][1])/(lut[position][1]-lut[position-1][1]))*(lut[position][0]-lut[position-1][0])+lut[position-1][0])*2048.0);
return returnValue;
}


int main (void){
wiringPiSetup();
pinMode(15, OUTPUT);
digitalWrite(15, HIGH);//In my wiring setup, I'm powering the device through this pin (BCM 14) so I can turn it off
pinMode(1, INPUT);//wiringPi 1 is BCM 18 or PWM0, and will be read.
clock_t start;
clock_t stop;
while(reading==1){}
while(reading==0){start=clock();}
while(reading==1){stop=clock();}
distance=(float)(stop-start)*34300.0/(CLOCKS_PER_SEC*1.88);//the 34300 is the speed of sound which has an air temperature dependance, the 1.88 is the adjustment based on experimental calibration
int input=linearInterp((float)distance*10.0-145.0);//-145 is the offset value which is the experimentally determined lens to sensor distance
printf("write \"1 8 0 0 0 0 128 0 %i %i 0 0\"", input%256, input/256);
//digitalWrite(15, LOW);
return 0;
}
