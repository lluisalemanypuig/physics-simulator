TEMPLATE = app

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES += \
    main.cpp \
    utils.cpp \
    study_cases.cpp \
    00_bounce_floor.cpp \
    01_roll_floor.cpp \
    02_bounce_sphere.cpp \
    03_tri_tri_plane.cpp \
    04_roll_plane_sphere.cpp \
    05_hose.cpp \
    06_mesh1d.cpp \
    07_mesh2d_regular.cpp \
    08_bounce_floor_sized.cpp \
    09_roll_floor_sized.cpp \
    10_bounce_sphere_sized.cpp

HEADERS += \
    utils.hpp \
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
