#ifndef __INTSECTION_CPP__
#define __INTSECTION_CPP__

#include "Lane.h"

IntSection::IntSection(const IntSection& sec) : Section(sec) // const copy constructor
{
	traf = sec.traf;
	out = sec.out;
	back = sec.back;
}

IntSection::IntSection(TrafficLight& tl, Lane::Direction dir, int i) : Section(dir, i) // constructor
{
	traf = &tl;
	out = nullptr;
	back = nullptr;
}

void IntSection::setExit(Section& sec) // out points to Section in outbound lane
{
	out = &sec;
}

void IntSection::setBack(Section& sec) // back points to IntSection from which vehicles
									   // passing through the intersection will come
{
	back = &sec;
}

Section* IntSection::getNext(Lane::Direction dir) // returns correct Section based on direction Vehicle travels
{
	if(dir == lane) // next points in direction of IntSection
	{
		return next;
	}
	return out; // out points to outbound lane
}

bool IntSection::isOpen(Section* sec)
{
	if(sec == back) // if vehicle is already in the intersection and is passing through
	{
		return open;
	}
	else if((*traf).getColor() == TrafficLight::GREEN) // if the vehicle is entering
													   // the intersection and has a green light
	{
		return open; // return whether the IntSection is occupied
	}
	return false; // if the light is yellow or red, do not advance vehicle
}

IntSection::~IntSection() {}

#endif
