// header file contains Clock class
#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "TrafficLight.h"
#include "Lane.h"
#include <vector>

class Clock
{
	protected:
		TrafficLight ns; // traffic light for north-south
		TrafficLight ew; // traffic light for east-west
		
		IntSection ne;
		IntSection nw;
		IntSection se;
		IntSection sw;
		
		std::vector<Lane> lanes;
		std::vector<Section*> occupied; // stores pointers to occupied Sections

	public:
		Clock(int num, int green_north_south, int yellow_north_south, 
			  int green_east_west, int yellow_east_west, double prob_new_vehicle_north_south,
			  double prob_new_vehicle_east_west, double proportion_of_cars,
			  double proportion_of_SUVs, double prob_right_turn_cars,
			  double prob_right_turn_SUVs, double prob_right_turn_trucks);
		~Clock();
		std::vector<Section*> Tick();
};

#endif
