#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QObject>

//Temporary explosion animation that deletes itself after displaying.
class Explosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Explosion(QGraphicsItem *parent = nullptr);

public slots:
    void end();   //Removes the explosion from the scene
};

#endif // EXPLOSION_H