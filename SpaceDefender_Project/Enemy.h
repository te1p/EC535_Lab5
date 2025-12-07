#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsPixmapItem>
#include <QObject>

// Abstract Base Class for all enemies
class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(int speedMultiplier = 1, int health = 1, QGraphicsItem *parent = nullptr);
    virtual ~Enemy() {}
    virtual int getScoreValue() const = 0;
    virtual int getDamageValue() const = 0;
    virtual void updateHealth(int damage);
    virtual int getHealth();

public slots:
    // Virtual move method to be implemented by subclasses
    virtual void move() = 0;
    void commonMove(); // Shared logic like checking bounds

protected:
    int speed;
    int startY;
    int timeAlive;
    int enemyHealth;
};

// Subclasses
class Asteroid : public Enemy {
    Q_OBJECT
public:
    Asteroid(int speedMultiplier = 1, int health = 1, QGraphicsItem *parent = nullptr);
    void move() override;
    int getScoreValue() const override { return 50; }
    int getDamageValue() const override { return 50; }
};

class StraightShip : public Enemy {
    Q_OBJECT
public:
    StraightShip(int speedMultiplier = 1, int health = 1, QGraphicsItem *parent = nullptr);
    void move() override;
    int getScoreValue() const override { return 100; }
    int getDamageValue() const override { return 100; }
    void tryShoot();
};

class SineShip : public Enemy {
    Q_OBJECT
public:
    SineShip(int speedMultiplier = 1, int health = 1, QGraphicsItem *parent = nullptr);
    void move() override;
    int getScoreValue() const override { return 100; }
    int getDamageValue() const override { return 100; }
    void tryShoot();
};

#endif // ENEMY_H