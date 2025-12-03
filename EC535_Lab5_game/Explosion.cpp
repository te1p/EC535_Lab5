#include "Explosion.h"
#include <QTimer>
#include <QGraphicsScene>

Explosion::Explosion(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/assets/assets/Explosion.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setZValue(100); // Ensure it's on top
    
    // Remove after 500ms
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(end()));
    timer->start(500);
}

void Explosion::end()
{
    if (scene()) scene()->removeItem(this);
    delete this;
}
