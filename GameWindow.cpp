#include <GameWindow.h>

#include <QDebug>

#include <Game.h>
#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>
#include <Dice.h>
#include <paint_helper.h>
#include <ValueError.h>

const QColor GameWindow::COLOR_RED = QColor (249,94,95);
const QColor GameWindow::COLOR_YELLOW = QColor (255,238,103);
const QColor GameWindow::COLOR_BLUE = QColor (3,175,212);
const QColor GameWindow::COLOR_GREEN = QColor (121,189,154);
const QColor GameWindow::BG_COLOR = QColor (251,248,248);
const QColor GameWindow::STROKE_COLOR = QColor (128,128,128);

const QColor GameWindow::COLOR_RED_LIGHT = QColor (255,134,134);
const QColor GameWindow::COLOR_YELLOW_LIGHT = QColor (255,252,151);
const QColor GameWindow::COLOR_BLUE_LIGHT = QColor (102,212,224);
const QColor GameWindow::COLOR_GREEN_LIGHT = QColor (153,235,148);

GameWindow::GameWindow(unsigned int players_count) :
mGame {new Game(players_count)},
mBoard {mGame->getGameBoard()},
state {ROLLING},
mScreen (new GameScreen(this)),
footer {new QWidget(this)},
footerLayout {new QVBoxLayout()},
dice {new Dice(nullptr, 6)},
hintLabel {new QLabel()} {

    this->setCentralWidget(this->mScreen);

    //Setup menu bar
    QMenuBar *menuBar = this->menuBar();
    QMenu *gameMenu = menuBar->addMenu("&Game");
    QAction *saveAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&Save");
    QAction *loadAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&Load");
    gameMenu->addSeparator();
    QAction *aboutAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&About");
    QAction *exitAction = gameMenu->addAction(QIcon::fromTheme("file-save"), "&Exit");
    connect(saveAction, &QAction::triggered, this, &GameWindow::saveRequested);
    connect(loadAction, &QAction::triggered, this, &GameWindow::loadRequested);
    connect(aboutAction, &QAction::triggered, this, &GameWindow::aboutRequested);
    connect(exitAction, &QAction::triggered, this, &GameWindow::exitRequested);

    dice->setVisualSize(DICE_SIZE);
    hintLabel->setFixedHeight(CELL_SIZE);

    footer->setFixedSize(CELL_SIZE * 5, dice->height()+hintLabel->height());
    footer->move((CELL_SIZE * 15),
        BOARD_BOUND + (this->height() / 2)); //To center-right

    hintLabel->setWordWrap(true);
    dice->setColor(COLOR_RED_LIGHT);
    hintLabel->setFixedWidth(footer->geometry().width());
    hintLabel->setAlignment(Qt::AlignCenter);

    footerLayout->addWidget(dice, 0, Qt::AlignCenter);
    footerLayout->addWidget(hintLabel, 0, Qt::AlignCenter);
    footer->setLayout(footerLayout);

    this->mScreen->setFixedSize(
        (BOARD_BOUND * 2) + (CELL_SIZE * 15) + footer->width(), //Extra space for interaction widget,
        (BOARD_BOUND * 2) + (CELL_SIZE * 15)
    );

    QObject::connect(dice, SIGNAL(clicked()), this, SLOT(rollDiceClicked()));

    //Attach all pawns with the game window
    for(auto pawn : mBoard->getAllPawns()) {
        pawn->attatchWindow(this);
        pawn->setEnabled(false); //Disable all the pawn visuals (buttons)
    }

    updateUi();
}

GameWindow::~GameWindow() {
    delete dice;
    delete hintLabel;
    delete footerLayout;
    delete footer;
    delete mGame;

    delete mScreen;
}

QString GameWindow::getUserName(PlayerColor color) {
    //TODO: Replace this temporary code with something more sensible
    switch (color) {
        case PlayerColor::RED:
            return "Player 1";
        case PlayerColor::YELLOW:
            return "Player 2";
        case PlayerColor::BLUE:
            return "Player 3";
        case PlayerColor::GREEN:
            return "Player 4";

        //This will never happen
        default:
            ValueError::raise_new(QString("GameWindow::getUserName(PlayerColor) : Invalid value of PlayerColor!"));
    }
    #pragma GCC diagnostic ignored "-Wreturn-type"
}
#pragma GCC diagnostic warning "-Wreturn-type"

void GameWindow::updateUi() {
    qInfo() << "\n\n updateUi called**********";

    //Set appropriate color for dice
    switch (mGame->getCurrentPlayer()) {
        case PlayerColor::RED:
            dice->setColor(COLOR_RED_LIGHT);
            break;
        case PlayerColor::YELLOW:
            dice->setColor(COLOR_YELLOW_LIGHT);
            break;
        case PlayerColor::BLUE:
            dice->setColor(COLOR_BLUE_LIGHT);
            break;
        case PlayerColor::GREEN:
            dice->setColor(COLOR_GREEN_LIGHT);
            break;
    }

    if (state == ROLLING) {
        hintLabel->setText(
            QString("%1: Roll the dice!")
                .arg(getUserName(mGame->getCurrentPlayer()))
            );
        for (auto p : mBoard->getAllPawns())
            p->setEnabled(false);
        dice->setEnabled(true);

    } else if (state == MOVING) {
        hintLabel->setText(
            QString("%1: Choose a pawn to move!")
                .arg(getUserName(mGame->getCurrentPlayer()))
            );
        dice->setEnabled(false);

    } else if (state == ANIMATING) {
        hintLabel->setText(QString(""));
        for (auto p : mBoard->getAllPawns())
            p->setEnabled(false);
        dice->setEnabled(false);

    } else
        ValueError::raise_new(QString("GameWindow::updateUi() : Invalid value for GameWindow::state"));
}

void GameWindow::rollDiceClicked() {

    mGame->rollDice();
    hintLabel->setText(QString(""));

    animateDiceRoll();
}

void GameWindow::animateDiceRoll() {
    state = ANIMATING;
    updateUi();

    if(diceAnimationGroup != 0)
        delete diceAnimationGroup;

    diceAnimationGroup = new QParallelAnimationGroup {};

    QPropertyAnimation *rollanimation = new QPropertyAnimation {dice, "rotation", diceAnimationGroup};
    rollanimation->setStartValue(0);
    rollanimation->setEndValue(360*8);
    rollanimation->setDuration(DICE_ANIMATION_DURATION);
    rollanimation->setEasingCurve(QEasingCurve::InOutQuad);

    QSequentialAnimationGroup *shake = new QSequentialAnimationGroup {diceAnimationGroup};

    for (int i = 0; i < 5; i++) {
        QPropertyAnimation *s = new QPropertyAnimation {dice, "pos", shake};
        s->setStartValue(QPoint(dice->x(), dice->y()));
        s->setKeyValueAt(0.25, QPoint(dice->x() - DICE_SHAKE_DISTANCE, dice->y()));
        s->setKeyValueAt(0.50, QPoint(dice->x(), dice->y()));
        s->setKeyValueAt(0.75, QPoint(dice->x() + DICE_SHAKE_DISTANCE, dice->y()));
        s->setEndValue(QPoint(dice->x(), dice->y()));
        s->setDuration(DICE_ANIMATION_DURATION / 5);
        s->setEasingCurve(QEasingCurve::InOutQuad);
        shake->addAnimation(s);
    }

    diceAnimationGroup->addAnimation(rollanimation);
    diceAnimationGroup->addAnimation(shake);
    diceAnimationGroup->start();

    connect(diceAnimationGroup, &QParallelAnimationGroup::finished,
        this,&GameWindow::diceAnimationFinished);
}

void GameWindow::diceAnimationFinished() {

    dice->setValue(mGame->getLastDiceValue());

    QVector<Pawn*> playables = mGame->getPlayablePawns(mGame->getLastDiceValue());

    if(playables.size() == 0) {
        mGame->changeCurrentPlayer(); //We got no pawns worth moving
        //But we need to wait for a second!

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [timer, this](){
            this->updateUi();
            delete timer; //No need to stop it, just delete it
        });
        timer->start(700);
        this->state = ROLLING;
        return; //It makes it look like we're still animating, but it's just a small pause between dice rollings

    } else if(playables.size() == 1) { //Only a singal move available, rather not ask for it
        pawnChosen(playables[0]); //Auto choosen

    } else {
        for (auto p : playables) {
            p->setEnabled(true);
            p->raise(); //So that it is visible
        }

        state = MOVING;
    }

    updateUi();
}

void GameWindow::pawnChosen(Pawn *p) {
    qInfo() << "GameWindow::pawnChosen()";

    qDebug() << "mGame->getLastDiceValue()" << mGame->getLastDiceValue();
    currentPawn = p;
    movePawnVisual(p, mGame->predictRel(p, mGame->getLastDiceValue()));
}

void GameWindow::movePawnVisual(Pawn *p, int newrel) {
    qInfo() << "GameWindow::movePawnVisual()";

    if(p->isAtHome()) {

        QRect geom = painthelp::getPawnGeometry(Board::getPawnCoordinates(
            p->getColor(), newrel
        ));
        p->setGeometry(geom);
        pawnAnimationFinished(); //No animation, so no wait
        state = ROLLING;

    } else {
        //Our magic spell.. Ha..ha..
        state = ANIMATING;
        animateVisualMovement(p, newrel);
    }

    updateUi();
}

void GameWindow::animateVisualMovement(Pawn* p, int endRel) {
    qInfo() << "GameWindow::animateVisualMovement()";

    if(animationGroup != 0)
        delete animationGroup;

    animationGroup = new QSequentialAnimationGroup();
    int startRel = p->getRelPosition();

    for (int i = startRel + 1; i <= endRel; i++) {
        QRect iniCell = painthelp::getPawnGeometry(mBoard->getPawnCoordinates(p->getColor(), i-1));
        QRect destCell = painthelp::getPawnGeometry(mBoard->getPawnCoordinates(p->getColor(), i));

        QPropertyAnimation *animation = new QPropertyAnimation (p, "geometry");
        animation->setDuration(ANIMATION_DURATION);
        animation->setStartValue(iniCell);
        animation->setEndValue(destCell);
        animationGroup->addAnimation(animation);

        animationGroup->addPause(100); //This feels smooth
    }

    connect(animationGroup, &QSequentialAnimationGroup::finished, this, &GameWindow::pawnAnimationFinished);
    animationGroup->start();
}

void GameWindow::pawnClashed(Pawn *p) {
    p->changePosition(Pawn::HOME);
    //This line is used in the the Constructor of Pawn and has details in the comment there
    p->setGeometry(painthelp::getPawnHomePosGeometry(p->getColor(), ((p->getId()+1) % 4) + 1));
}

void GameWindow::pawnAnimationFinished() {
    if (!mGame->playMove(currentPawn, mGame->getLastDiceValue()))
        mGame->changeCurrentPlayer();

    state = ROLLING;
    updateUi();
}

void GameWindow::saveRequested() {

}

void GameWindow::loadRequested() {

}

void GameWindow::aboutRequested() {

}

void GameWindow::exitRequested() {

}

void GameWindow::closeEvent(QCloseEvent* event) {

    QMessageBox dialog;
    dialog.setIcon(QMessageBox::Question);
    dialog.setText("Confirm exit");
    dialog.setInformativeText("Would you like to save the game before exiting?");

    dialog.addButton(QMessageBox::Save);
    dialog.addButton(QMessageBox::Discard);
    dialog.addButton(QMessageBox::Cancel);

    int ret = dialog.exec();

    switch (ret) {
        case QMessageBox::Cancel:
            event->ignore(); //Do nothing
            break;
        case QMessageBox::Save:
            //TODO: Open save dialog here
            event->accept();
            emit exit();
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
    }
}
