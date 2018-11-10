TEMPLATE = app

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES +=					\
	main.cpp				\
	utils.cpp				\
	study_cases.cpp			\
	0_bounce_floor.cpp		\
	1_roll_floor.cpp		\
	2_bounce_sphere.cpp		\
	3_tri_tri_plane.cpp		\
    4_roll_plane_sphere.cpp \
	5_hose.cpp				\
    6_mesh1d.cpp			\
    7_mesh2d_regular.cpp

HEADERS +=					\
	utils.hpp				\
    study_cases.hpp

# physim library
CONFIG(debug, debug|release) {
    LIBS += -L../../physim-debug/ -lphysim
    PRE_TARGETDEPS += ../../physim-debug/libphysim.a
}
CONFIG(release, debug|release) {
    LIBS += -L../../physim-release/ -lphysim
    PRE_TARGETDEPS += ../../physim-release/libphysim.a
}
INCLUDEPATH += ../..
DEPENDPATH += ../..
