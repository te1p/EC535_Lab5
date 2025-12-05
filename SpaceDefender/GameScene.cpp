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
#include <QDateTime>


GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), leftPressed(false), rightPressed(false),upPressed(false), downPressed(false),
    spacePressed(false), isPaused(false), gameStarted(false), score(0),
    highScore(0), health(1000), gamePhase(0) {

    loadHighScore();
    // Initializes high score and all gameplay state variables.


    // Set fixed game resolution (hardware LCD)
    setSceneRect(0, 0, 480, 272);


    // Background #1 (main sky layer)
    bg1 = new QGraphicsPixmapItem(
        QPixmap(":/assets/assets/Background_WIP.png")
            .scaled(480, 272, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    bg1->setZValue(-20);
    addItem(bg1);

    // Background #2 (space layer for fade transition)
    bg2 = new QGraphicsPixmapItem(
        QPixmap(":/assets/assets/Background_space_entry_WIP.png")
            .scaled(480, 272, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    bg2->setZValue(-19);
    bg2->setOpacity(0.0);   // starts invisible
    addItem(bg2);


    // HUD logo
    logoItem = new QGraphicsPixmapItem(
        QPixmap(":/assets/assets/Logo.png")
            .scaled(80, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoItem->setZValue(100);
    logoItem->setPos(480 - logoItem->pixmap().width() - 5, 5);
    addItem(logoItem);


    // Player setup
    player = new Player();
    addItem(player);
    player->setPos(240 - player->pixmap().width() / 2, 210);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    player->show();


    // Score text
    scoreText = new QGraphicsTextItem();
    scoreText->setPlainText("Score: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("times", 10));
    addItem(scoreText);
    scoreText->setPos(5, 5);
    scoreText->show();

    // High score display
    highScoreText = new QGraphicsTextItem();
    highScoreText->setPlainText("High Score: " + QString::number(highScore));
    highScoreText->setDefaultTextColor(Qt::red);
    highScoreText->setFont(QFont("times", 14, QFont::Bold));
    addItem(highScoreText);
    highScoreText->setPos(240 - 80, 120);


    // Health text
    healthText = new QGraphicsTextItem();
    healthText->setPlainText("Health: 1000");
    healthText->setDefaultTextColor(Qt::red);
    healthText->setFont(QFont("times", 10));
    addItem(healthText);
    healthText->setPos(5, 25);
    healthText->show();


    // Pause image overlay
    pauseImage = new QGraphicsPixmapItem(
        QPixmap(":/assets/assets/Pause.png")
            .scaled(300, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pauseImage->setPos(240 - pauseImage->pixmap().width() / 2,
                       136 - pauseImage->pixmap().height() / 2);
    pauseImage->setZValue(200);
    pauseImage->hide();
    addItem(pauseImage);


    // Exit button for pause menu
    menuButton = new QPushButton("Exit");
    menuButton->setFixedSize(150, 36);
    menuButton->setStyleSheet(
        "QPushButton {"
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
    // Positioned under pause image
    menuButton->setGeometry(240 - 75, 175, 150, 36);
    addWidget(menuButton);
    menuButton->hide();

    // Return to main menu when clicked
    connect(menuButton, &QPushButton::clicked, this, [this]() {
        emit exitToMenu();
    });


    // Main game loop timer (movement, collisions, UI)
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateGame()));
    gameTimer->start(50);   // 20 FPS update rate

    // Enemy spawn timer
    spawnTimer = new QTimer(this);
    connect(spawnTimer, SIGNAL(timeout()), this, SLOT(spawnEnemy()));
    spawnTimer->start(2000);

    //Automatic shooting
    // Faster bullets in late game
    // shootTimer = new QTimer(this);
    // int bulletSpeed = (gamePhase >= 3) ? 40 : 20;
    // player->shoot(bulletSpeed);

    // Track intro fade timing
    startTime = QDateTime::currentMSecsSinceEpoch();

    highScoreText->hide(); // optionally hidden until needed
}

void GameScene::start() {
    // Game actually begins on first key press
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    // Pause mode: only certain keys unpause
    if (isPaused) {
        if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
            event->key() == Qt::Key_Space || event->key() == Qt::Key_Escape) {

            isPaused = false;
            gameTimer->start();
            spawnTimer->start();
            pauseImage->hide();
            menuButton->hide();

            if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Space)
                return; // prevent triggering action instantly
        } else {
            return;
        }
    }

    // Movement + shooting
    if (event->key() == Qt::Key_Left) {
        leftPressed = true;
    }
    else if (event->key() == Qt::Key_Right) {
        rightPressed = true;
    }
    else if (event->key() == Qt::Key_Up)
    {
        upPressed = true;
    }
    else if (event->key() == Qt::Key_Down)
    {
        downPressed = true;
    }
    // else if (event->key() == Qt::Key_Space) {
    //     // Faster bullets in late game
    //     int bulletSpeed = (gamePhase >= 3) ? 40 : 20;
    //     player->shoot(bulletSpeed);
    // }
    else if (event->key() == Qt::Key_Escape) {
        // Enter pause mode
        isPaused = true;
        gameTimer->stop();
        spawnTimer->stop();
        pauseImage->show();
        menuButton->show();
    }
}

void GameScene::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat())
        return;

    if (event->key() == Qt::Key_Left)
        leftPressed = false;
    else if (event->key() == Qt::Key_Right)
        rightPressed = false;
    else if (event->key() == Qt::Key_Up)
        upPressed = false;
    else if (event-> key() == Qt::Key_Down)
        downPressed = false;
}

void GameScene::spawnEnemy() {
    // Spawn ratios change as score increases (difficulty progression)

    int speedMult = (gamePhase >= 3) ? 2 : 1;

    Enemy::Type type = Enemy::Asteroid;
    int r = rand() % 100;

    if (gamePhase == 0) {
        type = Enemy::Asteroid;
    } else if (gamePhase == 1) {
        type = (r < 50) ? Enemy::Asteroid : Enemy::StraightShip;
    } else {
        // Mix of all types
        if (r < 33)
            type = Enemy::Asteroid;
        else if (r < 66)
            type = Enemy::StraightShip;
        else
            type = Enemy::SineShip;
    }

    // Create enemy
    Enemy *enemy = new Enemy(type, speedMult);
    addItem(enemy);
}

void GameScene::updateGame() {
    // Player continuous movement
    if (leftPressed)  player->moveLeft();
    if (rightPressed) player->moveRight();
    if (upPressed) player->moveUp();
    if (downPressed) player->moveDown();

    // Update all moving objects
    QList<QGraphicsItem *> sceneItems = items();
    for (int i = 0, n = sceneItems.size(); i < n; ++i) {
        if (typeid(*(sceneItems[i])) == typeid(Enemy)) {
            dynamic_cast<Enemy *>(sceneItems[i])->move();
        } else if (typeid(*(sceneItems[i])) == typeid(Bullet)) {
            dynamic_cast<Bullet *>(sceneItems[i])->move();
        }
    }

    // Handle collisions for this frame
    checkCollisions();

    // Update UI elements
    scoreText->setPlainText("Score: " + QString::number(score));
    healthText->setPlainText("Health: " + QString::number(health));

    // Check if score pushes player into next phase
    checkProgression();

    // Handle background crossfade
    qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - startTime;
    float t = (float)elapsed / 10000.0f;
    updateBackgroundFade(t);

    // Health depleted → game over
    if (health <= 0)
        gameOver();
}

void GameScene::checkProgression() {
    // Score thresholds
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
    // Universal collision handler for player, enemies, bullets, etc.

    QList<QGraphicsItem *> itemsList = items();
    QList<QGraphicsItem *> itemsToRemove;

    for (QGraphicsItem *item : itemsList) {
        if (itemsToRemove.contains(item))
            continue;

        // PLAYER COLLISIONS
        if (typeid(*item) == typeid(Player)) {
            QList<QGraphicsItem *> colliding_items = item->collidingItems();
            for (QGraphicsItem *other : colliding_items) {
                if (itemsToRemove.contains(other))
                    continue;

                if (typeid(*other) == typeid(Enemy)) {
                    // Enemy hits player
                    Enemy *e = dynamic_cast<Enemy *>(other);
                    updateHealth(e->getType() == Enemy::Asteroid ? -50 : -100);

                    // Explosion visual
                    Explosion *explosion = new Explosion();
                    explosion->setPos(e->x(), e->y());
                    addItem(explosion);

                    itemsToRemove.append(e);
                }
                else if (typeid(*other) == typeid(Bullet)) {
                    // Enemy bullet hits player
                    Bullet *b = dynamic_cast<Bullet *>(other);
                    if (b->getType() == Bullet::EnemyBullet) {

                        updateHealth(-5);

                        Explosion *explosion = new Explosion();
                        explosion->setPos(b->x(), b->y());
                        addItem(explosion);

                        itemsToRemove.append(b);
                    }
                }
            }
        }

        // PLAYER BULLET COLLISIONS
        else if (typeid(*item) == typeid(Bullet)) {
            Bullet *b = dynamic_cast<Bullet *>(item);
            if (b->getType() == Bullet::PlayerBullet) {

                QList<QGraphicsItem *> colliding_items = b->collidingItems();
                for (QGraphicsItem *other : colliding_items) {
                    if (itemsToRemove.contains(other))
                        continue;

                    if (typeid(*other) == typeid(Enemy)) {
                        // Player shoots enemy
                        Enemy *e = dynamic_cast<Enemy *>(other);

                        updateScore(e->getType() == Enemy::Asteroid ? 50 : 100);

                        Explosion *explosion = new Explosion();
                        explosion->setPos(e->x(), e->y());
                        addItem(explosion);

                        itemsToRemove.append(e);
                        itemsToRemove.append(b);
                        break;
                    }
                }
            }
        }
    }

    // Safe deletion pass
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
    // Stops gameplay and displays summary text
    gameTimer->stop();
    spawnTimer->stop();

    QGraphicsTextItem *over = new QGraphicsTextItem("GAME OVER");
    over->setFont(QFont("times", 30));
    over->setDefaultTextColor(Qt::red);
    over->setPos(300, 150);
    addItem(over);

    // High score check + messages
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

    // Quick return to menu
    QTimer::singleShot(1500, [=]() {
        emit exitToMenu();
    });
}

void GameScene::loadHighScore() {
    // Read high score from file or default to 0
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
    // Write high score to file
    QFile file("highscore.txt");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << highScore;
        file.close();
    }
}

void GameScene::updateBackgroundFade(float t)
{
    // Clamp fade parameter
    if (t < 0) t = 0;
    if (t > 1) t = 1;

    // Simple crossfade between bg1 and bg2
    bg1->setOpacity(1.0 - t);
    bg2->setOpacity(t);
}
