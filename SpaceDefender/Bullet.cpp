#include "bullet.h"
#include <QBrush>
#include <QGraphicsScene>

// Creates a bullet (player or enemy) and assigns its speed + pixmap.
Bullet::Bullet(Type type, int speed, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), bulletType(type), speed(speed)
{
    // small bullet sprite
    setPixmap(QPixmap(":/assets/assets/Bullet.png")
                  .scaled(10, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // (EnemyBullet may use different behavior in move(), nothing special here)
}

void Bullet::move()
{
    if (bulletType == PlayerBullet) {
        // player bullets travel upward
        setPos(x() - speed, y());

        // remove when off-screen
        if (y() + pixmap().width() < 0) {
            if (scene()) scene()->removeItem(this);
            delete this;
        }
    }
    else {
        // enemy bullets travel downward
        setPos(x() + speed, y());

        // remove when off-screen
        if (x() > 520) {
            if (scene()) scene()->removeItem(this);
            delete this;
        }
    }
}
