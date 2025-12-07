#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QObject>

// Bullet object used by both the player and enemies.
class Bullet : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    enum Type { PlayerBullet, EnemyBullet };     //Identifies who fired it
    Bullet(Type type, int speed = 10, QGraphicsItem *parent = nullptr);

    Type getType() const { return bulletType; }  //Accessor
    void move(); //Called each frame

private:
    Type bulletType;
    int speed;    //Vertical movement speed
};

#endif // BULLET_H