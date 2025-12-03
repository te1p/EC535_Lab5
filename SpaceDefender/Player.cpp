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
    bullet->setPos(x() + pixmap().width() / 2 - bullet->pixmap().width() / 2,
                   y());
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
