#include "Enemy.h"
#include "Bullet.h"
#include <QBrush>
#include <QGraphicsScene>
#include <QtMath>
#include <stdlib.h> // rand()

Enemy::Enemy(Type type, int speedMultiplier, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), enemyType(type), speed(5 * speedMultiplier),
      timeAlive(0) {
  if (type == Asteroid) {
    setPixmap(
        QPixmap("assets/Asteroid.png")
            .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  } else if (type == StraightShip) {
    setPixmap(
        QPixmap("assets/Enemy.png")
            .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  } else if (type == SineShip) {
    setPixmap(
        QPixmap("assets/Enemy.png")
            .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // Maybe tint it or rotate it to distinguish?
  }

  // Random start X within screen bounds (0 to 750)
  startX = rand() % 750;
  setPos(startX, -100); // Start slightly off screen
}

Enemy::Type Enemy::getType() const { return enemyType; }

void Enemy::move() {
  timeAlive++;

  if (enemyType == Asteroid || enemyType == StraightShip) {
    setPos(x(), y() + speed);
  } else if (enemyType == SineShip) {
    // Sine wave movement
    // x = startX + amplitude * sin(frequency * time)
    double newX = startX + 100 * qSin(0.1 * timeAlive);
    setPos(newX, y() + speed);
  }

  if (y() > 600) { // Off screen
    if (scene())
      scene()->removeItem(this);
    delete this;
    return;
  }

  // Shooting logic (Random chance)
  // Only ships shoot, not asteroids
  if (enemyType != Asteroid) {
    if (rand() % 100 < 2) {
      int bulletSpeed = 10 * (speed / 5);
      Bullet *bullet = new Bullet(Bullet::EnemyBullet, bulletSpeed);
      bullet->setPos(x() + pixmap().width() / 2 - bullet->pixmap().width() / 2,
                     y() + pixmap().height());
      if (scene())
        scene()->addItem(bullet);
    }
  }
}
