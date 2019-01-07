#ifndef __ANIMATOR_CPP__
#define __ANIMATOR_CPP__

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "Animator.h"

// used for drawing width -- can be defined by user
int Animator::MAX_VEHICLE_COUNT = 9999;

int Animator::DIGITS_TO_DRAW = Animator::MAX_VEHICLE_COUNT <= 1 ? 
        1 : static_cast<int>(log10(Animator::MAX_VEHICLE_COUNT)) + 1;

// these two will be repeated strings of '-' or ' ' based on the digit-width
// of MAX_NUM_VEHCILES above
std::string Animator::SECTION_BOUNDARY_EW = std::string(Animator::DIGITS_TO_DRAW, '-');
std::string Animator::EMPTY_SECTION = std::string(Animator::DIGITS_TO_DRAW, ' ');

const std::string Animator::SECTION_BOUNDARY_NS = "|";
const std::string Animator::ERROR_MSG =
    "Error in Animator::draw: must call all four Animator::setVehicles* methods prior to calling Animator::draw";

const std::string Animator::COLOR_RED   = "\033[1;31m";
const std::string Animator::COLOR_GREEN = "\033[1;32m";
const std::string Animator::COLOR_BLUE  = "\033[1;34m";
const std::string Animator::COLOR_BLACK = "\033[0m";

//======================================================================
//* Animator::Animator(int numSectionsBeforeIntersection)
//======================================================================
Animator::Animator(int numSectionsBeforeIntersection)
{
    // redo here in case the user set MAX_VEHCILE_COUNT differently
    Animator::DIGITS_TO_DRAW = Animator::MAX_VEHICLE_COUNT <= 1 ? 
        1 : static_cast<int>(log10(Animator::MAX_VEHICLE_COUNT)) + 1;
    Animator::SECTION_BOUNDARY_EW = std::string(Animator::DIGITS_TO_DRAW, '-');
    Animator::EMPTY_SECTION = std::string(Animator::DIGITS_TO_DRAW, ' ');

    numSectionsBefore = numSectionsBeforeIntersection;

    // each lane will be twice the number of sections provided (before and 
    // after the intersection) plus the two intersection sections
    eastToWest.resize(numSectionsBefore * 2 + 2);
    westToEast.resize(numSectionsBefore * 2 + 2);
    northToSouth.resize(numSectionsBefore * 2 + 2);
    southToNorth.resize(numSectionsBefore * 2 + 2);

    // this will set the values in the vector to 0 (false), indicating that
    // the user must set the vehicles in each of the four directions using the
    // setVehicles* functions
    vehiclesAreSet.resize(4);
}

//======================================================================
//* Animator::~Animator()
//======================================================================
Animator::~Animator() {}

//======================================================================
//* Animator::draw(int time)
//======================================================================
void Animator::draw(int time)
{
    // ensure all four setVehicles* methods have been called prior
    std::vector<bool>::iterator it = vehiclesAreSet.begin();
    for (; it != vehiclesAreSet.end(); it++)
        if (*it == false) throw std::runtime_error(Animator::ERROR_MSG.c_str());

    std::cout << "\x1B[2J\x1B[H";  // clears the screen

    drawNorthPortion(time);
    drawWestbound();
    drawEastbound();
    drawSouthPortion();

    // reset the values (to false) in the boolean vector, indicating that the 
    // user must set the vehicles in each of the four directions using the
    // setVehicles* functions
    vehiclesAreSet.clear();
    vehiclesAreSet.resize(4);
}

//======================================================================
//* std::string getVehicleColor(VehicleBase* vptr)
//======================================================================
std::string Animator::getVehicleColor(VehicleBase* vptr)
{
    switch (vptr->getVehicleType())
    {
        case VehicleBase::CAR: 
            return Animator::COLOR_RED;
        case VehicleBase::SUV: 
            return Animator::COLOR_BLUE;
        case VehicleBase::TRUCK: 
            return Animator::COLOR_GREEN;
    }
    return Animator::COLOR_BLACK;
}

//======================================================================
//* Animator::drawNorthPortion(int time)
//======================================================================
void Animator::drawNorthPortion(int time)
{
    for (int s = 0; s < numSectionsBefore; s++)
    {
        // draw empty spaces to account for E/W lanes to left of intersection
        for (int i = 0; i < numSectionsBefore; i++) 
            std::cout << (i > 0 ? " " : "") << Animator::EMPTY_SECTION;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        // either draw (a portion of) southbound vehicle if present, 
        // or an empty section
        if (northToSouth[s] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(northToSouth[s])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << northToSouth[s]->getVehicleID()
                      << Animator::COLOR_BLACK;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        // either draw (a portion of) northbound vehicle if present, 
        // or an empty section
        int section = southToNorth.size() - s - 1;
        if (southToNorth[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(southToNorth[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << southToNorth[section]->getVehicleID()
                      << Animator::COLOR_BLACK;

        std::cout << Animator::SECTION_BOUNDARY_NS << std::endl;

        if (s < numSectionsBefore - 1)  // last will be drawn by westbound method
        {
            // draw empty spaces to account for E/W lanes to left of intersection
            for (int i = 0; i < numSectionsBefore; i++) 
                std::cout << (i > 0 ? " " : "") << Animator::EMPTY_SECTION;
            std::cout << Animator::SECTION_BOUNDARY_NS << Animator::SECTION_BOUNDARY_EW
                      << Animator::SECTION_BOUNDARY_NS << Animator::SECTION_BOUNDARY_EW 
                      << Animator::SECTION_BOUNDARY_NS;
        }

        // draw the time halfway down, on right
        if (s == numSectionsBefore / 2) 
            std::cout << std::setfill(' ') 
                << std::setw((numSectionsBefore / 2) * Animator::DIGITS_TO_DRAW)
                << "time: " << time;

        if (s < numSectionsBefore - 1) std::cout << std::endl;
    }
}

//======================================================================
//* Animator::drawEastWestBoundary()
//======================================================================
void Animator::drawEastWestBoundary()
{
    for (int s = 0; s < numSectionsBefore; s++)
        std::cout << Animator::SECTION_BOUNDARY_EW 
            << (s == numSectionsBefore-1 ? Animator::SECTION_BOUNDARY_NS : " ");
    std::cout << Animator::SECTION_BOUNDARY_EW << Animator::SECTION_BOUNDARY_NS;
    std::cout << Animator::SECTION_BOUNDARY_EW << Animator::SECTION_BOUNDARY_NS;
    for (int s = 0; s < numSectionsBefore; s++)
        std::cout << Animator::SECTION_BOUNDARY_EW << " ";
    std::cout << std::endl;
}

//======================================================================
//* Animator::drawEastbound()
//======================================================================
void Animator::drawEastbound()
{
    drawEastWestBoundary();

    // handle all the west-to-east sections before the intersection
    for (int s = 0; s < numSectionsBefore; s++)
    {
        int section = s;
        if (westToEast[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION << "|";
        else
            std::cout << getVehicleColor(westToEast[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << westToEast[section]->getVehicleID()
                      << Animator::COLOR_BLACK << "|";
    }

    // now handle the intersection; the first spot in the west to east lane
    // could be occupied by a vehicle in the W2E lane or in the N2S lane
    VehicleBase* vptr = (westToEast[numSectionsBefore] != nullptr ?
            westToEast[numSectionsBefore] : northToSouth[numSectionsBefore + 1]);
    if (vptr == nullptr)
        std::cout << Animator::EMPTY_SECTION << "|";
    else
        std::cout << getVehicleColor(vptr)
                  << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                  << vptr->getVehicleID()
                  << Animator::COLOR_BLACK << "|";

    // and the second spot in the west to east lane could be occupied by a
    // vehicle in the W2E lane or in the S2N lane
    vptr = (westToEast[numSectionsBefore + 1] != nullptr ?
            westToEast[numSectionsBefore + 1] : southToNorth[numSectionsBefore]);
    if (vptr == nullptr)
        std::cout << Animator::EMPTY_SECTION << "|";
    else
        std::cout << getVehicleColor(vptr)
                  << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                  << vptr->getVehicleID()
                  << Animator::COLOR_BLACK << "|";

    // and now handle all the west-to-east sections after the intersection
    for (int s = numSectionsBefore + 2; s < static_cast<int>(westToEast.size()); s++)
    {
        int section = s;
        if (westToEast[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(westToEast[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << westToEast[section]->getVehicleID()
                      << Animator::COLOR_BLACK;
        std::cout << (s < static_cast<int>(westToEast.size()) - 1 ? "|" :  "");
    }
    std::cout << std::endl;

    drawEastWestBoundary();
}

//======================================================================
//* Animator::drawWestbound()
//======================================================================
void Animator::drawWestbound()
{
    drawEastWestBoundary();

    // handle all the east-to-west sections before the intersection
    // (drawing in reverse order of the vector)
    for (int s = 0; s < numSectionsBefore; s++)
    {
        int section = eastToWest.size() - s - 1;
        if (eastToWest[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION << "|";
        else
            std::cout << getVehicleColor(eastToWest[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << eastToWest[section]->getVehicleID()
                      << Animator::COLOR_BLACK << "|";
    }

    // now handle the intersection; the first spot encountered L to R in the
    // east to west lane could be occupied by a vehicle in the E2W lane or in
    // the N2S lane
    VehicleBase* vptr = (eastToWest[numSectionsBefore + 1] != nullptr ?
            eastToWest[numSectionsBefore + 1] : northToSouth[numSectionsBefore]);
    if (vptr == nullptr)
        std::cout << Animator::EMPTY_SECTION << "|";
    else
        std::cout << getVehicleColor(vptr)
                  << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                  << vptr->getVehicleID()
                  << Animator::COLOR_BLACK << "|";

    // and the second spot encountered L to R in the east to west lane could be
    // occupied by a vehicle in the E2W lane or in the S2N lane
    vptr = (eastToWest[numSectionsBefore] != nullptr ?
            eastToWest[numSectionsBefore] : southToNorth[numSectionsBefore + 1]);
    if (vptr == nullptr)
        std::cout << Animator::EMPTY_SECTION << "|";
    else
        std::cout << getVehicleColor(vptr)
                  << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                  << vptr->getVehicleID()
                  << Animator::COLOR_BLACK << "|";

    // and now handle all the east-to-west sections after the intersection
    // (drawing in reverse order of the vector)
    for (int s = numSectionsBefore + 2; s < static_cast<int>(eastToWest.size()); s++)
    {
        int section = eastToWest.size() - s - 1;
        if (eastToWest[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(eastToWest[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << eastToWest[section]->getVehicleID()
                      << Animator::COLOR_BLACK;
        std::cout << (s < static_cast<int>(eastToWest.size()) - 1 ? "|" :  "");
    }
    std::cout << std::endl;

}

//======================================================================
//* Animator::drawSouthPortion()
//======================================================================
void Animator::drawSouthPortion()
{
    for (int s = 0; s < numSectionsBefore; s++)
    {
        // draw empty spaces to account for E/W lanes to left of intersection
        for (int i = 0; i < numSectionsBefore; i++) 
            std::cout << (i > 0 ? " " : "") << Animator::EMPTY_SECTION;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        // either draw (a portion of) southbound vehicle if present, 
        // or an empty section
        int section = numSectionsBefore + s + 2;
        if (northToSouth[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(northToSouth[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << northToSouth[section]->getVehicleID()
                      << Animator::COLOR_BLACK;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        // either draw (a portion of) northbound vehicle if present, 
        // or an empty section
        section = numSectionsBefore - s - 1;
        if (southToNorth[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(southToNorth[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << southToNorth[section]->getVehicleID()
                      << Animator::COLOR_BLACK;

        std::cout << Animator::SECTION_BOUNDARY_NS << std::endl;

        if (s < numSectionsBefore - 1)  // no need to draw last section spacer
        {
            // draw empty spaces to account for E/W lanes to left of intersection
            for (int i = 0; i < numSectionsBefore; i++) 
                std::cout << (i > 0 ? " " : "") << Animator::EMPTY_SECTION;
            std::cout << Animator::SECTION_BOUNDARY_NS << Animator::SECTION_BOUNDARY_EW
                      << Animator::SECTION_BOUNDARY_NS << Animator::SECTION_BOUNDARY_EW 
                      << Animator::SECTION_BOUNDARY_NS << std::endl;
        }

    }
}

#endif
