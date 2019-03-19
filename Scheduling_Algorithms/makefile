# Varun 


CPP = g++
CC  = gcc
CFLAGS   = -Wall -c 


# --- working directories
WORKSPACE = $(shell pwd)
OUTPUTDIR = $(WORKSPACE)/
OBJECTC = sched_sim.o

# --- file sources
CSOURCES = \
	$(WORKSPACE)/sched_sim.cpp

CPPSOURCES = \
	$(WORKSPACE)/OS_program3.cpp


all: prog3

prog3: 
	$(CPP)  $(CSOURCES) $(CFLAGS) -o $(OUTPUTDIR)$(OBJECTC)
	$(CPP) -o $(OUTPUTDIR)prog3 $(OUTPUTDIR)$(OBJECTC) $(CPPSOURCES)


.PHONY: clean
clean:
	rm -rf prog3
	rm ./*.o
	
