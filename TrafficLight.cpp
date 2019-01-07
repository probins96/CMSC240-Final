#ifndef __TRAFFICLIGHT_CPP__
#define __TRAFFICLIGHT_CPP__

#include "TrafficLight.h"

TrafficLight::TrafficLight() {}

TrafficLight::TrafficLight(TrafficLight::Color col, int green, int yellow)
{
	time = 0;
	color = col;
	lengthGreen = green;
	lengthYellow = yellow;
	lengthRed = green + yellow;
}

TrafficLight::~TrafficLight(){}

void TrafficLight::advanceTick()
{
	time++; // advance time 
	switch(color) // see what color the traffic light is now
	{
		case TrafficLight::RED:
			if(time >= lengthRed) // if it is time to change color
			{
				time = 0; // reset timer
				color = TrafficLight::GREEN; // change from red to green
			}
		break;
		case TrafficLight::GREEN:
			if(time >= lengthGreen) // if it is time to change color
			{
				time = 0; // reset timer
				color = TrafficLight::YELLOW; // change from green to yellow
			}
		break;
		case TrafficLight::YELLOW:
			if(time >= lengthYellow) // if it is time to change color
			{
				time = 0; // reset timer
				color = TrafficLight::RED; // change from yellow to red
			}
		break;
	}
}

TrafficLight::Color TrafficLight::getColor() // returns what color the traffic light is
{
	return color;
}

#endif
