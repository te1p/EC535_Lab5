#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>

// Player represents the user's ship and exposes movement + shooting actions.
class Player : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Player(QGraphicsItem *parent = nullptr);  // loads sprite + sets initial state
    void shoot(int bulletSpeed = 10);         // creates a bullet moving upward
    void moveLeft();                          // moves player left within bounds
    void moveRight();                         // moves player right within bounds
};

#endif // PLAYER_H
