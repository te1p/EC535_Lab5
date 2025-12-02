#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    enum Type { Asteroid, StraightShip, SineShip };
    Enemy(Type type, int speedMultiplier = 1, QGraphicsItem *parent = nullptr);
    Type getType() const;

public slots:
    void move();

private:
    Type enemyType;
    int speed;
    int startX;
    int timeAlive; // For sinewave calculation
};

#endif // ENEMY_H
