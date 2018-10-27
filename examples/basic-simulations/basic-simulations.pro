TEMPLATE = app

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DNDEBUG -g
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

INCLUDEPATH += ../..

CONFIG(release, debug|release) {
	LIBS += -L../../physim-release/ -lphysim
}
CONFIG(debug, debug|release) {
	LIBS += -L../../physim-debug/ -lphysim
}

SOURCES +=					\
	main.cpp				\
	utils.cpp				\
	study_cases.cpp			\
	0_bounce_floor.cpp		\
	1_roll_floor.cpp		\
	2_bounce_sphere.cpp		\
	3_tri_tri_plane.cpp		\
    4_roll_plane_sphere.cpp \
    5_hose.cpp

HEADERS +=					\
	utils.hpp				\
    study_cases.hpp

