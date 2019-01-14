# Examples

This directory is intended to provide several examples on how to use
the _physim_ library. It contains two subprojects:

- [command-line](https://github.com/lluisalemanypuig/physics-simulator/tree/master/examples/command-line): this is
a command-line application that uses the __physim__ library. Mainly used for debugging purposes.
- [interfaces](https://github.com/lluisalemanypuig/physics-simulator/tree/master/examples/interfaces): this project
contains some OpenGL-based interactive applications implementing several simulations each.
    - The [particles](https://github.com/lluisalemanypuig/physics-simulator/tree/master/examples/interfaces/particles)
    interface sets up several simulations involving sizeless-particles and sized-particles. Some of
    the applications mix the the two types of particles.
    - The [spring-meshes](https://github.com/lluisalemanypuig/physics-simulator/tree/master/examples/interfaces/spring-meshes)
    interface sets up four different simulations involving both 1-dimensional and 2-dimensional
    simulations.
    - The [fluids](https://github.com/lluisalemanypuig/physics-simulator/tree/master/examples/interfaces/fluids)
    __(to be completed)__.

## Compilation of the examples

The examples can be easily compiled following similar steps:

        qmake -makefile command-line/command-line.pro -o command-line-release/Makefile
        cd command-line-release && make
        
        qmake -makefile interfaces/interfaces.pro -o interfaces-release/Makefile
        cd interfaces-release && make

## Dependencies of the examples

The examples require the following libraries:

- OpenGL (tested under 4.6) or, GLUT, GLEW (tested under version 1.2.3.4).
- [glm](https://glm.g-truc.net/0.9.9/index.html) (tested under version 0.9.5 or older)
- [png](http://www.libpng.org/pub/png/libpng.html) to record videos (only for the _interfaces_ subproject).

The _.pro_ files of each subproject assume that these libraries are installed in the system.
Also, the library _physim_ needs to be compiled and placed in a particular directory:

		physics-simulator:
			|-- physim/
			|-- physim-release/
			|-- examples/
                            |--command-line/
                            |--command-line-release/
                            |--interfaces/
                            |--interfaces-release/

If the physim library, or the examples, are to be placed and/or compiled into different directories
then the _.pro_ files have to be modified appropriately.
