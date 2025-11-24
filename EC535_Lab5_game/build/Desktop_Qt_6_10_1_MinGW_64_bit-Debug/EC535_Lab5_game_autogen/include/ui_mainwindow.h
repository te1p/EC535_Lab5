/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *menuScreen;
    QPushButton *startButton;
    QWidget *gameOver;
    QPushButton *menuButton;
    QWidget *gameScreen;
    QPushButton *pushButton_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(480, 272);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("background-image:url(:/img/Background_WIP.png)"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("color:rgb(255, 170, 0)"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 0, 480, 272));
        menuScreen = new QWidget();
        menuScreen->setObjectName("menuScreen");
        menuScreen->setStyleSheet(QString::fromUtf8("background-image: url(:/img/Menu_placeholder.png);"));
        startButton = new QPushButton(menuScreen);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(190, 130, 93, 29));
        stackedWidget->addWidget(menuScreen);
        gameOver = new QWidget();
        gameOver->setObjectName("gameOver");
        gameOver->setStyleSheet(QString::fromUtf8("background-image: url(:/img/Game_over_screen.png);"));
        menuButton = new QPushButton(gameOver);
        menuButton->setObjectName("menuButton");
        menuButton->setGeometry(QRect(172, 170, 111, 29));
        stackedWidget->addWidget(gameOver);
        gameScreen = new QWidget();
        gameScreen->setObjectName("gameScreen");
        gameScreen->setStyleSheet(QString::fromUtf8("background-image: url(:/img/Background_WIP.png);"));
        pushButton_2 = new QPushButton(gameScreen);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(190, 80, 93, 29));
        stackedWidget->addWidget(gameScreen);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        menuButton->setText(QCoreApplication::translate("MainWindow", "Back to Menu", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "DIE", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
