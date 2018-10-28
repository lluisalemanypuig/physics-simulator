TEMPLATE = app
TARGET = particles-renderer

QT += core gui opengl
CONFIG += c++11

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -g
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

LIBS += -lGL -lGLU

FORMS += mainwindow.ui

SOURCES +=						\
	main.cpp					\
	mainwindow.cpp				\
	simulation_renderer.cpp		\
	render_simulation.cpp		\
	utils.cpp					\
	sim1.cpp					\
	sim2.cpp					\
	sim3.cpp					\
	sim4.cpp					\
	sim5.cpp					\
	sim6.cpp

HEADERS +=						\
    mainwindow.hpp				\
    simulation_renderer.hpp		\
	utils.hpp

DISTFILES +=					\
	shaders/simpleshader.vert	\
    shaders/simpleshader.frag

RESOURCES += \
    resources.qrc

# mesh and .obj reader
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../opengl/release/ -lopengl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../opengl/debug/ -lopengl
else:unix: LIBS += -L$$OUT_PWD/../opengl/ -lopengl

INCLUDEPATH += $$PWD/../opengl
DEPENDPATH += $$PWD/../opengl

# physim library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../physim-release/ -lphysim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../physim-debug/ -lphysim
else:unix: LIBS += -L$$PWD/../../../physim-debug/ -lphysim

INCLUDEPATH += $$PWD/../../../physim
DEPENDPATH += $$PWD/../../../physim

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-release/libphysim.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-debug/libphysim.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-release/physim.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../physim-debug/physim.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../physim-debug/libphysim.a
