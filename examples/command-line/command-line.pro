TEMPLATE = app

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
LIBS += -fopenmp

SOURCES += \
    main.cpp \
    utils.cpp \
    sim_000.cpp \
    sim_001.cpp \
    sim_002.cpp \
    sim_003.cpp \
    sim_004.cpp \
    sim_005.cpp \
    sim_100.cpp \
    sim_101.cpp \
    sim_102.cpp \
    sim_200.cpp \
    sim_201.cpp \
    sim_900.cpp \
    sim_800.cpp \
    sim_801.cpp \
    sim_802.cpp \
    sim_300.cpp \
    sim_301.cpp \
    sim_302.cpp \
    kernel_functions_fluids.cpp

HEADERS += \
    utils.hpp \
    kernel_functions_fluids.hpp

DISTFILES += \
    example-models/cropped-cube.obj \
    example-models/cropped-cube.ply \
    example-models/pipe.obj \
    example-models/pipe.obj \
    example-models/slide.obj \
    example-models/slide.obj

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
