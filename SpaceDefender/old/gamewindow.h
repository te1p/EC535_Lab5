#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QPushButton>

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

signals:
    void exitToMenu();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QTimer *timer;

    QPixmap background;
    QPixmap playerSprite;
    QPixmap enemySprite;
    QPixmap bulletSprite;

    QPushButton *backButton;

    int playerX = 0;
    int playerY = 0;

    int enemyX = 0;
    int enemyY = 0;

    int bulletX = 0;
    int bulletY = 0;
};

#endif // GAMEWINDOW_H
