TEMPLATE = lib

CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
INCLUDEPATH += ..

HEADERS += \
    geometry/geometry.hpp \
    geometry/triangle.hpp \
    geometry/sphere.hpp \
    geometry/plane.hpp \
    simulator.hpp \
    particle.hpp \
    initialiser.hpp

SOURCES += \
    geometry/geometry.cpp \
    geometry/triangle.cpp \
    geometry/sphere.cpp \
    geometry/plane.cpp \
    simulator.cpp \
    particle.cpp \
    initialiser.cpp


