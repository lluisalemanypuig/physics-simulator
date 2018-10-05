TEMPLATE = app

CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS += -static
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

INCLUDEPATH += ../..

CONFIG(release, debug|release) {
    LIBS += -L../../simulator-release/ -lsimulator
}
CONFIG(debug, debug|release) {
    LIBS += -L../../simulator-debug/ -lsimulator
}

SOURCES += main.cpp \
    utils.cpp \
    tri_tri_plane.cpp \
    study_cases.cpp \
    bounce_floor.cpp \
    roll_floor.cpp

HEADERS += \
    study_cases.hpp \
    utils.hpp

