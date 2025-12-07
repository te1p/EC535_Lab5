#include "Enemy.h"
#include "Bullet.h"
#include <QGraphicsScene>
#include <QtMath>
#include <stdlib.h> // rand() for randomness

Enemy::Enemy(int speedMultiplier, int health, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), speed(5 * speedMultiplier), timeAlive(0), enemyHealth(health) {
  // Screen height is 272. Enemy height is 45.
  // We want to spawn fully inside [0, 272].
  // Max safe Y = 272 - 45 = 227.
  startY = rand() % 228;
  setPos(-60, startY);
}

void Enemy::commonMove() {
  if (x() > 520) {
    if (scene())
      scene()->removeItem(this);
    delete this;
  }
}

void Enemy::updateHealth(int damage)
{
    enemyHealth -= damage;
}

int Enemy::getHealth()
{
    return enemyHealth;
}

Asteroid::Asteroid(int speedMultiplier, int health, QGraphicsItem *parent)
    : Enemy(speedMultiplier, health ,parent) {
  setPixmap(QPixmap(":/assets/assets/Asteroid.png")
                .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Asteroid::move() {
  timeAlive++;
  setPos(x() + speed, y());
  commonMove();
}

StraightShip::StraightShip(int speedMultiplier, int health, QGraphicsItem *parent)
    : Enemy(speedMultiplier, health, parent) {
  setPixmap(QPixmap(":/assets/assets/Enemy.png")
                .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void StraightShip::move() {
  timeAlive++;
  setPos(x() + speed, y());

  tryShoot();
  commonMove();
}

void StraightShip::tryShoot() {
  if (rand() % 100 < 2) {
    int bulletSpeed = 10 * (speed / 5);
    Bullet *bullet = new Bullet(Bullet::EnemyBullet, bulletSpeed);
    bullet->setPos(x() + pixmap().width(),
                   y() + pixmap().height() / 2 - bullet->pixmap().height() / 2);

    if (scene())
      scene()->addItem(bullet);
  }
}

SineShip::SineShip(int speedMultiplier, int health, QGraphicsItem *parent)
    : Enemy(speedMultiplier, health, parent) {
  setPixmap(QPixmap(":/assets/assets/Enemy.png")
                .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));

  // Recalculate startY for SineShip to stay on screen during wave movement
  // Wave amplitude is 50. Buffer needed: 50 top, 50 bottom.
  // Min Y = 50. Max Y = 272 - 45 - 50 = 177.
  startY = (rand() % 128) + 50;
  setPos(x(), startY);
}

void SineShip::move() {
  timeAlive++;
  double newY = startY + 50 * qSin(0.15 * timeAlive);
  setPos(x() + speed, newY);

  tryShoot();
  commonMove();
}

void SineShip::tryShoot() {
  // Same shooting logic as StraightShip
  if (rand() % 100 < 2) {
    int bulletSpeed = 10 * (speed / 5);
    Bullet *bullet = new Bullet(Bullet::EnemyBullet, bulletSpeed);

    bullet->setPos(x() + pixmap().width(),
                   y() + pixmap().height() / 2 - bullet->pixmap().height() / 2);

    if (scene())
      scene()->addItem(bullet);
  }
}
