#include "Bullet.h"
#include <QBrush>
#include <QGraphicsScene>

// Creates a bullet (player or enemy) and assigns its speed + pixmap.
Bullet::Bullet(Type type, int speed, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), bulletType(type), speed(speed)
{
    // small bullet sprite
    setPixmap(QPixmap(":/assets/assets/Bullet.png")
                  .scaled(10, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Bullet::move()
{
    if (bulletType == PlayerBullet) {
        // player bullets travel RIGHT
        setPos(x() + speed, y());
        // remove when off-screen RIGHT
        if (x() > 480) {  // FIXED: was checking < 0
            if (scene()) scene()->removeItem(this);
            delete this;
        }
    }
    else {
        // enemy bullets travel LEFT
        setPos(x() - speed, y());
        // remove when off-screen LEFT
        if (x() + pixmap().width() < 0) {  // FIXED: was > 520
            if (scene()) scene()->removeItem(this);
            delete this;
        }
    }
}