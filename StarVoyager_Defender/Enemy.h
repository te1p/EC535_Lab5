#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>

// Abstract base class for all enemy types in the game
// Defines common interface and shared properties for enemies
class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    
public:
    // Constructor: creates enemy with speed multiplier and health
    Enemy(int speedMultiplier = 1, int health = 1, QGraphicsItem *parent = nullptr);
    virtual ~Enemy() {}
    
    // Pure virtual functions - must be implemented by subclasses
    virtual int getScoreValue() const = 0;   // Points awarded when destroyed
    virtual int getDamageValue() const = 0;  // Damage dealt to player on collision
    
    // Health management
    virtual void updateHealth(int damage);   // Reduces enemy health
    virtual int getHealth();                 // Returns current health
    
public slots:
    // Pure virtual move function - defines enemy movement behavior
    virtual void move() = 0;
    
    // Common movement logic - checks bounds and removes off-screen enemies
    void commonMove();
    
protected:
    int speed;        // Horizontal movement speed (pixels per frame)
    int startY;       // Initial Y spawn position
    int timeAlive;    // Frame counter since spawn (used for animations)
    int enemyHealth;  // Current hit points
};

// ========== ENEMY SUBCLASSES ==========

// Asteroid: Basic enemy with straight movement, no shooting
class Asteroid : public Enemy {
    Q_OBJECT
    
public:
    Asteroid(int speedMultiplier = 1, int health = 1, QGraphicsItem *parent = nullptr);
    void move() override;                        // Straight horizontal movement
    int getScoreValue() const override { return 50; }   // Worth 50 points
    int getDamageValue() const override { return 50; }  // Deals 50 damage
};

// StraightShip: Enemy with straight movement and shooting capability
class StraightShip : public Enemy {
    Q_OBJECT
    
public:
    StraightShip(int speedMultiplier = 1, int health = 1, QGraphicsItem *parent = nullptr);
    void move() override;                         // Straight movement with shooting
    int getScoreValue() const override { return 100; }   // Worth 100 points
    int getDamageValue() const override { return 100; }  // Deals 100 damage
    void tryShoot();                              // Attempts to fire bullet
};

// SineShip: Enemy with sine wave movement and shooting capability
class SineShip : public Enemy {
    Q_OBJECT
    
public:
    SineShip(int speedMultiplier = 1, int health = 1, QGraphicsItem *parent = nullptr);
    void move() override;                         // Sine wave movement with shooting
    int getScoreValue() const override { return 100; }   // Worth 100 points
    int getDamageValue() const override { return 100; }  // Deals 100 damage
    void tryShoot();                              // Attempts to fire bullet
};

#endif // ENEMY_H
