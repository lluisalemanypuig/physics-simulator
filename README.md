# Physiscs simulator

This repository offers a series of algorithms for real-time simulation of particles
and systems of particles, including:
- Size-less particles.
- Sized particles (consider each particle has a certain radius).
- Agent particles (simulation of agents for crowds).
- Springs (1-dimensional springs).
- Meshes of springs (2-dimensional springs - regular grid model).
- Fluids (Newtonian).

## Examples

See the [examples](https://github.com/lluisalemanypuig/physics-simulator/tree/master/examples) directory to see
how to use this library.

## Documentation

The [docs](https://github.com/lluisalemanypuig/physics-simulator/tree/master/docs) contains the library's
documentation generated with [Doxygen](http://www.doxygen.nl/).

## Dependencies

Building the library can be done using the _.pro_ file provided. In order to generate
the Makefile the _qmake_ tool is required (developed and tested under version 3.0). Also,
a compiler that supports C++11 is needed. Also, [OpenMP](http://www.openmp.org/) is needed
for the parallelised version of some of the algorithms.

## Building the library

One can build the library by issuing the following commands:

        qmake -makefile simulator/simulator.pro -o simulator-release/Makefile
        cd simulator-release && make

## Context

This project was developed during the first semester of the academic
year 2018-2019 as part of the Computer Animation course project from the
Master in Innovation and Research in Informatics (MIRI).
