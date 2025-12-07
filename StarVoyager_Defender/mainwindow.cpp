#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Window is locked to the BeagleBone LCD resolution.
    this->setFixedSize(480, 272);

    // Background image added manually as a QLabel.
    // This avoids Qt’s stylesheet scaling, which caused unwanted zooming.
    bgLabel = new QLabel(this);
    QPixmap bgPix(":/assets/assets/corners.jpg");
    if (!bgPix.isNull()) {
        bgPix = bgPix.scaled(480, 272, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        bgLabel->setPixmap(bgPix);
        bgLabel->setGeometry(0, 0, 480, 272);
        bgLabel->lower();     // keeps it behind UI elements
    }

    // Logo image placed manually at the upper center.
    // Using a QLabel allows correct transparency and scaling.
    logoLabel = new QLabel(this);
    QPixmap pix(":/assets/assets/Logo.png");
    if (!pix.isNull()) {
        QPixmap scaled = pix.scaledToWidth(200, Qt::SmoothTransformation);
        logoLabel->setPixmap(scaled);
        logoLabel->setAttribute(Qt::WA_TranslucentBackground);
        logoLabel->setFixedSize(scaled.size());
        logoLabel->setAlignment(Qt::AlignCenter);

        // Center horizontally, slightly above normal UI.
        int x = (width() - logoLabel->width()) / 2;
        logoLabel->move(x, -30);
        logoLabel->raise();
    }

    // "PRESS PLAY" text under the button.
    // Works as a subtle animated HUD to guide the player.
    QLabel *hudLabel = new QLabel("PRESS PLAY", this);
    hudLabel->setAlignment(Qt::AlignCenter);

    QFont fHud("Consolas");
    fHud.setPointSize(9);
    fHud.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    hudLabel->setFont(fHud);
    hudLabel->setStyleSheet("color: rgba(180,235,255,220);");

    // Position HUD relative to the settingsButton position from Designer.
    // If Designer is missing, falls back to center-ish coordinates.
    if (ui->settingsButton) {
        QRect g = ui->settingsButton->geometry();
        hudLabel->setGeometry(g.x(), g.y() + g.height() + 4, g.width(), 20);
    } else {
        hudLabel->setGeometry(140, 150, 200, 20);
    }

    // Light opacity animation to simulate flicker.
    QGraphicsOpacityEffect *hudEff = new QGraphicsOpacityEffect(this);
    hudLabel->setGraphicsEffect(hudEff);

    QPropertyAnimation *hudAnim = new QPropertyAnimation(hudEff, "opacity", this);
    hudAnim->setDuration(1600);
    hudAnim->setKeyValueAt(0.0, 0.75);
    hudAnim->setKeyValueAt(0.5, 1.0);
    hudAnim->setKeyValueAt(1.0, 0.85);
    hudAnim->setLoopCount(-1);
    hudAnim->start();

    /*
     * CREDITS PANEL (settings screen)
     * - Appears when the user presses the “Settings” button.
     * - Covers part of the screen with a translucent, styled box.
     * - All main menu buttons are temporarily disabled while the panel is open.
     */
    creditsPanel = new QWidget(this);
    creditsPanel->setGeometry(60, 25, 360, 220);
    creditsPanel->setStyleSheet(
        "background-color: rgba(0,0,0,180);"
        "border: 3px solid #00e5ff;"
        "border-radius: 18px;"
        );
    creditsPanel->hide();

    // Title at the top of the panel.
    creditsTitle = new QLabel("CREDITS", creditsPanel);
    creditsTitle->setAlignment(Qt::AlignCenter);
    creditsTitle->setGeometry(0, 10, 360, 30);
    creditsTitle->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");

    // Inner content box for styling separation.
    QWidget *bodyBox = new QWidget(creditsPanel);
    bodyBox->setGeometry(15, 50, 330, 125);
    bodyBox->setStyleSheet(
        "background-color: rgba(0,0,0,90);"
        "border: 3px solid #00e5ff;"
        "border-radius: 16px;"
        );

    // Credit text centered in the content box.
    QLabel *bodyText = new QLabel(
        "Developed by:\n"
        "- Phyliss Darko\n"
        "- Tymoteusz Pardej\n"
        "- Dhiraj Simhadri\n"
        "\nEC535 – Lab5",
        bodyBox
        );
    bodyText->setAlignment(Qt::AlignCenter);
    bodyText->setGeometry(10, 10, 310, 105);
    bodyText->setStyleSheet("color: white; font-size: 12px;");

    // Button to close the credits panel.
    creditsBackButton = new QPushButton("Back", creditsPanel);
    creditsBackButton->setGeometry(105, 180, 150, 32);
    creditsBackButton->setStyleSheet(
        "QPushButton {"
        "  color: white;"
        "  background-color: rgba(0,0,0,120);"
        "  border: 3px solid #00e5ff;"
        "  border-radius: 16px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { border-color: #5ff6ff; }"
        "QPushButton:pressed { background-color: rgba(0,0,0,170); }"
        );

    // Closing the panel restores main menu control.
    connect(creditsBackButton, &QPushButton::clicked, this, [=]() {
        hideCreditsPanel();
    });

    // Connect menu buttons to their handlers.
    connect(ui->playButton, &QPushButton::clicked,
            this, &MainWindow::on_playButton_clicked);
    connect(ui->settingsButton, &QPushButton::clicked,
            this, &MainWindow::on_settingsButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked,
            this, &MainWindow::on_exitButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Show Credits overlay and disable menu buttons.
void MainWindow::showCreditsPanel()
{
    creditsPanel->raise();
    creditsPanel->show();

    ui->playButton->setEnabled(false);
    ui->settingsButton->setEnabled(false);
    ui->exitButton->setEnabled(false);
}

// Hide Credits overlay and re-enable menu buttons.
void MainWindow::hideCreditsPanel()
{
    creditsPanel->hide();

    ui->playButton->setEnabled(true);
    ui->settingsButton->setEnabled(true);
    ui->exitButton->setEnabled(true);
}

// Play button creates a new GameScene in a new borderless QGraphicsView.
// The menu hides until the scene emits exitToMenu().
void MainWindow::on_playButton_clicked()
{
    if (view) { view->hide(); delete view; view = nullptr; }
    if (scene) { delete scene; scene = nullptr; }

    // GameScene manages gameplay, logic, and drawing.
    scene = new GameScene(this);

    // QGraphicsView acts as the “window” for the scene.
    view = new QGraphicsView();
    view->setScene(scene);
    view->setFixedSize(480, 272);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);

    // When ESC → Exit is pressed in-game, return to menu.
    connect(scene, &GameScene::exitToMenu, this, [=]() {
        view->hide();
        this->show();
    });

    this->hide();
    view->show();
}

// Settings button simply opens Credits panel.
void MainWindow::on_settingsButton_clicked()
{
    showCreditsPanel();
}

// Exit closes the whole program.
void MainWindow::on_exitButton_clicked()
{
    close();
}
