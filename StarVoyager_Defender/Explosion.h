#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QGraphicsPixmapItem>
#include <QObject>

// Explosion class: Temporary visual effect shown on collisions
// Automatically removes itself after a brief display period (500ms)
class Explosion : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    
public:
    // Constructor: Creates explosion effect at specified position
    Explosion(QGraphicsItem *parent = nullptr);
    
public slots:
    // Removes explosion from scene and deletes object
    void end();
};

#endif // EXPLOSION_H
