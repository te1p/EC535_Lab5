#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set fixed size for the window
    setFixedSize(800, 480);

    // Create the scene
    scene = new GameScene(this);
    scene->setSceneRect(0, 0, 800, 480);

    // Create the view
    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(800, 480);
    
    setCentralWidget(view);
    
    // Start the game
    scene->start();
}

MainWindow::~MainWindow()
{
}
