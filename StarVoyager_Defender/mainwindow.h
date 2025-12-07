#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsView>
#include "GameScene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//MainWindow controls the home menu UI and launches the GameScene.
//It also manages the credits overlay panel triggered from Settings.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Menu button actions
    void on_playButton_clicked();       //opens the game
    void on_settingsButton_clicked();   //shows credits panel
    void on_exitButton_clicked();       //exits application

private:
    Ui::MainWindow *ui;

    QLabel *logoLabel = nullptr;        //game title/logo display
    QLabel *bgLabel;                    //background

    //GameScene connection (menu opens game in a separate QGraphicsView)
    QGraphicsView *view = nullptr;
    GameScene     *scene = nullptr;

    //Credits panel elements (shown when Settings is pressed)
    QWidget     *creditsPanel = nullptr;
    QPushButton *creditsBackButton = nullptr;
    QLabel      *creditsTitle = nullptr;
    QLabel      *creditsBody = nullptr;

    //Show/hide credits overlay and toggle menu interactivity
    void showCreditsPanel();
    void hideCreditsPanel();
};

#endif // MAINWINDOW_H