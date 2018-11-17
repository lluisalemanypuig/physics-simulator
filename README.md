# Physiscs simulator

This repository offers a series of algorithms for realistic, real-time
simulation of particles and systems of particles, including:
- single particles
- springs
- meshes of springs
- rigid bodies

The simulation of the first three items are done using force-based simulation.
The simulation of rigid bodies is done using position-based simulation.

## Dependencies

Building the library can be done using standard tools, like the Makefile
system. However, a .pro file is provided. In order to generate the Makefile
the _qmake_ tool is required (developed and tested under version 3.0). Also,
a compiler that supports C++11 is needed.

## Building the library

One can build the library by issuing the following commands:

        cd /path/to/library
        mkdir simulator-release
        qmake -makefile simulator/simulator.pro -o simulator-release/Makefile
        cd simulator-release
        make

## Context

This repository was developed during the first semester of the academic
year 2018-2019 as part of the Computer Animation course project from the
Master in Innovation and Research in Informatics (MIRI).
