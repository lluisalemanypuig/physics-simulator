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
    sim_300.cpp

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
