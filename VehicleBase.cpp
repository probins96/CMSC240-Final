#ifndef __VEHICLE_BASE_CPP__
#define __VEHICLE_BASE_CPP__

#include "VehicleBase.h"

int VehicleBase::vehicleCount = 0;

VehicleBase::VehicleBase(const VehicleBase& veh)
{
	vehicleID = veh.vehicleID;
	vehicleType = veh.vehicleType;
}

VehicleBase::VehicleBase(VehicleBase::VehicleType type)
    : vehicleID(VehicleBase::vehicleCount++), vehicleType(type) {}

VehicleBase::~VehicleBase() {}

#endif
