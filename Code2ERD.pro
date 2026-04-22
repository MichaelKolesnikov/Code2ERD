QT       += core gui widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Items/AnchorItem.cpp \
    Items/SelectionGroupItem.cpp \
    Managers/LineGeometryManager.cpp \
    Mappers/DoublePairMapper.cpp \
    Mappers/PositionMapper.cpp \
    Mappers/SizeMapper.cpp \
    Models/IdNamePositionSizeTypeModel.cpp \
    Scene.cpp \
    SceneView.cpp \
    Tools/NewLineDrawerTool.cpp \
    Tools/SelectionTool.cpp \
    Tools/Tool.cpp \
    Undo/AddRemoveCommand.cpp \
    Holders/NameHolder.cpp \
    Holders/PositionHolder.cpp \
    Items/ERDItem.cpp \
    Items/EntityItem.cpp \
    Items/LineItem.cpp \
    Items/LinkLineItem.cpp \
    Mappers/ERDJsonMapper.cpp \
    Mappers/EntityMapper.cpp \
    Mappers/LineMapper.cpp \
    Mappers/LinesConnectionsMapper.cpp \
    Mappers/LinkLineMapper.cpp \
    Models/ERDItemModel.cpp \
    Models/ERDModel.cpp \
    Models/LineModel.cpp \
    Models/LinesConnectionsModel.cpp \
    Models/LinkLineModel.cpp \
    RestrictedMenu.cpp \
    Undo/ReplaceErdModelCommand.cpp \
    Undo/UndoCommand.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    DTO/ERDItemDTO.h \
    DTO/IdNamePositionSizeTypeDTO.h \
    DTO/LineDTO.h \
    DTO/LinkLineDTO.h \
    Items/AnchorItem.h \
    Items/IRectHolder.h \
    Items/SelectionGroupItem.h \
    Managers/LineGeometryManager.h \
    Mappers/DoublePairMapper.h \
    Mappers/PositionMapper.h \
    Mappers/SizeMapper.h \
    Models/IdNamePositionSizeTypeModel.h \
    Scene.h \
    SceneView.h \
    Tools/NewLineDrawerTool.h \
    Tools/SelectionTool.h \
    Tools/Tool.h \
    Undo/AddRemoveCommand.h \
    Holders/NameHolder.h \
    Holders/PositionHolder.h \
    Interfaces/IERDItem.h \
    Items/ERDItem.h \
    Items/ERDItemType.h \
    Items/EntityItem.h \
    Items/LineItem.h \
    Items/LinkLineItem.h \
    Items/ZValue.h \
    MainWindow.h \
    Mappers/ERDJsonMapper.h \
    Mappers/EntityMapper.h \
    Mappers/LineMapper.h \
    Mappers/LinesConnectionsMapper.h \
    Mappers/LinkLineMapper.h \
    Models/ERDItemModel.h \
    Models/ERDModel.h \
    Models/LineModel.h \
    Models/LinesConnectionsModel.h \
    Models/LinkLineModel.h \
    RestrictedMenu.h \
    Undo/ReplaceErdModelCommand.h \
    Undo/UndoCommand.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
