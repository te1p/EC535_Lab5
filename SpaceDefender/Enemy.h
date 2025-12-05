#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>

// Enemy object (asteroid or ship) with different movement behaviors.
class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    enum Type { Asteroid, StraightShip, SineShip }; //Movement pattern / sprite type
    Enemy(Type type, int speedMultiplier = 1, QGraphicsItem *parent = nullptr);
    Type getType() const;

public slots:
    void move();               //Updates position each frame

private:
    Type enemyType;            //Determines movement logic
    int speed;                 //Vertical movement speed
    int startY;                //Starting horizontal position
    int timeAlive;             //Used for sine wave movement
};

#endif // ENEMY_H
