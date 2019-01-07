//This file implements the "sections" of a lane, the number of which depends on file input.
//Key aspects include section occupancy and its "next" section.

#ifndef __SECTION_CPP__
#define __SECTION_CPP__

#include "Lane.h"

Section::Section(Lane::Direction l, int i) 
{ 
	open = true; // Section is not initially occupied
	next = nullptr;
	lane = l;
	index = i;
	vehicle = nullptr;
}

Section::Section(const Section& sec) // const copy constructor
{
	open = sec.open;
	next = sec.next;
	lane = sec.lane;
	index = sec.index;
	vehicle = sec.vehicle;
}

Section::~Section(){}

bool Section::isOpen(Section* sec) // returns whether the Section is occupied by a Vehicle
{
	return open;
}

Section* Section::getNext(Lane::Direction dir) // returns the next Section in the Lane
{
	return next;
}

void Section::occupy(Vehicle& veh) // this Section is now occupied by veh
{
	open = false;
	vehicle = &veh;
}

void Section::leave() // the vehicle is leaving this Section, it is now unoccupied
{
	open = true;
	vehicle = nullptr;
}

void Section::setNext(Section& sec) // sets the Section to point to sec as next
{
	next = &sec;
}

int Section::getIndex() // returns the index of the lane where the Section is stored (0 is first Section)
{
	return index;
}

Vehicle* Section::getVehicle() // returns a pointer to the vehicle occupying the Section
{
	return vehicle;
}

Lane::Direction Section::getLane() // returns which lane (direction) this Section is in
{
	return lane;
}

#endif
