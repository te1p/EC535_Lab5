#include "Enemy.h"
#include "Bullet.h"
#include <QGraphicsScene>
#include <QtMath>
#include <stdlib.h>

// Enemy base class constructor
// speedMultiplier: scales base speed (5 pixels/frame)
// health: enemy hit points
Enemy::Enemy(int speedMultiplier, int health, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), speed(5 * speedMultiplier), timeAlive(0), enemyHealth(health) {
    // Calculate safe spawn Y position to keep enemy on screen
    // Screen height: 272, Enemy height: 45
    // Max safe Y = 272 - 45 = 227
    startY = rand() % 228;
    setPos(480 + 60, startY); // Spawn off-screen to the right
}

// Removes enemy when it moves off-screen to the left
void Enemy::commonMove() {
    if (x() < -60) {
        if (scene())
            scene()->removeItem(this);
        delete this;
    }
}

// Reduces enemy health by damage amount
void Enemy::updateHealth(int damage) {
    enemyHealth -= damage;
}

// Returns current enemy health
int Enemy::getHealth() {
    return enemyHealth;
}

// ========== ASTEROID CLASS ==========

// Asteroid constructor: non-shooting enemy
Asteroid::Asteroid(int speedMultiplier, int health, QGraphicsItem *parent)
    : Enemy(speedMultiplier, health, parent) {
    // Load and scale asteroid sprite to 45x45 pixels
    setPixmap(QPixmap(":/assets/assets/Asteroid.png")
                  .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// Asteroid movement: straight horizontal path to the left
void Asteroid::move() {
    timeAlive++;
    setPos(x() - speed, y());
    commonMove();
}

// ========== STRAIGHTSHIP CLASS ==========

// StraightShip constructor: enemy that moves straight and shoots
StraightShip::StraightShip(int speedMultiplier, int health, QGraphicsItem *parent)
    : Enemy(speedMultiplier, health, parent) {
    // Load and scale enemy ship sprite to 45x45 pixels
    setPixmap(QPixmap(":/assets/assets/Enemy.png")
                  .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// StraightShip movement: moves left and attempts to shoot
void StraightShip::move() {
    timeAlive++;
    setPos(x() - speed, y());
    tryShoot();
    commonMove();
}

// Randomly fires bullets toward player (2% chance per frame)
void StraightShip::tryShoot() {
    if (rand() % 100 < 2) {
        // Bullet speed scales with enemy speed
        int bulletSpeed = 10 * (speed / 5);
        Bullet *bullet = new Bullet(Bullet::EnemyBullet, bulletSpeed);
        
        // Spawn bullet at left edge of enemy, centered vertically
        bullet->setPos(x() - pixmap().width(),
                       y() + pixmap().height() / 2 - bullet->pixmap().height() / 2);
        if (scene())
            scene()->addItem(bullet);
    }
}

// ========== SINESHIP CLASS ==========

// SineShip constructor: enemy that moves in sine wave pattern and shoots
SineShip::SineShip(int speedMultiplier, int health, QGraphicsItem *parent)
    : Enemy(speedMultiplier, health, parent) {
    // Load and scale enemy ship sprite to 45x45 pixels
    setPixmap(QPixmap(":/assets/assets/Enemy.png")
                  .scaled(45, 45, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    // Recalculate startY to keep sine wave on screen
    // Wave amplitude: 50 pixels (±50 from center)
    // Safe range: Y between 50 and 177 (272 - 45 - 50)
    startY = (rand() % 128) + 50;
    setPos(x(), startY);
}

// SineShip movement: sine wave pattern horizontally to the left
void SineShip::move() {
    timeAlive++;
    
    // Calculate Y position using sine function
    // Amplitude: 50, Frequency: 0.15
    double newY = startY + 50 * qSin(0.15 * timeAlive);
    setPos(x() - speed, newY);
    
    tryShoot();
    commonMove();
}

// Randomly fires bullets toward player (2% chance per frame)
void SineShip::tryShoot() {
    if (rand() % 100 < 2) {
        // Bullet speed scales with enemy speed
        int bulletSpeed = 10 * (speed / 5);
        Bullet *bullet = new Bullet(Bullet::EnemyBullet, bulletSpeed);
        
        // Spawn bullet at left edge of enemy, centered vertically
        bullet->setPos(x() - pixmap().width(),
                       y() + pixmap().height() / 2 - bullet->pixmap().height() / 2);
        if (scene())
            scene()->addItem(bullet);
    }
}
