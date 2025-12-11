#include "Explosion.h"
#include <QTimer>
#include <QGraphicsScene>

// Constructor: Creates explosion effect at collision point
// Automatically removes itself after display duration
Explosion::Explosion(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    // Load and scale explosion sprite to 50x50 pixels
    setPixmap(QPixmap(":/assets/assets/Explosion.png")
                  .scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    // Set high Z-value to render explosion above all other game objects
    setZValue(100);
    
    // Set timer to automatically remove explosion after 500ms
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(end()));
    timer->start(500);
}

// Removes explosion from scene and cleans up memory
void Explosion::end()
{
    if (scene()) 
        scene()->removeItem(this);
    delete this;
}
