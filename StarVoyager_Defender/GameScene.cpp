#include "GameScene.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Explosion.h"
#include <QDateTime>
#include <QFile>
#include <QFont>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTextStream>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "hardwareio.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QEventLoop>

// Constructor: Initializes all game state, UI elements, and hardware
GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), leftPressed(false), rightPressed(false),
      upPressed(false), downPressed(false), spacePressed(false),
      isPaused(false), gameStarted(false), score(0), highScore(0), health(1000),
      gamePhase(0), win(0), enemyNum(0) {

  // Load saved high score from file
  loadHighScore();

  // Initialize GPIO hardware (buttons and LEDs)
  HardwareIO::initButtons();            
  HardwareIO::initLEDs();               
  HardwareIO::setLEDState(false, false, true);  // Start with green LED (full health)

  // Set fixed scene dimensions for BeagleBone LCD (480x272)
  setSceneRect(0, 0, 480, 272);

  // Background layer #1: Main sky/entry background
  bg1 = new QGraphicsPixmapItem(
      QPixmap(":/assets/assets/Background_entry.png")
          .scaled(480, 272, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  bg1->setZValue(-20);  // Behind all game objects
  addItem(bg1);

  // Background layer #2: Space background for transition effect
  bg2 = new QGraphicsPixmapItem(
      QPixmap(":/assets/assets/Background_middle.png")
          .scaled(480, 272, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  bg2->setZValue(-19);
  bg2->setOpacity(0.0);  // Starts invisible, fades in during gameplay
  addItem(bg2);

  // Win screen image (shown at game completion)
  winText = new QGraphicsPixmapItem(
      QPixmap(":/assets/assets/Win_Screen.png")
          .scaled(480, 272, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  winText->setZValue(-18);
  winText->setOpacity(0.0);  // Hidden until player wins
  addItem(winText);

  // Game logo displayed in top-right corner
  logoItem = new QGraphicsPixmapItem(
      QPixmap(":/assets/assets/Logo.png")
          .scaled(80, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  logoItem->setZValue(100);  // Above all game elements
  logoItem->setPos(480 - logoItem->pixmap().width() - 5, 5);
  addItem(logoItem);

  // Player spaceship - spawns on left side of screen
  player = new Player();
  addItem(player);
  player->setPos(20, 210);  // Bottom-left starting position
  player->setFlag(QGraphicsItem::ItemIsFocusable);
  player->setFocus();
  player->show();

  // Score display (top-left corner)
  scoreText = new QGraphicsTextItem();
  scoreText->setPlainText("Score: 0");
  scoreText->setDefaultTextColor(Qt::white);
  scoreText->setFont(QFont("times", 10));
  addItem(scoreText);
  scoreText->setPos(5, 5);
  scoreText->show();

  // High score display (center of screen)
  highScoreText = new QGraphicsTextItem();
  highScoreText->setPlainText("High Score: " + QString::number(highScore));
  highScoreText->setDefaultTextColor(Qt::red);
  highScoreText->setFont(QFont("times", 14, QFont::Bold));
  addItem(highScoreText);
  highScoreText->setPos(240 - 80, 120);

  // Health display (below score in top-left)
  healthText = new QGraphicsTextItem();
  healthText->setPlainText("Health: 1000");
  healthText->setDefaultTextColor(Qt::red);
  healthText->setFont(QFont("times", 10));
  addItem(healthText);
  healthText->setPos(5, 25);
  healthText->show();

  // Pause overlay image (center of screen when paused)
  pauseImage = new QGraphicsPixmapItem(
      QPixmap(":/assets/assets/Pause.png")
          .scaled(300, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  pauseImage->setPos(240 - pauseImage->pixmap().width() / 2,
                     136 - pauseImage->pixmap().height() / 2);
  pauseImage->setZValue(200);  // Above all elements
  pauseImage->hide();  // Hidden until pause is triggered
  addItem(pauseImage);

  // Exit button for pause menu
  menuButton = new QPushButton("Exit");
  menuButton->setFixedSize(150, 36);
  menuButton->setStyleSheet("QPushButton {"
                            "  color: white;"
                            "  background-color: rgba(0, 0, 0, 80);"
                            "  border: 3px solid #1ec8ff;"
                            "  border-radius: 40px;"
                            "  font-size: 18px;"
                            "  font-weight: bold;"
                            "  padding: 6px 12px;"
                            "  letter-spacing: 1px;"
                            "}"
                            "QPushButton:hover {"
                            "  border-color: #63e3ff;"
                            "  background-color: rgba(0, 0, 0, 120);"
                            "}"
                            "QPushButton:pressed {"
                            "  background-color: rgba(0, 0, 0, 160);"
                            "  border-color: #18b0e3;"
                            "}");
  menuButton->setGeometry(240 - 75, 175, 150, 36);  // Centered below pause image
  addWidget(menuButton);
  menuButton->hide();  // Hidden until pause menu is shown

  // Connect exit button to return to main menu
  connect(menuButton, &QPushButton::clicked, this,
          [this]() { emit exitToMenu(); });

  // Main game loop timer: handles movement, collisions, and UI updates
  gameTimer = new QTimer(this);
  connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateGame()));
  gameTimer->start(50);  // 20 FPS (50ms per frame)

  // Enemy spawn timer: wave-based spawning system
  spawnTimer = new QTimer(this);
  connect(spawnTimer, SIGNAL(timeout()), this, SLOT(startWave()));
  spawnTimer->start(2000);  // New wave every 2 seconds

  // Auto-shoot timer: player fires automatically
  autoShootTimer = new QTimer(this);
  connect(autoShootTimer, &QTimer::timeout, this, [this]() {
      if (!isPaused && health > 0) {
          // Bullet speed increases in later game phases
          int bulletSpeed = (gamePhase >= 3) ? 40 : 20;
          player->shoot(bulletSpeed);
      }
  });
  autoShootTimer->start(250);  // Fire 4 bullets per second

  // Record start time for intro fade animation
  startTime = QDateTime::currentMSecsSinceEpoch();

  // Hide high score text initially (shown on title screen)
  highScoreText->hide();
}

// Start function: Game begins on first input
void GameScene::start() {
  // Actual gameplay starts when player presses a button/key
}

// Handles keyboard input for player movement and game controls
void GameScene::keyPressEvent(QKeyEvent *event) {
  // Pause mode: only movement/action keys can unpause
  if (isPaused) {
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
        event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
        event->key() == Qt::Key_Space || event->key() == Qt::Key_Escape) {

      // Resume game timers and hide pause overlay
      isPaused = false;
      gameTimer->start();
      spawnTimer->start();
      autoShootTimer->start(250);
      pauseImage->hide();
      menuButton->hide();

      // Prevent immediate action when unpausing with space/escape
      if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Space)
        return;
    } else {
      return;  // Ignore other keys while paused
    }
  }

  // Player movement controls (arrow keys)
  if (event->key() == Qt::Key_Left) {
    leftPressed = true;
  } else if (event->key() == Qt::Key_Right) {
    rightPressed = true;
  } else if (event->key() == Qt::Key_Up) {
    upPressed = true;
  } else if (event->key() == Qt::Key_Down) {
    downPressed = true;
  } else if (event->key() == Qt::Key_Space) {
    // Manual shooting (bullet speed increases in late game phases)
    int bulletSpeed = (gamePhase >= 3) ? 40 : 20;
    player->shoot(bulletSpeed);
  } else if (event->key() == Qt::Key_Escape) {
    // Enter pause mode: stop all timers and show pause overlay
    isPaused = true;
    gameTimer->stop();
    spawnTimer->stop();
    autoShootTimer->stop();
    pauseImage->show();
    menuButton->show();
  }
}

// Handles keyboard key release events
void GameScene::keyReleaseEvent(QKeyEvent *event) {
  // Ignore auto-repeat key events
  if (event->isAutoRepeat())
    return;

  // Reset movement flags when keys are released
  if (event->key() == Qt::Key_Left)
    leftPressed = false;
  else if (event->key() == Qt::Key_Right)
    rightPressed = false;
  else if (event->key() == Qt::Key_Up)
    upPressed = false;
  else if (event->key() == Qt::Key_Down)
    downPressed = false;
}

// Spawns enemies based on current game phase (difficulty progression)
void GameScene::spawnEnemy() {
  // Speed multiplier increases in phase 2+
  int speedMult = (gamePhase >= 2) ? 2 : 1;
  int r = rand() % 100;  // Random number for enemy type selection
  
  // Enemy health values
  int astHealth = 3;
  int straHealth = 4;
  int sinHealth = 4;
  
  Enemy *enemy = nullptr;
  
  // Track enemy count for wave progression
  if (win < 1) {
      enemyNum++;
  }

  // Phase-based enemy distribution
  switch(gamePhase) {
  case 0:  // Early game: mostly asteroids, some straight ships
      if (r < 70) {
          enemy = new Asteroid(speedMult, astHealth);
      } else {
          enemy = new StraightShip(speedMult, straHealth);
      }
      break;

  case 1:  // Mid game: balanced mix with sine ships introduced
      if (r < 40) {
          enemy = new Asteroid(speedMult, astHealth);
      } else if ((r >= 40) && (r < 70)) {
          enemy = new StraightShip(speedMult, straHealth);
      } else {
          enemy = new SineShip(speedMult, sinHealth);
      }
      break;

  case 2:  // Late game: mostly shooting enemies
      if (r < 10) {
          enemy = new Asteroid(speedMult, astHealth);
      } else if ((r >= 10) && (r < 70)) {
          enemy = new StraightShip(speedMult, straHealth);
      } else {
          enemy = new SineShip(speedMult, sinHealth);
      }
      break;

  case 3:  // Win condition reached
      win++;
      gamePhase = 2;  // Return to phase 2 for endless mode
      break;
  }

  // Add spawned enemy to scene
  if (enemy)
      addItem(enemy);
}

// Wave-based spawning system: controls enemy spawning and phase progression
void GameScene::startWave() {
    // Normal wave spawning: spawn enemies until wave limit reached
    if ((win < 1) && (enemyNum < (15 + gamePhase * 5))) {
        spawnEnemy();
    } 
    // Wave complete: advance to next phase
    else if (enemyNum >= (15 + gamePhase * 5)) {
        enemyNum = 0;
        gamePhase++;
    } 
    // First win: show win screen for 5 seconds
    else if (win == 1) {
        winText->setOpacity(100.0);
        gameTimer->stop();
        spawnTimer->stop();

        // Display win screen using event loop
        QEventLoop loop;
        QTimer t;
        t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
        t.start(5000);  // 5 second display
        loop.exec();

        // Resume game for endless mode
        gameTimer->start();
        spawnTimer->start();
        winText->setOpacity(0.0);
        win++;
    } 
    // Endless mode after winning
    else if (win > 1) {
        gamePhase = 2;  // Keep difficulty at phase 2
        spawnEnemy();
    }
}

// Main game update loop: runs every frame (20 FPS)
void GameScene::updateGame() {
  // Handle continuous keyboard movement
  if (leftPressed)
    player->moveLeft();
  if (rightPressed)
    player->moveRight();
  if (upPressed)
    player->moveUp();
  if (downPressed)
    player->moveDown();

  // Handle GPIO button input (hardware controls)
  if (HardwareIO::readLeft())
    player->moveLeft();

  if (HardwareIO::readRight())
    player->moveRight();

  if (HardwareIO::readUp())
    player->moveUp();

  if (HardwareIO::readDown())
    player->moveDown();

  // Update LED health indicator based on current health
  if (health > 500)
    HardwareIO::setLEDState(false, false, true);  // Green LED (healthy)
  else if (health > 100)
    HardwareIO::setLEDState(false, true, false);  // Yellow LED (warning)
  else
    HardwareIO::setLEDState(true, false, false);  // Red LED (critical)

  // Update positions of all moving objects (enemies and bullets)
  QList<QGraphicsItem *> sceneItems = items();
  for (int i = 0, n = sceneItems.size(); i < n; ++i) {
    if (Enemy *e = dynamic_cast<Enemy *>(sceneItems[i])) {
      e->move();
    } else if (typeid(*(sceneItems[i])) == typeid(Bullet)) {
      dynamic_cast<Bullet *>(sceneItems[i])->move();
    }
  }

  // Check for collisions between all game objects
  checkCollisions();

  // Update HUD text elements
  scoreText->setPlainText("Score: " + QString::number(score));
  healthText->setPlainText("Health: " + QString::number(health));

  // Check if score threshold triggers phase progression
  checkProgression();

  // Update background fade animation
  qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - startTime;
  float t = (float)elapsed / 10000.0f;
  updateBackgroundFade(t);

  // Check for game over condition
  if (health <= 0)
    gameOver();
}

// Checks if score threshold triggers phase progression
// Currently uses wave-based progression handled in startWave()
void GameScene::checkProgression() {
  // Wave-based progression (not score-based)
  // Progression logic is handled in startWave()
}

// Universal collision detection and handling for all game objects
void GameScene::checkCollisions() {
  QList<QGraphicsItem *> itemsList = items();
  QList<QGraphicsItem *> itemsToRemove;  // Safe deletion list

  for (QGraphicsItem *item : itemsList) {
    // Skip items already marked for removal
    if (itemsToRemove.contains(item))
      continue;

    // ========== PLAYER COLLISIONS ==========
    if (typeid(*item) == typeid(Player)) {
      QList<QGraphicsItem *> colliding_items = item->collidingItems();
      for (QGraphicsItem *other : colliding_items) {
        if (itemsToRemove.contains(other))
          continue;

        // Player collides with enemy
        if (typeid(*other) == typeid(Asteroid) ||
            typeid(*other) == typeid(StraightShip) ||
            typeid(*other) == typeid(SineShip)) {
          
          Enemy *e = dynamic_cast<Enemy *>(other);
          // Reduce player health by enemy's damage value
          updateHealth(-e->getDamageValue());

          // Create explosion effect at collision point
          Explosion *explosion = new Explosion();
          explosion->setPos(e->x(), e->y());
          addItem(explosion);

          itemsToRemove.append(e);
        } 
        // Player hit by enemy bullet
        else if (typeid(*other) == typeid(Bullet)) {
          Bullet *b = dynamic_cast<Bullet *>(other);
          if (b->getType() == Bullet::EnemyBullet) {
            // Enemy bullets deal 5 damage
            updateHealth(-5);

            // Create explosion effect
            Explosion *explosion = new Explosion();
            explosion->setPos(b->x(), b->y());
            addItem(explosion);

            itemsToRemove.append(b);
          }
        }
      }
    }

    // ========== PLAYER BULLET COLLISIONS ==========
    else if (typeid(*item) == typeid(Bullet)) {
      Bullet *b = dynamic_cast<Bullet *>(item);
      
      // Only check player bullets for enemy hits
      if (b->getType() == Bullet::PlayerBullet) {
        QList<QGraphicsItem *> colliding_items = b->collidingItems();
        for (QGraphicsItem *other : colliding_items) {
          if (itemsToRemove.contains(other))
            continue;

          // Player bullet hits enemy
          if (typeid(*other) == typeid(Asteroid) ||
              typeid(*other) == typeid(StraightShip) ||
              typeid(*other) == typeid(SineShip)) {
            
            Enemy *e = dynamic_cast<Enemy *>(other);

            // Reduce enemy health by 1
            e->updateHealth(1);

            // Enemy destroyed: award points and remove
            if (e->getHealth() <= 0) {
                updateScore(e->getScoreValue());

                Explosion *explosion = new Explosion();
                explosion->setPos(e->x(), e->y());
                addItem(explosion);

                itemsToRemove.append(e);
            }

            // Always show explosion on hit
            Explosion *explosion = new Explosion();
            explosion->setPos(e->x(), e->y());
            addItem(explosion);

            itemsToRemove.append(b);
            break;  // Bullet can only hit one enemy
          }
        }
      }
    }
  }

  // Safely remove and delete all marked items
  for (QGraphicsItem *item : itemsToRemove) {
    if (item->scene() == this) {
      removeItem(item);
      delete item;
    }
  }
}

// Updates player score
void GameScene::updateScore(int points) { 
    score += points; 
}

// Updates player health (negative for damage, positive for healing)
void GameScene::updateHealth(int damage) { 
    health += damage; 
}

// Handles touchscreen input for pause/resume functionality
void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF pos = event->scenePos();

    // If pause menu is visible
    if (menuButton->isVisible()) {
        // Get button geometry in scene coordinates
        QRectF buttonRect = menuButton->geometry();

        // Check if touch is inside the exit button
        if (buttonRect.contains(pos)) {
            // Manually trigger button click to exit to menu
            menuButton->click();
            return;
        }

        // Touch outside button: resume game
        isPaused = false;
        gameTimer->start();
        spawnTimer->start();
        autoShootTimer->start(250);
        pauseImage->hide();
        menuButton->hide();
        return;
    }

    // If not paused: touching anywhere pauses the game
    isPaused = true;
    gameTimer->stop();
    spawnTimer->stop();
    autoShootTimer->stop();
    pauseImage->show();
    menuButton->show();
}

// Handles game over state: displays final score and checks for high score
void GameScene::gameOver() {
  // Stop all game timers
  gameTimer->stop();
  spawnTimer->stop();
  autoShootTimer->stop();

  // Turn off all LEDs
  HardwareIO::setLEDState(false, false, false);

  // Display "GAME OVER" text
  QGraphicsTextItem *over = new QGraphicsTextItem("GAME OVER");
  over->setFont(QFont("times", 30));
  over->setDefaultTextColor(Qt::red);
  over->setPos(width() / 2 - over->boundingRect().width() / 2,
               height() * 0.25);
  addItem(over);

  // Check for new high score
  if (score > highScore) {
    highScore = score;
    saveHighScore();

    // Display "NEW HIGH SCORE!" message
    QGraphicsTextItem *newHigh = new QGraphicsTextItem("NEW HIGH SCORE!");
    newHigh->setFont(QFont("times", 24));
    newHigh->setDefaultTextColor(Qt::green);
    newHigh->setPos(width() / 2 - newHigh->boundingRect().width() / 2,
                    height() * 0.45);
    addItem(newHigh);

    // Display current score
    QGraphicsTextItem *yourScore =
        new QGraphicsTextItem("Your Score: " + QString::number(score));
    yourScore->setFont(QFont("times", 20));
    yourScore->setDefaultTextColor(Qt::white);
    yourScore->setPos(width() / 2 - yourScore->boundingRect().width() / 2,
                      height() * 0.60);
    addItem(yourScore);

  } else {
    // Display existing high score
    QGraphicsTextItem *highScoreMsg =
        new QGraphicsTextItem("High Score: " + QString::number(highScore));
    highScoreMsg->setFont(QFont("times", 20));
    highScoreMsg->setDefaultTextColor(Qt::yellow);
    highScoreMsg->setPos(width() / 2 - highScoreMsg->boundingRect().width() / 2,
                         height() * 0.45);
    addItem(highScoreMsg);

    // Display current score
    QGraphicsTextItem *yourScore =
        new QGraphicsTextItem("Your Score: " + QString::number(score));
    yourScore->setFont(QFont("times", 20));
    yourScore->setDefaultTextColor(Qt::white);
    yourScore->setPos(width() / 2 - yourScore->boundingRect().width() / 2,
                      height() * 0.60);
    addItem(yourScore);
  }

  // Return to main menu after 1.5 seconds
  QTimer::singleShot(1500, [=]() {
      emit exitToMenu();
  });
}

// Loads saved high score from file
void GameScene::loadHighScore() {
  QFile file("highscore.txt");
  if (file.open(QIODevice::ReadOnly)) {
    QTextStream in(&file);
    in >> highScore;
    file.close();
  } else {
    highScore = 0;  // Default to 0 if file doesn't exist
  }
}

// Saves current high score to file
void GameScene::saveHighScore() {
  QFile file("highscore.txt");
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream out(&file);
    out << highScore;
    file.close();
  }
}

// Animates background transition using crossfade effect
// t: fade parameter from 0.0 (bg1 only) to 1.0 (bg2 only)
void GameScene::updateBackgroundFade(float t) {
  // Clamp fade parameter to valid range
  if (t < 0)
    t = 0;
  if (t > 1)
    t = 1;

  // Simple linear crossfade between two backgrounds
  bg1->setOpacity(1.0 - t);  // Fade out first background
  bg2->setOpacity(t);        // Fade in second background
}
