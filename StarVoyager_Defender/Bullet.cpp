#include "Bullet.h"
#include <QBrush>
#include <QGraphicsScene>

// Creates a bullet with specified type and speed
// type: PlayerBullet moves right, EnemyBullet moves left
// speed: pixels per frame movement
Bullet::Bullet(Type type, int speed, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), bulletType(type), speed(speed)
{
    // Load bullet sprite and scale to 10x20 pixels
    setPixmap(QPixmap(":/assets/assets/Bullet.png")
                  .scaled(10, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// Move bullet based on type and remove when off-screen
void Bullet::move()
{
    if (bulletType == PlayerBullet) {
        // Player bullets travel RIGHT
        setPos(x() + speed, y());
        
        // Remove when off-screen to the right
        if (x() > 480) {
            if (scene()) scene()->removeItem(this);
            delete this;
        }
    }
    else {
        // Enemy bullets travel LEFT
        setPos(x() - speed, y());
        
        // Remove when off-screen to the left
        if (x() + pixmap().width() < 0) {
            if (scene()) scene()->removeItem(this);
            delete this;
        }
    }
}
