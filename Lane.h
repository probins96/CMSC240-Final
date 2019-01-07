//Header file contains Lane, Vehicle, Section, IntSection classes

#ifndef __LANE_H__
#define __LANE_H__

#include "TrafficLight.h"
#include "VehicleBase.h"
#include <vector>
#include <iostream>
#include <random>

class Vehicle;
class Section;
class IntSection;

class Lane
{
	public:
		enum Direction {NORTH, SOUTH, EAST, WEST};
		Lane(const Lane& lane);
		Lane(Direction dir, int numSections,
			 double prob_new, double proportion_cars, double proportion_SUVs,
			 double prob_right_cars, double prob_right_SUVs,
			 double prob_right_trucks);
		~Lane();
		std::vector<Section*> advance();
		void removeVehicle();
		double randDouble();
		Section& link(int numSections, IntSection& one, IntSection& two);
		static std::mt19937 rng;
		static std::uniform_real_distribution<double> rand_double;

	protected:
		std::vector<Section> sections;
		Direction lDirection;
		std::vector<Vehicle> vehicles;
		int indexFirstVehicle;
		int indexLastVehicle;
		std::vector<Section*> occupied;
		double prob_new_vehicle;
		double proportion_of_cars;
		double proportion_of_SUVs;
		double prob_right_turn_cars;
		double prob_right_turn_SUVs;
		double prob_right_turn_trucks;
		Lane::Direction assignDir(double prob_right_turn);
};

class Vehicle : public VehicleBase
{
	public:
		Vehicle(const Vehicle& veh);
		Vehicle(Section& sec, Lane::Direction dir, VehicleBase::VehicleType type);
		~Vehicle();
		std::vector<Section*> proceed(Lane& lane);
		Lane::Direction getDirection();
		void operator=(const Vehicle& veh);

	protected:
		Lane::Direction vDirection;
		std::vector<Section*> location;
		std::vector<Section*> proceedCar(Lane& lane);
		std::vector<Section*> proceedSUV(Lane& lane);
		std::vector<Section*> proceedTruck(Lane& lane); 
};

class Section
{
	public:
		Section(Lane::Direction l, int i);
		Section(const Section& sec);
		~Section();
		virtual bool isOpen(Section* sec);
		virtual Section* getNext(Lane::Direction dir);
		void occupy(Vehicle& veh);
		void leave();
		void setNext(Section& sec);
		int getIndex();
		Vehicle* getVehicle();
		Lane::Direction getLane();

	protected:
		Section* next;
		bool open;
		Lane::Direction lane;
		Vehicle* vehicle;
		int index;
};

class IntSection : public Section
{
	public:
		IntSection(const IntSection& sec);
		IntSection(TrafficLight& tl, Lane::Direction dir, int i);
		void setExit(Section& sec);
		void setBack(Section& sec);
		Section* getNext(Lane::Direction dir);
		bool isOpen(Section* sec);
		~IntSection();

	protected:
		TrafficLight* traf;
		Section* out;
		Section* back;
};

#endif
