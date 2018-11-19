TEMPLATE = lib
TARGET = base

CONFIG += c++11
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

HEADERS += \
    obj_reader.hpp \
    box.hpp \
    include_gl.hpp \
    cameras/orthogonal.hpp \
    cameras/perspective.hpp \
    cameras/observer.hpp \
    model/model.hpp \
    model/model_utils.hpp \
    model/rendered_model.hpp \
    scene/renderer.hpp \
    scene/sim_renderer.hpp \
    geometry/rsphere.hpp \
    geometry/rtriangle.hpp \
    geometry/rplane.hpp \
    geometry/rrectangle.hpp \
    geometry/rgeometry.hpp \
    shader.hpp \
    textures/stb_image.h \
    textures/std_image.hpp \
    textures/texture_loader.hpp \
    model/shader_helper.hpp

SOURCES += \
    obj_reader.cpp \
    box.cpp \
    geometry/rplane.cpp \
    geometry/rrectangle.cpp \
    geometry/rsphere.cpp \
    geometry/rtriangle.cpp \
    cameras/orthogonal.cpp \
    cameras/perspective.cpp \
    cameras/observer.cpp \
    model/model.cpp \
    model/model_utils.cpp \
    model/rendered_model.cpp \
    scene/renderer.cpp \
    scene/sim_renderer.cpp \
    geometry/rgeometry.cpp \
    shader.cpp \
    textures/texture_loader.cpp \
    model/shader_helper.cpp

# for 'self' includes
INCLUDEPATH += ..

# physim includes
INCLUDEPATH += ../../..
