QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StarVoyager
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    GameScene.cpp \
    Player.cpp \
    Enemy.cpp \
    Bullet.cpp \
    Explosion.cpp

HEADERS += \
    MainWindow.h \
    GameScene.h \
    Player.h \
    Enemy.h \
    Bullet.h \
    Explosion.h

# Default to 800x480, can be adjusted
DEFINES += SCREEN_WIDTH=800
DEFINES += SCREEN_HEIGHT=480
