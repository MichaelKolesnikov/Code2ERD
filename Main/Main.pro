QT += core gui widgets svg

TEMPLATE = lib
TARGET = Main
DEFINES += MAIN_LIBRARY
CONFIG += shared

MAIN_SELF_BUILD = 1
include($$PWD/Main.pri)

SOURCES += \
    Theme.cpp \
    \
    Diagram/DiagramScene.cpp \
    Diagram/DiagramView.cpp \
    Diagram/EdgeItem.cpp \
    Diagram/NodeItem.cpp \
    Diagram/RestrictedMenu.cpp \
    \
    Editor/ErdEditor.cpp \
    Editor/ErdHighlighter.cpp \
    Editor/FindBar.cpp \
    \
    Export/ImageExporter.cpp \
    \
    MainWindow.cpp

HEADERS += \
    Main_global.h \
    Theme.h \
    \
    Diagram/DiagramScene.h \
    Diagram/DiagramStyle.h \
    Diagram/DiagramView.h \
    Diagram/EdgeItem.h \
    Diagram/ItemType.h \
    Diagram/NodeItem.h \
    Diagram/RestrictedMenu.h \
    \
    Editor/ErdEditor.h \
    Editor/ErdHighlighter.h \
    Editor/FindBar.h \
    \
    Export/ImageExporter.h \
    \
    MainWindow.h

RESOURCES += \
    Resources/resources.qrc
