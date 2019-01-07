EXECS = testAnimator
OBJS = testAnimator.o Animator.o VehicleBase.o Vehicle.o Lane.o Clock.o Section.o IntSection.o TrafficLight.o

CC = g++
CCFLAGS = -Wall -Wno-deprecated -Werror=return-type -g -std=c++11

all: $(EXECS)

testAnimator: $(OBJS)
	$(CC) $(CCFLAGS) $^ -o $@

%.o: %.cpp *.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	/bin/rm -f a.out $(OBJS) $(EXECS)
