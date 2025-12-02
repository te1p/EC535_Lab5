#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Bullet : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT

public:
  enum Type { PlayerBullet, EnemyBullet };
  Bullet(Type type, int speed = 10, QGraphicsItem *parent = nullptr);
  Type getType() const { return bulletType; }
  void move();

private:
  Type bulletType;
  int speed;
};

#endif // BULLET_H
