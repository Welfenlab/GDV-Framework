#
# Leibniz Universität Hannover - Institute for Man-Machine-Communication
# Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
#
# You should have received a copy of the MIT License along with this program.
#

QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GDV-Framework
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        framework/mainwindow.cpp \
    framework/meshloader.cpp \
    framework/slotmapper.cpp \
    framework/performancemonitor.cpp \
    framework/gdvcanvas2d.cpp \
    framework/gdvcanvas3d.cpp \   
    examples/frameworkexample.cpp

HEADERS  += framework/mainwindow.h \
    framework/meshloader.h \
    interfaces/GdvCanvas.h \
    interfaces/GdvGui.h \
    framework/slotmapper.h \
    interfaces/RendererBase.h \
    framework/performancemonitor.h \
    framework/gdvcanvas2d.h \
    framework/gdvcanvas3d.h \
    interfaces/Tuple3.h \
    examples/frameworkexample.h

FORMS    += framework/mainwindow.ui

QMAKE_CXXFLAGS_RELEASE = -O3

textures.files = textures/*
textures.path = $$OUT_PWD/textures/

meshes.files = meshes/*
meshes.path = $$OUT_PWD/meshes/

INSTALLS += textures meshes


