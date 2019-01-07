//Names: Wesley Su, Palmer Robins, Miles Clikeman
//This file tests, and displays, our implementation of the traffic intersection simulation.

#include <iostream>
#include "Animator.h"
#include "VehicleBase.h"
#include "Clock.h"
#include <fstream>

int main(int argc, char* argv[])
{
    std::ifstream inputFile;

    if (argc != 2) 
    {
        std::cerr << "Usage: " << argv[0] << " You need to include an input file with intersection specifications." << std::endl;
        exit(0);
    }
    
    inputFile.open(argv[1]);
    if (!inputFile.is_open()) 
    {
        std::cerr << "Error: Unable to open specified input file: [" << argv[1] << "]." << std::endl;
        exit(1);
    }

    //Animator::MAX_VEHICLE_COUNT = 9999;  // vehicles will be displayed with four digits
    Animator::MAX_VEHICLE_COUNT = 999;  // vehicles will be displayed with three digits
    //Animator::MAX_VEHICLE_COUNT = 99;  // vehicles will be displayed with two digits

    int maxTime;   //Maximum time for the sim
    int halfSize;  // number of sections before intersection
    int ns_green;  //These 4 vars control timing of the traffic lights
    int ns_yellow;
    int ew_green;
    int ew_yellow;
    double prob_nsVehicle; //probability that a new car generates in NS lanes
    double prob_ewVehicle; //probability that a new car generates in EW lanes
    double prop_cars;      //proportion of cars/suvs
    double prop_SUVs;
    double probRight_cars; //probability that a vehicle turns right
    double probRight_SUVs;
    double probRight_trucks;
    double probLeft_cars;   //left turn probabilities not used
    double probLeft_SUVs;
    double probLeft_trucks;

    //Reading the values from the inputfile for our simulation
    inputFile >> maxTime >> halfSize >> ns_green >> ns_yellow >> ew_green >> ew_yellow;
    inputFile >> prob_nsVehicle >> prob_ewVehicle >> prop_cars >> prop_SUVs >> probRight_cars;
    inputFile >> probLeft_cars >> probRight_SUVs >> probLeft_SUVs >> probRight_trucks >> probLeft_trucks;

    inputFile.close();

    Animator anim(halfSize);

    //construct vectors of VehicleBase* of appropriate size, init to nullptr
    std::vector<VehicleBase*> westbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> eastbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> southbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> northbound(halfSize * 2 + 2, nullptr);

    char dummy;

    // test drawing vehicles moving east, west, north, and south
    //Construct an instance of clock, which will create the intersection
    Clock clock(halfSize, ns_green, ns_yellow, ew_green, ew_yellow, prob_nsVehicle, prob_ewVehicle, 
        prop_cars, prop_SUVs, probRight_cars, probRight_SUVs, probRight_trucks);

    //Stores the sections of each lane in the intersection
    std::vector<Section*> sections;

    // test drawing vehicles moving east, west, north, and south
    for (int i = 0; i <= maxTime; i++)
    {
        //tick the clock
        sections = clock.Tick();
        std::vector<Section*>::iterator it = sections.begin();
        
        //set up the vectors we are going to draw, based off info from our intersection
        while(it != sections.end())
        {   
            switch ( (*(*it)).getLane() ) // find which lane occupied Section is in
            {
                case Lane::NORTH:
                    northbound[(*(*it)).getIndex()] = (*(*it)).getVehicle(); 
                break;
                case Lane::SOUTH:
                    southbound[(*(*it)).getIndex()] = (*(*it)).getVehicle();
                break;
                case Lane::WEST:
                    westbound[(*(*it)).getIndex()] = (*(*it)).getVehicle();
                break;
                case Lane::EAST:
                    eastbound[(*(*it)).getIndex()] = (*(*it)).getVehicle();
                break;
            }
            it++;
        }
        //Draw the vehicles in each vector
        anim.setVehiclesNorthbound(northbound);
        anim.setVehiclesWestbound(westbound);
        anim.setVehiclesSouthbound(southbound);
        anim.setVehiclesEastbound(eastbound);

        anim.draw(i);
        std::cin.get(dummy);

        eastbound.assign(halfSize * 2 + 2, nullptr); // reset
        westbound.assign(halfSize * 2 + 2, nullptr); // reset
        northbound.assign(halfSize * 2 + 2, nullptr);// reset
        southbound.assign(halfSize * 2 + 2, nullptr);// reset

        sections.clear();
    }
}
