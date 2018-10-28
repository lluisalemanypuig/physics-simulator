QT += opengl
QT -= core gui

CONFIG += c++11

TARGET = opengl
TEMPLATE = lib

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -g
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES +=					\
	obj_reader.cpp			\
	mesh/mesh.cpp			\
	mesh/mesh_utils.cpp		\
	mesh/rendered_mesh.cpp	\
	rgeom/rgeom.cpp			\
	rgeom/rplane.cpp		\
	rgeom/rrectangle.cpp	\
	rgeom/rsphere.cpp		\
    rgeom/rtriangle.cpp

HEADERS +=					\
	obj_reader.hpp			\
	mesh/mesh.hpp			\
	mesh/mesh_utils.hpp		\
	mesh/rendered_mesh.hpp	\
    rgeom/rendered_geometry.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../physim-release/ -lphysim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../physim-debug/ -lphysim
else:unix: LIBS += -L$$PWD/../../../physim-debug/ -lphysim

INCLUDEPATH += $$PWD/../../../physim
DEPENDPATH += $$PWD/../../../physim

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-release/libphysim.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-debug/libphysim.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-release/physim.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-debug/physim.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../physim-debug/libphysim.a
