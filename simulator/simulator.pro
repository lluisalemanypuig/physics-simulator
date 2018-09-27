TEMPLATE = lib

CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
INCLUDEPATH += ..

HEADERS += \
    simulator.hpp \
    geometry.hpp \
    triangle.hpp \
    sphere.hpp \
    plane.hpp

SOURCES += \
    simulator.cpp \
    geometry.cpp \
    triangle.cpp \
    sphere.cpp \
    plane.cpp


