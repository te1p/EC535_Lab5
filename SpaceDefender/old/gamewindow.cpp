#include "gamewindow.h"
#include <QPainter>
#include <QKeyEvent>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(480, 272);

    background.load(":/assets/Background_WIP.png");

    // Load sprites
    playerSprite.load(":/assets/1.png");
    enemySprite.load(":/assets/2.png");
    bulletSprite.load(":/assets/bullet.jpg");

    playerSprite = playerSprite.scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    enemySprite  = enemySprite.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    bulletSprite = bulletSprite.scaled(15, 35, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Initial positions
    playerX = width()/2 - playerSprite.width()/2;
    playerY = height() - playerSprite.height() - 10;

    enemyX = width()/2 - enemySprite.width()/2;
    enemyY = 20;

    bulletX = playerX + playerSprite.width()/2 - bulletSprite.width()/2;
    bulletY = playerY - bulletSprite.height();

    // BACK button
    backButton = new QPushButton("BACK", this);
    backButton->setGeometry(10, 10, 80, 40);
    backButton->setFocusPolicy(Qt::NoFocus);

    backButton->setStyleSheet(
        "QPushButton {"
        " background: rgba(0,0,0,120);"
        " color: white;"
        " border: 2px solid rgba(255,255,255,180);"
        " border-radius: 10px;"
        " font-size: 16px;"
        " padding: 5px;"
        "}"
        "QPushButton:hover { background: rgba(50,50,50,180); }"
        );

    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit exitToMenu();
        this->hide();
    });

    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]() {

        bulletY -= 5;
        if (bulletY < 0) {
            bulletY = playerY - bulletSprite.height();
            bulletX = playerX + playerSprite.width()/2 - bulletSprite.width()/2;
        }

        enemyY += 2;
        if (enemyY > height())
            enemyY = 0;

        update();
    });

    timer->start(16);
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), background);

    painter.drawPixmap(playerX, playerY,
                       playerSprite.width(), playerSprite.height(),
                       playerSprite);

    painter.drawPixmap(enemyX, enemyY,
                       enemySprite.width(), enemySprite.height(),
                       enemySprite);

    painter.drawPixmap(bulletX, bulletY,
                       bulletSprite.width(), bulletSprite.height(),
                       bulletSprite);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left && playerX > 0) {
        playerX -= 10;
        bulletX -= 10;
    }

    if (event->key() == Qt::Key_Right && playerX < width() - playerSprite.width()) {
        playerX += 10;
        bulletX += 10;
    }

    if (event->key() == Qt::Key_Escape) {
        emit exitToMenu();
        this->hide();
    }
}
