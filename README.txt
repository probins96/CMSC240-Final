# cmsc240_s2018_delta

TO COMPILE AND RUN THIS PROGRAM:
1. Type "make" into the command line and this will compile all necessary code to run the program. 
2. To run, simply type "./testAnimator" followed by the name of the desired input file. The one attached is named "example.txt"

Main Classes Implemented:
  1. CLOCK - stores the two traffic lights one for west/east and then one for south/north. It stores the "intersection" sections, the four 
  sections in the middle of the intersection. It also stores the lanes of traffic. Clock controls the progression of the simulation through
  it's "Tick()" method, in which it tells each lane to have its vehicles progress. "Tick()" also returns a list of pointers to the occupied 
  sections in all lanes. 
  2. LANE - This stores all sections of roadway, and controls entry and exit of vehicles. The method "link()" constructs the sections and 
  connects them (along with the int sections). The advance() method progresses all vehicles in that lane, telling them to try to proceed.
  After all vehicles have proceeded, if the first section is open it creates a new vehicle with a certain probability (and also determines 
  its final direction of travel).
  3. SECTION - Each section, stored in its lane, stores a pointer to the vehicle occupying it, as well as a pointer to the next section in 
  the lane. It also stores the index in the lane (where it is located in the lane). INTSECTION is a derived class of SECTION, and involves 
  additional consideration of whether a vehicle plans to turn or continue straight, as well as the color of the traffic light. It can 
  prevent movement when the light isn't green by returning "isOpen()" as false. 
  4. TRAFFIC LIGHT - Each traffic light has a current enum describing its state as green, yellow, or red. It keeps track time in order to 
  know when to switch colors. It controls traffic movement in and near intersection sections. 
  5. VEHICLE - Depending on a vehicle's status as a car, sport utility vehicle (or possibly even a crossover...), or a panel truck with a four
  wheel drive and mud tires, it stores between 2-4 pointers to the sections it occupies. The vehicle proceed method will advance the vehicle 
  if the next section returns "open". 
  
  Other Design Decisions:
  1. We decided to use vectors because of space efficiency. The vector that stores "vehicles" (within lane) functioned as a cyclical array, in 
  order to avoid memory reallocation which would invalidate pointers to its elements. 
      a. We had to first initialize a lane before creating its sections for the same reason. 
  2. We also realized we didn't need Car, SUV, and panel truck as seperate sub-classes of vehicle. Therefore, we decided to have Vehicle handle
     all those cases, keeping in mind its vehicle type.
