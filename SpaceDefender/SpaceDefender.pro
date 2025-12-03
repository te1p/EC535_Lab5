QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    GameScene.cpp \
    Player.cpp \
    Enemy.cpp \
    Bullet.cpp \
    Explosion.cpp \
    wave.cpp

HEADERS += \
    mainwindow.h \
    GameScene.h \
    Player.h \
    Enemy.h \
    Bullet.h \
    Explosion.h \
    wave.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    assets/Background_WIP.png \
    assets/Long_Background_WIP.png

RESOURCES += \
    resources.qrc
