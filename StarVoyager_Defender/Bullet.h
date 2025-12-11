#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QObject>

// Bullet class: Represents projectiles fired by player and enemies
// Inherits from QObject (for Qt signals/slots) and QGraphicsPixmapItem (for rendering)
class Bullet : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    
public:
    // Bullet type enum: distinguishes player bullets from enemy bullets
    enum Type { PlayerBullet, EnemyBullet };
    
    // Constructor: Creates bullet with specified type and speed
    // Default speed is 10 pixels per frame
    Bullet(Type type, int speed = 10, QGraphicsItem *parent = nullptr);
    
    // Getter: Returns the type of bullet (player or enemy)
    Type getType() const { return bulletType; }
    
    // Updates bullet position each frame based on type and speed
    void move();
    
private:
    Type bulletType;  // Identifies who fired the bullet
    int speed;        // Horizontal movement speed in pixels per frame
};

#endif // BULLET_H
