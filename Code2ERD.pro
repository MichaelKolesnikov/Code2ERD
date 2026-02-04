QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ERDScene.cpp \
    Holders/NameHolder.cpp \
    Holders/PositionHolder.cpp \
    Items/ERDItem.cpp \
    Items/EntityItem.cpp \
    Items/LineItem.cpp \
    Items/LinkItem.cpp \
    Items/LinkLineItem.cpp \
    Items/PropertyItem.cpp \
    Items/PropertyLineItem.cpp \
    Models/ERDItemModel.cpp \
    Models/ERDModel.cpp \
    Models/EntityItemModel.cpp \
    Models/LineItemModel.cpp \
    Models/LinkItemModel.cpp \
    Models/LinkLineItemModel.cpp \
    Models/PropertyItemModel.cpp \
    Models/PropertyLineItemModel.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    ERDScene.h \
    Holders/NameHolder.h \
    Holders/PositionHolder.h \
    Interfaces/IERDItem.h \
    Items/ERDItem.h \
    Items/ERDItemType.h \
    Items/EntityItem.h \
    Items/LineItem.h \
    Items/LinkItem.h \
    Items/LinkLineItem.h \
    Items/PropertyItem.h \
    Items/PropertyLineItem.h \
    Items/ZValue.h \
    MainWindow.h \
    Models/ERDItemModel.h \
    Models/ERDModel.h \
    Models/EntityItemModel.h \
    Models/LineItemModel.h \
    Models/LinkItemModel.h \
    Models/LinkLineItemModel.h \
    Models/PropertyItemModel.h \
    Models/PropertyLineItemModel.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
