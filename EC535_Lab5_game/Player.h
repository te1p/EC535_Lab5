#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Player : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT

public:
  Player(QGraphicsItem *parent = nullptr);
  void shoot(int bulletSpeed = 10);
  void moveLeft();
  void moveRight();
};

#endif // PLAYER_H
