######################################################################
# Automatically generated by qmake (3.0) Wed Nov 2 09:00:19 2016
######################################################################

TEMPLATE = app
TARGET = project
INCLUDEPATH += .\
            lodepng
CONFIG += c++11

# Input
HEADERS += algebra.h material.h polyroots.h scene.h shapes.h lodepng/lodepng.h \
    photon.h \
    misc.h
SOURCES += algebra.cpp \
           main.cpp \
           material.cpp \
           polyroots.cpp \
           scene.cpp \
           lodepng/lodepng.cpp \
    photon.cpp
