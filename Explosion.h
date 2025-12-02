#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Explosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Explosion(QGraphicsItem *parent = nullptr);

public slots:
    void end();
};

#endif // EXPLOSION_H
