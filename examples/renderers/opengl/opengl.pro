QT += opengl
QT -= core gui

CONFIG += c++11

TARGET = opengl
TEMPLATE = lib

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -g
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES += \
    obj_reader.cpp \
    mesh/mesh.cpp \
    mesh/mesh_utils.cpp \
    mesh/rendered_mesh.cpp

HEADERS +=\
    obj_reader.hpp \
    mesh/mesh.hpp \
    mesh/mesh_utils.hpp \
    mesh/rendered_mesh.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
