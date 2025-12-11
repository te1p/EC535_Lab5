#include "Player.h"
#include "Bullet.h"
#include <QBrush>
#include <QGraphicsScene>

// Constructor: Initializes player sprite
// Position is set by GameScene after creation
Player::Player(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    // Load and scale player sprite to 50x50 pixels
    setPixmap(QPixmap(":/assets/assets/Player.png")
                  .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// Fires a bullet to the right from player's position
// bulletSpeed: horizontal speed of the bullet in pixels per frame
void Player::shoot(int bulletSpeed) {
    // Create player bullet moving right
    Bullet *bullet = new Bullet(Bullet::PlayerBullet, bulletSpeed);
    
    // Spawn bullet at right edge of player, vertically centered
    bullet->setPos(x() + pixmap().width(),
                   y() + pixmap().height() / 2 - bullet->pixmap().height() / 2);
    scene()->addItem(bullet);
}

// Moves player left by 20 pixels with boundary check
void Player::moveLeft() {
    if (x() > 0)
        setPos(x() - 20, y());
}

// Moves player right by 20 pixels with screen boundary check (480px wide)
void Player::moveRight() {
    if (x() + pixmap().width() < 480)
        setPos(x() + 20, y());
}

// Moves player up by 20 pixels with screen boundary check
void Player::moveUp() {
    if (y() > 0)
        setPos(x(), y() - 20);
}

// Moves player down by 20 pixels with screen boundary check (272px high)
void Player::moveDown() {
    if (y() + pixmap().height() < 272)
        setPos(x(), y() + 20);
}
