TEMPLATE = app

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DNDEBUG -g
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
    5_hose.cpp

HEADERS +=					\
	utils.hpp				\
    study_cases.hpp

# physim library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../physim-release/ -lphysim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../physim-debug/ -lphysim
else:unix: LIBS += -L$$PWD/../../physim-debug/ -lphysim

INCLUDEPATH += $$PWD/../..
DEPENDPATH += $$PWD/../..

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../physim-release/libphysim.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../physim-debug/libphysim.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../physim-release/physim.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../physim-debug/physim.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../physim-debug/libphysim.a
