/*------------------------------------------------------------------------------
  This example illustrates how to use the LIDAR-Lite library to gain quick
  access to the basic functions of LIDAR-Lite via the Raspberry Pi interface.
  Additionally, it provides users of any platform with a template for their
  own application code.

  Copyright (c) 2019 Garmin Ltd. or its subsidiaries.
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
  http://www.apache.org/licenses/LICENSE-2.0
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
------------------------------------------------------------------------------*/
//compile on a Raspberry Pi with a Lidar Lite V3 sensor connected through I2C with
//g++ readLidar.cpp lidarlite_v3.cpp -I . -o readLidar
#include <linux/types.h>
#include <cstdio>

#include <lidarlite_v3.h>

LIDARLite_v3 myLidarLite;

int linearInterp(float value){
//experimentally determined look up table for your lens (this is on a 25mm Olympus f1.8)
//generated based on using two bluetoothctl shells simultaneously,
//one receiving notifications from the Incoming Camera Control characteristic
//another writing focus distances 0.0 to 1.0 to the Outgoing Camera Control characteristic
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


int main()
{
__u16 distance;
myLidarLite.i2c_init();
//myLidarLite.configure(0);//optional configuration
myLidarLite.takeRange();
distance = myLidarLite.readDistance();

int input=linearInterp((float)distance*10.0-145.0);//-145 is the offset value which is the experimentally determined lens to sensor distance
printf("write \"1 8 0 0 0 0 128 0 %i %i 0 0\"", input%256, input/256);
}
