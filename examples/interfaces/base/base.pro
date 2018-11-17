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
    render_geometry/rendered_geometry.hpp \
    cameras/orthogonal.hpp \
    cameras/perspective.hpp \
    cameras/observer.hpp \
    model/model.hpp \
    model/model_utils.hpp \
    model/rendered_model.hpp \
    scene/renderer.hpp \
    scene/sim_renderer.hpp \
    render_geometry/rsphere.hpp \
    render_geometry/rtriangle.hpp \
    render_geometry/rplane.hpp \
    render_geometry/rrectangle.hpp

SOURCES += \
    obj_reader.cpp \
    box.cpp \
    render_geometry/rendered_geometry.cpp \
    render_geometry/rplane.cpp \
    render_geometry/rrectangle.cpp \
    render_geometry/rsphere.cpp \
    render_geometry/rtriangle.cpp \
    cameras/orthogonal.cpp \
    cameras/perspective.cpp \
    cameras/observer.cpp \
    model/model.cpp \
    model/model_utils.cpp \
    model/rendered_model.cpp \
    scene/renderer.cpp \
    scene/sim_renderer.cpp

# for 'self' includes
INCLUDEPATH += ..

# physim includes
INCLUDEPATH += ../../..
