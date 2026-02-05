QT       += core gui widgets

CONFIG += c++17

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
    Mappers/ERDJsonMapper.cpp \
    Mappers/EntityMapper.cpp \
    Mappers/LineMapper.cpp \
    Mappers/LinkLineMapper.cpp \
    Mappers/LinkMapper.cpp \
    Mappers/PositionMapper.cpp \
    Mappers/PropertyLineMapper.cpp \
    Mappers/PropertyMapper.cpp \
    Models/ERDItemModel.cpp \
    Models/ERDModel.cpp \
    Models/EntityModel.cpp \
    Models/LineModel.cpp \
    Models/LinkLineModel.cpp \
    Models/LinkModel.cpp \
    Models/PropertyItemModel.cpp \
    Models/PropertyLineModel.cpp \
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
    Mappers/ERDJsonMapper.h \
    Mappers/EntityMapper.h \
    Mappers/LineMapper.h \
    Mappers/LinkLineMapper.h \
    Mappers/LinkMapper.h \
    Mappers/PositionMapper.h \
    Mappers/PropertyLineMapper.h \
    Mappers/PropertyMapper.h \
    Models/ERDItemModel.h \
    Models/ERDModel.h \
    Models/EntityModel.h \
    Models/LineModel.h \
    Models/LinkLineModel.h \
    Models/LinkModel.h \
    Models/PropertyLineModel.h \
    Models/PropertyModel.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
