// header file contains TrafficLight class
#ifndef __TRAFFICLIGHT_H__
#define __TRAFFICLIGHT_H__

class TrafficLight
{
	public:
		enum Color {RED, YELLOW, GREEN}; // possible colors
		TrafficLight();
		TrafficLight(Color col, int green, int yellow);
		~TrafficLight();
		void advanceTick();
		Color getColor();

	protected:
		Color color;
		int lengthRed;
		int lengthYellow;   
		int lengthGreen;    
		int time;	        // keeps track of when to change color
};

#endif
