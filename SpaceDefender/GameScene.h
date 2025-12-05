#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Player.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QPushButton>

// GameScene manages all active gameplay: player control, enemies,
// UI updates, collisions, timers, and background transitions.
class GameScene : public QGraphicsScene {
    Q_OBJECT

public:
    GameScene(QObject *parent = nullptr);
    void start();  // legacy start trigger (game begins on keypress)

signals:
    void exitToMenu();   // emitted when user chooses to leave game

protected:
    void keyPressEvent(QKeyEvent *event) override;     // handles movement/shoot/pause
    void keyReleaseEvent(QKeyEvent *event) override;   // clears movement flags

private slots:
    void updateGame();    // main game loop called by timer
    void spawnEnemy();    // spawns enemies on an interval

private:
    Player *player;       // main player object
    QTimer *gameTimer;    // drives game updates
    QTimer *spawnTimer;   // controls enemy spawn rate

    QPushButton *menuButton;  // pause menu "Exit" button

    // Input state
    bool leftPressed;
    bool rightPressed;
    bool upPressed;
    bool downPressed;
    bool spacePressed;
    bool isPaused;
    bool gameStarted;

    // Game state variables
    int score;
    int highScore;
    int health;
    int gamePhase; // 0: Start, 1: Asteroids, 2: +Straight, 3: +Sine, 4: Speed Up

    qint64 startTime;    // timestamp used for time-based background fade

    // On-screen UI elements
    QGraphicsTextItem *scoreText;
    QGraphicsTextItem *highScoreText;
    QGraphicsTextItem *healthText;
    QGraphicsPixmapItem *pauseImage;
    QGraphicsPixmapItem *logoItem;
    QGraphicsPixmapItem *startImage;

    // Two background layers used for transition
    QGraphicsPixmapItem *bg1;  // near-ground background
    QGraphicsPixmapItem *bg2;  // upper-space background

    // Internal helpers
    void updateScore(int points);           // adds to score
    void updateHealth(int damage);          // applies player damage
    void checkCollisions();                 // resolves bullets/enemy/player hits
    void checkProgression();                // updates gamePhase based on score
    void gameOver();                        // displays end screen + triggers return
    void loadHighScore();                   // reads highscore.txt
    void saveHighScore();                   // writes highscore.txt
    void updateBackgroundFade(float t);     // handles background opacity blend
};

#endif // GAMESCENE_H
