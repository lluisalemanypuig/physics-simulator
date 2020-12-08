# Physiscs simulator

This repository offers a series of algorithms for real-time simulation of particles and systems of particles, including:
- Size-less particles.
- Sized particles (consider each particle has a certain radius).
- Agent particles (simulation of agents for crowds).
- Springs (1-dimensional springs).
- Meshes of springs (2-dimensional springs - regular grid model).
- Fluids (Newtonian).

## Examples

See the [examples](https://github.com/lluisalemanypuig/physics-simulator/tree/master/examples) directory to see how to use this library. Some of these examples have been recorded and pieced together to make some videos:
- Simulation of [particles](https://youtu.be/cjhv6R0XVQ0).
- Simulation of [spring meshes](https://youtu.be/mnILFxv_1is).
- Simulation of [fluids](https://youtu.be/15qUi43HnmI).

Also, the [character-animation](https://github.com/lluisalemanypuig/character-animation/) project shows how to use this library for crowd simulation.

## Documentation

The [docs](https://github.com/lluisalemanypuig/physics-simulator/tree/master/docs) contains the library's documentation generated with [Doxygen](http://www.doxygen.nl/).

## Dependencies

Building the library can be done using the _.pro_ file provided. In order to generate the Makefile the _qmake_ tool is required (developed and tested under version 3.0). Also, a compiler that supports C++11 is needed. Also, [OpenMP](http://www.openmp.org/) is needed for the parallelised version of some of the algorithms.

## Building the library

One can build the library by issuing the following commands:

        qmake -makefile simulator/simulator.pro -o simulator-release/Makefile
        cd simulator-release && make

## Context

This repository contains the first [Computer Animation](https://www.fib.upc.edu/en/studies/masters/master-innovation-and-research-informatics/curriculum/syllabus/CA-MIRI) course project, carried out during the first semester of the academic year 2018-2019. The course is part of the [Master in Innovation and Research in Informatics (MIRI)](https://www.fib.upc.edu/en/studies/masters/master-innovation-and-research-informatics) curriculum.
