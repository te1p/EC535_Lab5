#include "Player.h"
#include "Bullet.h"
#include <QBrush>
#include <QGraphicsScene>

Player::Player(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    // Load player sprite (scaled once on creation)
    setPixmap(QPixmap(":/assets/assets/Player.png")
                  .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Position is assigned by the GameScene
}

void Player::shoot(int bulletSpeed) {
    // Create a new upward-moving bullet from the player's center
    Bullet *bullet = new Bullet(Bullet::PlayerBullet, bulletSpeed);
    bullet->setPos(x() + 45,
                   y() + pixmap().height() / 2 - bullet->pixmap().height() / 2);
    scene()->addItem(bullet);
}

void Player::moveLeft() {
    // Move left with boundary check
    if (x() > 0)
        setPos(x() - 20, y());
}

void Player::moveRight() {
    // Move right with screen-edge boundary (480px wide scene)
    if (x() + pixmap().width() < 480)
        setPos(x() + 20, y());
}

void Player::moveUp()
{
    // Move up with screen-edge boundary (272px wide scene)
    if (y() > 0)
        setPos(x(), y() - 20);
}

void Player::moveDown()
{
    // Move down with screen-edge boundary (480px wide scene)
    if (y() + pixmap().height() < 272)
        setPos(x(), y() + 20);
}
