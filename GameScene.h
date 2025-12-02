#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Player.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>

class GameScene : public QGraphicsScene {
  Q_OBJECT

public:
  GameScene(QObject *parent = nullptr);
  void start();

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

private slots:
  void updateGame();
  void spawnEnemy();

private:
  Player *player;
  QTimer *gameTimer;
  QTimer *spawnTimer;

  bool leftPressed;
  bool rightPressed;
  bool spacePressed;
  bool isPaused;
  bool gameStarted;

  int score;
  int highScore;
  int health;
  int gamePhase; // 0: Start, 1: Asteroids, 2: +Straight, 3: +Sine, 4: Speed Up

  QGraphicsTextItem *scoreText;
  QGraphicsTextItem *highScoreText;
  QGraphicsTextItem *healthText;
  QGraphicsPixmapItem *pauseImage;
  QGraphicsPixmapItem *logoItem;
  QGraphicsPixmapItem *startImage;

  void updateScore(int points);
  void updateHealth(int damage);
  void checkCollisions();
  void checkProgression();
  void gameOver();
  void loadHighScore();
  void saveHighScore();
};

#endif // GAMESCENE_H
