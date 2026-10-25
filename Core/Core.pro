QT -= gui

TEMPLATE = lib
TARGET = Core
DEFINES += CORE_LIBRARY
CONFIG += shared

CORE_SELF_BUILD = 1
include($$PWD/Core.pri)

SOURCES += \
    DTO/DiagramDTO.cpp \
    \
    Erd/ErdLexer.cpp \
    Erd/ErdParser.cpp \
    \
    Layout/AutoLayout.cpp \
    Layout/LayoutMapper.cpp

HEADERS += \
    Core_global.h \
    \
    DTO/AttributeDTO.h \
    DTO/DiagramDTO.h \
    DTO/EntityDTO.h \
    DTO/RelationshipDTO.h \
    \
    Erd/ErdError.h \
    Erd/ErdLexer.h \
    Erd/ErdParser.h \
    \
    Layout/AutoLayout.h \
    Layout/LayoutDTO.h \
    Layout/LayoutMapper.h
