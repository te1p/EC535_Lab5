#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "score.h"
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scores(new Score)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scores;
}

void MainWindow::on_startButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_menuButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_clicked()
{
    scores->incrementPoints(10);
}


void MainWindow::on_pushButton_3_clicked()
{
    scores->saveScore();
}


void MainWindow::on_pushButton_4_clicked()
{
    scores->cleanScore();
}


void MainWindow::on_pushButton_5_clicked()
{
    scores->loadScore();
    QString str;
    ui->label->setText(str.setNum(scores->getHighscore()));
}

