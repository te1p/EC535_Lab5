#include "Enemy.h"
#include "Bullet.h"
#include <QBrush>
#include <QGraphicsScene>
#include <QtMath>
#include <stdlib.h> // rand() for randomness

// Constructor: sets enemy sprite, movement speed, and starting position
Enemy::Enemy(Type type, int speedMultiplier, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), enemyType(type), speed(5 * speedMultiplier),
    timeAlive(0) {

    // Choose sprite depending on enemy type
    if (type == Asteroid) {
        setPixmap(
            QPixmap(":/assets/assets/Asteroid.png")
                .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else if (type == StraightShip) {
        setPixmap(
            QPixmap(":/assets/assets/Enemy.png")
                .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else if (type == SineShip) {
        setPixmap(
            QPixmap(":/assets/assets/enemy.png")
                .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        // Could visually distinguish sine ship
    }

    // Random start X within LCD screen width (approx 480)
    startY = (rand() % 257) + 10;       // enemy vertical spawn position
    setPos(-60, startY);         // appear slightly off-screen at the top
}

Enemy::Type Enemy::getType() const { return enemyType; }

// Handles movement each frame (called in GameScene::updateGame)
void Enemy::move() {
    timeAlive++;

    if (enemyType == Asteroid || enemyType == StraightShip) {
        // Basic downward movement
        setPos(x() + speed, y());

    }
    else if (enemyType == SineShip) {
        // Horizontal sine-wave movement formula
        double newY = startY + 50 * qSin(0.15 * timeAlive);
        setPos(x() + speed, newY);
    }

    // Delete enemy once it leaves bottom of the screen
    if (x() > 520) { // Off screen
        if (scene())
            scene()->removeItem(this);
        delete this;
        return;
    }

    // Random shooting chance for ships (asteroids cannot shoot)
    if (enemyType != Asteroid) {
        if (rand() % 100 < 2) {  // ~2% chance each frame
            int bulletSpeed = 10 * (speed / 5);   // scale bullet speed with enemy speed
            Bullet *bullet = new Bullet(Bullet::EnemyBullet, bulletSpeed);

            // Position bullet at center-bottom of enemy sprite
            bullet->setPos(x() + pixmap().width() / 2 - bullet->pixmap().width() / 2,
                           y() + pixmap().height());

            if (scene())
                scene()->addItem(bullet);
        }
    }
}
