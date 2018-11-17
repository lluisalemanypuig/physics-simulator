# Examples

This directory is intended to provide several examples on how to use
the _physim_ library. To start off, the directory [command-line]() contains
simple examples that show how to use the library in a command line application.

In [interfaces]() one will find several subprojects that render a simulation
using the _physim_ library.

## Dependencies

All projects depend on the _qmake_ tool for building the projects, and a compiler
that supports C++11.

Whereas the [command-line]() examples do not depend on external libraries, other than
the _physim_ library, the [interfaces]() examples do. Its dependencies are the
following:
- Spring meshes: [GLUT]() and [OpenGL]().
- Particles renderer: [Qt](), [GLUT]() and [OpenGL]().

## Building the examples
