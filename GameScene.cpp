#include "GameScene.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Explosion.h"
#include <QFile>
#include <QFont>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTextStream>
#include <stdlib.h> // rand

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), leftPressed(false), rightPressed(false),
      spacePressed(false), isPaused(false), gameStarted(false), score(0),
      highScore(0), health(1000), gamePhase(0) {
  loadHighScore();

  // Background
  setBackgroundBrush(QBrush(Qt::black));
  // QGraphicsPixmapItem *bg = new QGraphicsPixmapItem(
  //     QPixmap("assets/Background_WIP.png")
  //         .scaled(800, 480, Qt::IgnoreAspectRatio,
  //         Qt::SmoothTransformation));
  // addItem(bg);
  // bg->setZValue(-1); // Ensure it's behind everything

  // Logo
  logoItem = new QGraphicsPixmapItem(
      QPixmap("assets/Logo.png")
          .scaled(600, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  logoItem->setPos(400 - logoItem->pixmap().width() / 2,
                   50); // Moved up slightly to fit Start button
  addItem(logoItem);

  // Start Image
  startImage = new QGraphicsPixmapItem(
      QPixmap("assets/Start.png")
          .scaled(200, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  startImage->setPos(400 - startImage->pixmap().width() / 2, 350); // Below logo
  addItem(startImage);

  // Player
  player = new Player();
  addItem(player);
  player->setPos(400 - player->pixmap().width() / 2, 400); // Bottom center
  player->setFlag(QGraphicsItem::ItemIsFocusable);
  player->setFocus();
  player->hide(); // Hide initially

  // UI
  scoreText = new QGraphicsTextItem();
  scoreText->setPlainText("Score: 0");
  scoreText->setDefaultTextColor(Qt::white);
  scoreText->setFont(QFont("times", 16));
  addItem(scoreText);
  scoreText->setPos(10, 10);
  scoreText->hide();

  highScoreText = new QGraphicsTextItem();
  highScoreText->setPlainText(QString("High Score: ") +
                              QString::number(highScore));
  highScoreText->setDefaultTextColor(Qt::red);
  highScoreText->setFont(QFont("times", 20, QFont::Bold));
  addItem(highScoreText);
  // Logo is at (250, 100) with size approx 300x200 (center ~400, 200)
  // Start image is at (300, 350)
  // Let's put high score at (320, 310) to be in between
  highScoreText->setPos(320, 310);

  healthText = new QGraphicsTextItem();
  healthText->setPlainText("Health: 1000");
  healthText->setDefaultTextColor(Qt::red);
  healthText->setFont(QFont("times", 16));
  addItem(healthText);
  healthText->setPos(10, 40);
  healthText->hide();

  // Pause Image
  pauseImage = new QGraphicsPixmapItem(
      QPixmap("assets/Pause.png")
          .scaled(400, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  pauseImage->setPos(400 - pauseImage->pixmap().width() / 2,
                     240 - pauseImage->pixmap().height() / 2); // Centered
  pauseImage->setZValue(200);                                  // Ensure on top
  pauseImage->hide();
  addItem(pauseImage);

  // Timers
  gameTimer = new QTimer(this);
  connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateGame()));

  spawnTimer = new QTimer(this);
  connect(spawnTimer, SIGNAL(timeout()), this, SLOT(spawnEnemy()));
}

void GameScene::start() {
  // Timers are started in keyPressEvent upon first input
}

void GameScene::keyPressEvent(QKeyEvent *event) {
  if (!gameStarted) {
    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_Right) {
      gameStarted = true;
      logoItem->hide();
      startImage->hide();
      highScoreText->hide(); // Hide high score during game
      player->show();
      scoreText->show();
      healthText->show();
      gameTimer->start(50);
      spawnTimer->start(2000);
      return;
    }
  }

  if (isPaused) {
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_Space || event->key() == Qt::Key_Escape) {
      isPaused = false;
      gameTimer->start();
      spawnTimer->start();
      pauseImage->hide();

      // If Escape or Space, return to avoid re-pausing or immediate shooting
      if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Space)
        return;
      // If Left/Right, fall through to set flags so movement starts immediately
    } else {
      return; // Ignore other keys while paused
    }
  }

  if (event->key() == Qt::Key_Left) {
    leftPressed = true;
  } else if (event->key() == Qt::Key_Right) {
    rightPressed = true;
  } else if (event->key() == Qt::Key_Space) {
    // Bullet speed increases with phase
    int bulletSpeed = 20;
    if (gamePhase >= 3)
      bulletSpeed = 40;
    player->shoot(bulletSpeed);
  } else if (event->key() == Qt::Key_Escape) {
    isPaused = true;
    gameTimer->stop();
    spawnTimer->stop();
    pauseImage->show();
  }
}

void GameScene::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat())
    return;

  if (event->key() == Qt::Key_Left) {
    leftPressed = false;
  } else if (event->key() == Qt::Key_Right) {
    rightPressed = false;
  }
}

void GameScene::spawnEnemy() {
  // Progression Logic for Spawning
  // Phase 0 (0-1000): Asteroids only
  // Phase 1 (1000-2000): + Straight
  // Phase 2 (2000-3000): + Sine
  // Phase 3 (3000+): Speed up (handled in creation)

  int speedMult = (gamePhase >= 3) ? 2 : 1;

  Enemy::Type type = Enemy::Asteroid;

  int r = rand() % 100;

  if (gamePhase == 0) {
    type = Enemy::Asteroid;
  } else if (gamePhase == 1) {
    // 50% Asteroid, 50% Straight
    if (r < 50)
      type = Enemy::Asteroid;
    else
      type = Enemy::StraightShip;
  } else {
    // 33% each
    if (r < 33)
      type = Enemy::Asteroid;
    else if (r < 66)
      type = Enemy::StraightShip;
    else
      type = Enemy::SineShip;
  }

  Enemy *enemy = new Enemy(type, speedMult);
  addItem(enemy);
}

void GameScene::updateGame() {
  // Move Player (Continuous)
  if (leftPressed)
    player->moveLeft();
  if (rightPressed)
    player->moveRight();

  // Move Enemies and Bullets
  QList<QGraphicsItem *> sceneItems = items();
  for (int i = 0, n = sceneItems.size(); i < n; ++i) {
    if (typeid(*(sceneItems[i])) == typeid(Enemy)) {
      Enemy *enemy = dynamic_cast<Enemy *>(sceneItems[i]);
      enemy->move();
    } else if (typeid(*(sceneItems[i])) == typeid(Bullet)) {
      Bullet *bullet = dynamic_cast<Bullet *>(sceneItems[i]);
      bullet->move();
    }
  }

  // Check collisions
  checkCollisions();

  // Update UI
  scoreText->setPlainText(QString("Score: ") + QString::number(score));
  healthText->setPlainText(QString("Health: ") + QString::number(health));

  checkProgression();

  if (health <= 0) {
    gameOver();
  }
}

void GameScene::checkProgression() {
  if (score < 1000)
    gamePhase = 0;
  else if (score < 2000)
    gamePhase = 1;
  else if (score < 3000)
    gamePhase = 2;
  else
    gamePhase = 3;
}

void GameScene::checkCollisions() {
  QList<QGraphicsItem *> itemsList = items();
  QList<QGraphicsItem *> itemsToRemove;

  for (QGraphicsItem *item : itemsList) {
    if (itemsToRemove.contains(item))
      continue;

    // Check Player Collisions
    if (typeid(*item) == typeid(Player)) {
      QList<QGraphicsItem *> colliding_items = item->collidingItems();
      for (QGraphicsItem *other : colliding_items) {
        if (itemsToRemove.contains(other))
          continue;

        if (typeid(*other) == typeid(Enemy)) {
          Enemy *e = dynamic_cast<Enemy *>(other);
          if (e->getType() == Enemy::Asteroid)
            updateHealth(-50);
          else
            updateHealth(-100);

          // Explosion
          Explosion *explosion = new Explosion();
          explosion->setPos(e->x(), e->y());
          addItem(explosion);

          itemsToRemove.append(e);
        } else if (typeid(*other) == typeid(Bullet)) {
          Bullet *b = dynamic_cast<Bullet *>(other);
          if (b->getType() == Bullet::EnemyBullet) {
            updateHealth(-5);

            // Explosion
            Explosion *explosion = new Explosion();
            explosion->setPos(player->x(),
                              player->y()); // Explosion on player? Or bullet?
            // Let's put it on the bullet position or player center
            explosion->setPos(b->x(), b->y());
            addItem(explosion);

            itemsToRemove.append(b);
          }
        }
      }
    }
    // Check Player Bullet Collisions
    else if (typeid(*item) == typeid(Bullet)) {
      Bullet *b = dynamic_cast<Bullet *>(item);
      if (b->getType() == Bullet::PlayerBullet) {
        QList<QGraphicsItem *> colliding_items = b->collidingItems();
        for (QGraphicsItem *other : colliding_items) {
          if (itemsToRemove.contains(other))
            continue;

          if (typeid(*other) == typeid(Enemy)) {
            Enemy *e = dynamic_cast<Enemy *>(other);
            if (e->getType() == Enemy::Asteroid)
              updateScore(50);
            else
              updateScore(100);

            // Explosion
            Explosion *explosion = new Explosion();
            explosion->setPos(e->x(), e->y());
            addItem(explosion);

            itemsToRemove.append(e);
            itemsToRemove.append(b); // Remove bullet too
            break;                   // Bullet hits one enemy
          }
        }
      }
    }
  }

  // Remove items safely
  for (QGraphicsItem *item : itemsToRemove) {
    if (item->scene() == this) {
      removeItem(item);
      delete item;
    }
  }
}

void GameScene::updateScore(int points) { score += points; }

void GameScene::updateHealth(int damage) { health += damage; }

void GameScene::gameOver() {
  // Simple Game Over
  gameTimer->stop();
  spawnTimer->stop();
  QGraphicsTextItem *over = new QGraphicsTextItem("GAME OVER");
  over->setFont(QFont("times", 30));
  over->setDefaultTextColor(Qt::red);
  over->setPos(300, 150);
  addItem(over);

  if (score > highScore) {
    highScore = score;
    saveHighScore();
    QGraphicsTextItem *newHigh = new QGraphicsTextItem("NEW HIGH SCORE!");
    newHigh->setFont(QFont("times", 24));
    newHigh->setDefaultTextColor(Qt::green);
    newHigh->setPos(280, 250);
    addItem(newHigh);

    QGraphicsTextItem *yourScore =
        new QGraphicsTextItem("Your Score: " + QString::number(score));
    yourScore->setFont(QFont("times", 20));
    yourScore->setDefaultTextColor(Qt::white);
    yourScore->setPos(320, 300);
    addItem(yourScore);

  } else {
    QGraphicsTextItem *highScoreMsg =
        new QGraphicsTextItem("High Score: " + QString::number(highScore));
    highScoreMsg->setFont(QFont("times", 20));
    highScoreMsg->setDefaultTextColor(Qt::yellow);
    highScoreMsg->setPos(320, 250);
    addItem(highScoreMsg);

    QGraphicsTextItem *yourScore =
        new QGraphicsTextItem("Your Score: " + QString::number(score));
    yourScore->setFont(QFont("times", 20));
    yourScore->setDefaultTextColor(Qt::white);
    yourScore->setPos(320, 300);
    addItem(yourScore);
  }
}

void GameScene::loadHighScore() {
  QFile file("highscore.txt");
  if (file.open(QIODevice::ReadOnly)) {
    QTextStream in(&file);
    in >> highScore;
    file.close();
  } else {
    highScore = 0;
  }
}

void GameScene::saveHighScore() {
  QFile file("highscore.txt");
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream out(&file);
    out << highScore;
    file.close();
  }
}
