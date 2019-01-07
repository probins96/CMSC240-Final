// Implements Lane class: each Lane stores a vector of Sections and a vector of Vehicles
// includes methods for advancing vehicles and linking Sections together

#ifndef __LANE_CPP__
#define __LANE_CPP__

#include "Lane.h"
#include <vector>

std::mt19937 Lane::rng = std::mt19937(8675306);
std::uniform_real_distribution<double> Lane::rand_double = std::uniform_real_distribution<double>(0.0, 1.0);

Lane::Lane(const Lane& lane)
{
	sections.reserve(lane.sections.capacity());
	sections = lane.sections;
	lDirection = lane.lDirection;
	
	vehicles.reserve(lane.vehicles.capacity());
	std::vector<Vehicle>::const_iterator it = lane.vehicles.cbegin();
	while(it != lane.vehicles.cend())
	{
		vehicles.push_back(*it);
		it++;
	}
	occupied.reserve(lane.occupied.capacity());
	occupied = lane.occupied;
	indexFirstVehicle = lane.indexFirstVehicle;
	indexLastVehicle = lane.indexLastVehicle;
	prob_new_vehicle = lane.prob_new_vehicle;
	proportion_of_cars = lane.proportion_of_cars;
	proportion_of_SUVs = lane.proportion_of_SUVs;
	prob_right_turn_cars = lane.prob_right_turn_cars;
	prob_right_turn_SUVs = lane.prob_right_turn_SUVs;
	prob_right_turn_trucks = lane.prob_right_turn_trucks;
}

Lane::Lane(Direction dir, int numSections,
		   double prob_new, double proportion_cars, double proportion_SUVs,
		   double prob_right_cars, double prob_right_SUVs,
		   double prob_right_trucks)
{
	vehicles.reserve(2 * numSections + 2);
	sections.reserve(2 * numSections);
	occupied.reserve(2 * numSections + 2);
	lDirection = dir; // assign the Lane a direction
	prob_new_vehicle = prob_new;
	proportion_of_cars = proportion_cars;
	proportion_of_SUVs = proportion_SUVs;
	prob_right_turn_cars = prob_right_cars;
	prob_right_turn_SUVs = prob_right_SUVs;
	prob_right_turn_trucks = prob_right_trucks;
	indexFirstVehicle = 0;
	indexLastVehicle = 0;
}

Section& Lane::link(int numSections, IntSection& one, IntSection& two)
{
	vehicles.reserve(2 * numSections + 2);
	sections.reserve(2 * numSections);

	for(int i = 0; i < numSections; i++) // create inbound sections 
	{
		Section sec(lDirection, i);
		sections.push_back(sec);
	}
	for(int i = numSections + 2; i < 2 * numSections + 2; i++) // create outbound sections
	{
		Section sec(lDirection, i);
		sections.push_back(sec);
	}

	for(int i = 0; i < numSections - 1; i++) // now link sections together
	{
		sections.at(i).setNext(sections.at(i + 1));
	}
	sections.at(numSections - 1).setNext(two);
	for(int i = numSections; i < 2 * numSections - 1; i++)
	{
		sections.at(i).setNext(sections.at(i + 1));
	}
	return sections.at(numSections);
}

Lane::~Lane() {}

std::vector<Section*> Lane::advance()
{
	occupied.clear();
	std::vector<Section*> temp;
	for(int i = indexFirstVehicle; i != indexLastVehicle; i = (i + 1) % vehicles.capacity())
	{
		temp = vehicles.at(i).proceed(*this); // tell each vehicle to proceed
		std::vector<Section*>::iterator it2 = temp.begin();
		while(it2 != temp.end())
		{
			if(*it2 != nullptr)
			{
				occupied.push_back(*it2); // gather up all occupied Sections
			}
			it2++;
		}
		temp.clear();
	}
	
	if(sections.front().isOpen(nullptr)) // if first Section in the lane is open
	{
		double random = randDouble();
		if(random < prob_new_vehicle)    // test probability of creating new vehicle
		{
			double prob = randDouble();
			if(prob < proportion_of_cars) // creating a new car
			{
				Vehicle car(sections.front(), assignDir(prob_right_turn_cars), VehicleBase::CAR);
				if(indexLastVehicle < (int) vehicles.size()) // reuse memory of vehicles that have exited the lane
				{
					vehicles.at(indexLastVehicle) = car;
				}
				else
				{
					vehicles.push_back(car); // still filling out vehicles vector
				}
			}
			else if(prob < proportion_of_cars + proportion_of_SUVs) // creating an SUV
			{
				Vehicle suv(sections.front(), assignDir(prob_right_turn_SUVs), VehicleBase::SUV);
				if(indexLastVehicle < (int) vehicles.size())
				{
					vehicles.at(indexLastVehicle) = suv;
				}
				else
				{
					vehicles.push_back(suv);
				}
			}
			else
			{
				Vehicle truck(sections.front(), assignDir(prob_right_turn_trucks), VehicleBase::TRUCK);
				if(indexLastVehicle < (int) vehicles.size())
				{
					vehicles.at(indexLastVehicle) = truck;
				}
				else
				{
					vehicles.push_back(truck);
				}
			}
			indexLastVehicle = (indexLastVehicle + 1) % vehicles.capacity();  
			occupied.push_back(&(sections.front())); // first Section is now occupied
		}
	}
	return occupied;
}

void Lane::removeVehicle() // remove vehicle when it exits the lane
{
	indexFirstVehicle = (indexFirstVehicle + 1) % vehicles.capacity();
}

Lane::Direction Lane::assignDir(double prob_right_turn) // method returns a direction
														// determining right turn or straight
{
	double prob = randDouble();
	if (prob < prob_right_turn) // vehicle is turning right
	{
		switch(lDirection)
		{
			case Lane::NORTH:
				return Lane::EAST;
			case Lane::SOUTH:
				return Lane::WEST;
			case Lane::EAST:
				return Lane::SOUTH;
			case Lane::WEST:
				return Lane::NORTH;
		}
	}
	return lDirection; // vehicle is going straight
}


double Lane::randDouble() // returns a random double between 0.0 and 1.0
{
	return rand_double(rng);
}

#endif
