#include "bullet.h"
#include <QBrush>
#include <QGraphicsScene>

Bullet::Bullet(Type type, int speed, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), bulletType(type), speed(speed) {
  setPixmap(QPixmap(":/assets/assets/Bullet.png")
                .scaled(10, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));

  if (type == EnemyBullet) {
  }
}

void Bullet::move() {
  if (bulletType == PlayerBullet) {
    setPos(x(), y() - speed);
    if (y() + pixmap().height() < 0) {
      if (scene())
        scene()->removeItem(this);
      delete this;
    }
  } else {
    setPos(x(), y() + speed);
    if (y() > 600) { // Off screen bottom
      if (scene())
        scene()->removeItem(this);
      delete this;
    }
  }
}
