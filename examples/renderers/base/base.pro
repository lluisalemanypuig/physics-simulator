TEMPLATE = lib
TARGET = base

CONFIG += c++11
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

# for 'self' includes
INCLUDEPATH += ..
# physim includes
INCLUDEPATH += ../../../

HEADERS +=					\
	obj_reader.hpp			\
	rgeom/rendered_geometry.hpp \
	cameras/orthogonal.hpp \
	cameras/perspective.hpp \
	box.hpp \
	model/model.hpp \
	model/model_utils.hpp \
	model/rendered_model.hpp \
    include_gl.hpp \
    cameras/first_person.hpp \
    cameras/observer.hpp

SOURCES +=					\
	obj_reader.cpp			\
	rgeom/rgeom.cpp			\
	rgeom/rplane.cpp		\
	rgeom/rrectangle.cpp	\
	rgeom/rsphere.cpp		\
    rgeom/rtriangle.cpp \
    cameras/orthogonal.cpp \
    cameras/perspective.cpp \
    box.cpp \
    model/model.cpp \
    model/model_utils.cpp \
    model/rendered_model.cpp \
    cameras/first_person.cpp \
    cameras/observer.cpp
