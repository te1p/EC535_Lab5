#include "Player.h"
#include "Bullet.h"
#include <QBrush>
#include <QGraphicsScene>

Player::Player(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
  // Load sprite
  setPixmap(QPixmap(":/assets/assets/Player.png")
                .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

  // Start position will be set by scene
}

void Player::shoot(int bulletSpeed) {
  Bullet *bullet = new Bullet(Bullet::PlayerBullet, bulletSpeed);
  bullet->setPos(x() + pixmap().width() / 2 - bullet->pixmap().width() / 2,
                 y());
  scene()->addItem(bullet);
}

void Player::moveLeft() {
  if (x() > 0)
    setPos(x() - 20, y());
}

void Player::moveRight() {
  if (x() + pixmap().width() < 800) // Assuming 800 width
    setPos(x() + 20, y());
}
