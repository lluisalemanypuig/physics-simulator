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
    interface allows the user to define a fluid by giving values to several parameters (density, fluidity, ...).
    Three different simulations have been prepared: one to simulate a droplet of a fluid, another to fill up a
    small box, and another simulating a fluid falling on top of a money's head.

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

## Running the examples

### Command line

The _command-line_ example is simple. Once compiled navigate to the corresponding directory and issue the
command:
	
	./command-line --list

to see the different simulations available. Each of these simulations is briefly described in the application
and here:

		The available simulations are:
		Free particles:
		    000 : particle bouncing on a floor.
		    001 : particle rolling on a floor.
		    002 : particle bouncing on a sphere.
		    003 : two triangles on a floor. The particle
			  falls directly on a triangle and bounces.
		    004 : a particle that rolls on a plane then collides
			  with a sphere at the bottom of the plane.
		    005 : several geometrical objects. Intended for
			  efficiency measuring.

		Sized particles:
		    100 : sized particle bouncing on a floor.
		    101 : sized particle rolling on a floor.
		    102 : sized particle bouncing on a sphere.

		Spring meshes:
		    200 : simulate a very simple 1-dimensional mesh
			  of three particles
		    201 : simulate a two-dimensional regular mesh

		Fluids:
		    300 : simulate a very small and simple fluid
		    301 : simulate a very small and simple fluid
			  over several time steps
		    302 : simulate a very small and simple fluid
			  over several time steps with geometry

		Input testing:
		    800 : reads a model from a file and create a geometrical
			  object from it. Query of closest triangles.
		    801 : reads a point list from a file and queries the closest
			  points within a certain radius.
		    802 : reads a model from a file and create a geometrical
			  geometrical object from it. Query of closest triangles
			  within radius.

		Memory leak testing:
		    900 : Using vec3 inside a 'random' struct, fills an octree

### Interfaces

As explained above, this example provides several simple interfaces to visualise the different features
offered by the library. It also provides other interfaces but they were done for debugging purposes. Users
may ignore _octree-viewer_, _basic-viewer_ and _render_, the last one containing the classes used for renderisation.
Those examples showing the actual features of the library are: _particles_, _spring-meshes_ and _fluids_. They
can be run issuing the following commands:
	
	cd interfaces-release/particles ; ./particles --list
	cd interfaces-release/spring-meshes ; ./spring-meshes --list
	cd interfaces-release/fluids ; ./fluids --list

- The _particles_ example offers several simulations. When issuing the corresponding command the following
message summarising each of them will appear on screeen:
	
		Welcome to the particles renderer
		The list of simulations available are:

		Free particles:
		    000 : shower of 1000 particles on a plane.
		    001 : similar to '00' but with 300 particles and a
			  triangle and a rectangle in between.
		    002 : particles rolling down a rectangle and
			  colliding with a sphere.
		    003 : simulation of 16000 particles randomly shot
			  inside a quadrilateral (delimited by planes)
			  with geometry inside it (2 spheres and a triangle).
		    004 : to study the effects of the friction coefficient.
			  11 particles, each with a different friction cofficient
			  roll on an infinte plane with bouncing coefficient
			  fixed to 1.
		    005 : likewise '04' but for the bouncing coefficient.
			  Friction coefficient is fixed to 0.
		    006 : (testing) One free particle colliding with an object.

		Sized particles:
		    100 : (testing) sized particle bouncing on a plane.
		    101 : (testing) sized particle bouncing on a sphere.
		    102 : (testing) sized particle bouncing on a triangle.
		    103 : (testing) sized particle bouncing on a rectangle.
		    104 : (testing) two sized particles bouncing within a quadrilateral.
		    105 : (testing) three sized particles bouncing within a quadrilateral
			  with no gravity.
		    106 : (testing) three sized particles bouncing within a quadrilateral
			  with gravity. All planes have their normals pointing outwards
			  quadrilateral.
		    107 : (testing) One sized particle colliding with an object.

		Mixed particles:
		    200 : A shower of free particles on a sized particle.
		    201 : A sized particle going through a shower of free particles.

- The _spring-meshes_ example offers four simulations of spring meshes. When issuing the corresponding
command the following message summarising each of them will appear on screeen:

		Welcome to the Spring-meshes renderer
		The list of simulations available are:

		    00 : simulation of several one-dimensional meshes.
		    01 : simulation of a single two-dimensional mesh.
			Most of its parameters (mass, size, elasticity, ...)
			can be modified. The model of the mesh is a regular grid.
		    02 : simulation of a single two-dimensional
			mesh that falls on a sphere.
			Most of its parameters (mass, size, elasticity, ...)
			can be modified. The model of the mesh is a regular grid.
		    03 : simulation of a single two-dimensional
			mesh that falls on a geometrical object (pipe).
			Most of its parameters (mass, size, elasticity, ...)
			can be modified. The model of the mesh is a regular grid.
		    04 : simulation of a single two-dimensional
			mesh that falls on a geometrical object (monkey head).
			Most of its parameters (mass, size, elasticity, ...)
			can be modified. The model of the mesh is a regular grid.

- The _fluids_ example offers three simulations. When issuing the corresponding command the following
message summarising each of them will appear on screeen:

		Welcome to the Fluids renderer
		The list of simulations available are:

		    00 : simulation of a droplet of a fluid.
		    01 : filling a small box.
		    02 : fluid on monkey.

It is suggested that the _fluids_ simulations be recorded. This can be done by pressing letter _m_ after launching
one of the simulations. The application will start recording but not simulating the fluid. To do so, press letter
_b_. Since obtaining a "nice" fluid behaviour is somewhat difficult, the user is encouraged to try the following
parameters:

	./fluids 00 --nt 4 --it 1 --radius 0.08 --visc 0.001 --dt 0.01 --cs 1 --dens 1000 --sidex 16 --sidey 16 --sidez 16 --lenx 0.2 --leny 0.2 --lenz 0.2 --dens-kernel poly6 --press-kernel poly6 --visc-kernel poly6
	./fluids 01 --nt 4 --it 1 --radius 0.08 --visc 0.001 --dt 0.01 --cs 1 --dens 1000 --sidex 8 --sidey 64 --sidez 8 --lenx 0.1 --leny 1.2 --lenz 0.1 --dens-kernel poly6 --press-kernel poly6 --visc-kernel poly6
	./fluids 01 --nt 4 --it 10 --radius 0.08 --visc 0.001 --dt 0.001 --cs 1 --dens 1000 --sidex 8 --sidey 64 --sidez 8 --lenx 0.1 --leny 1.2 --lenz 0.1 --dens-kernel poly6 --press-kernel poly6 --visc-kernel poly6
	./fluids 01 --nt 4 --it 1 --radius 0.08 --visc 0.001 --dt 0.001 --cs 1 --dens 1000 --sidex 8 --sidey 64 --sidez 8 --lenx 0.1 --leny 1.2 --lenz 0.1 --dens-kernel poly6 --press-kernel poly6 --visc-kernel poly6
	./fluids 01 --nt 4 --it 1 --radius 0.08 --visc 0.001 --dt 0.001 --cs 2 --dens 1000 --sidex 8 --sidey 64 --sidez 8 --lenx 0.1 --leny 1.2 --lenz 0.1 --dens-kernel poly6 --press-kernel poly6 --visc-kernel poly6
	./fluids 02 --nt 4 --it 1 --radius 0.08 --visc 0.001 --dt 0.01 --cs 1 --dens 1000 --sidex 8 --sidey 64 --sidez 8 --lenx 0.1 --leny 1.2 --lenz 0.1 --dens-kernel poly6 --press-kernel poly6 --visc-kernel poly6
	./fluids 02 --nt 4 --it 1 --radius 0.08 --visc 0.001 --dt 0.001 --cs 1 --dens 1000 --sidex 8 --sidey 64 --sidez 8 --lenx 0.1 --leny 1.2 --lenz 0.1 --dens-kernel poly6 --press-kernel poly6 --visc-kernel poly6
	./fluids 02 --nt 4 --it 1 --radius 0.08 --visc 0.001 --dt 0.001 --cs 2 --dens 1000 --sidex 8 --sidey 64 --sidez 8 --lenx 0.1 --leny 1.2 --lenz 0.1 --dens-kernel poly6 --press-kernel poly6 --visc-kernel poly6

These parameters are guaranteed to make a "nice" fluid. Although there are minors differences between each set of
parameters, note that these can make huge changes in the simulations.Some of these simulations were recorded in this
[video](https://youtu.be/15qUi43HnmI).
