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

GameWindow::GameWindow() :
players_count {4}, mGame {new Game(players_count)}, mBoard {mGame->getGameBoard()},
state {ROLLING}, footer {new QWidget(this)}, footerLayout {new QVBoxLayout()},
dice {new Dice(nullptr, STROKE_WIDTH, 6)}, hintLabel {new QLabel("Player 1: Roll the dice!!")} {

    dice->setVisualSize(DICE_SIZE);
    dice->setColor(COLOR_RED_LIGHT);
    hintLabel->setFixedHeight(35);

    footer->move(BOARD_BOUND, (CELL_SIZE * 15) + BOARD_BOUND); //To bottom
    footer->setFixedSize(CELL_SIZE * 15, CELL_SIZE * 2.7);

    footerLayout->addWidget(dice, 0, Qt::AlignHCenter);
    footerLayout->addWidget(hintLabel, 0, Qt::AlignHCenter);
    footer->setLayout(footerLayout);

    this->setFixedSize(
        (BOARD_BOUND * 2) + (CELL_SIZE * 15),
        (CELL_SIZE * 15) +
            footer->height() //Extra space for interaction widget
    );

    QObject::connect(dice, SIGNAL(clicked()), this, SLOT(rollDiceClicked()));

    //Attach all pawns with the game window
    for(auto pawn : mBoard->getAllPawns()) {
        pawn->attatchWindow(this);
        pawn->setEnabled(false); //Disable all the pawn visuals (buttons)
    }
}

GameWindow::~GameWindow() {
    delete dice;
    delete hintLabel;
    delete footerLayout;
    delete footer;
    delete mGame;
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
}

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
        state = ROLLING;

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
        qDebug() << "GameWindow::movePawnVisual : p->isAtHome() == true  newrel == " << newrel;

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

void GameWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush backBrush (BG_COLOR);
    painter.setBrush(backBrush);

    QRect outer(BOARD_BOUND, BOARD_BOUND, CELL_SIZE * 15, CELL_SIZE * 15);
    painter.drawRect(outer);

    painter.setPen(QPen(QBrush(STROKE_COLOR), STROKE_WIDTH));

    //Printing the grids (Like some little kid's math notebook, lmao!)
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            QRect g (BOARD_BOUND + (i * CELL_SIZE), BOARD_BOUND + (j * CELL_SIZE), CELL_SIZE, CELL_SIZE);
            painter.drawRect(g);
        }
    }

    painter.drawRect(painthelp::getDestRect());

    drawHomes(painter);
    drawGuidePaths(painter);
}

//Draws homes for all players
void GameWindow::drawHomes(QPainter &painter) {
    QBrush redBrush (COLOR_RED);
    painter.setBrush(redBrush);
    painter.drawRect(painthelp::getRedHomeRect());

    QBrush yellowBrush (COLOR_YELLOW);
    painter.setBrush(yellowBrush);
    painter.drawRect(painthelp::getYellowHomeRect());

    QBrush blueBrush (COLOR_BLUE);
    painter.setBrush(blueBrush);
    painter.drawRect(painthelp::getBlueHomeRect());

    QBrush greenBrush (COLOR_GREEN);
    painter.setBrush(greenBrush);
    painter.drawRect(painthelp::getGreenHomeRect());

    //Draw four white circles on each home which holds the pawns
    painter.setBrush(QBrush(BG_COLOR));
    for (auto e : painthelp::getHomeCircleRects())
        painter.drawEllipse(e);
}


//Colors some proper cells to guide the pawns
void GameWindow::drawGuidePaths(QPainter &painter) {
    QPoint redg {1, 6};
    painter.setBrush(COLOR_RED);
    painter.drawEllipse(painthelp::getCellRect(redg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);
    redg = painthelp::pointBelow(redg);
    painter.drawEllipse(painthelp::getCellRect(redg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);

    for (int i = 0; i < 4; i++) {
        redg = painthelp::pointRight(redg);
        painter.drawEllipse(painthelp::getCellRect(redg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);
    }

    QPoint yellowg {8, 1};
    painter.setBrush(COLOR_YELLOW);
    painter.drawEllipse(painthelp::getCellRect(yellowg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);
    yellowg = painthelp::pointLeft(yellowg);
    painter.drawEllipse(painthelp::getCellRect(yellowg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);

    for (int i = 0; i < 4; i++) {
        yellowg = painthelp::pointBelow(yellowg);
        painter.drawEllipse(painthelp::getCellRect(yellowg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);
    }

    QPoint blueg {13, 8};
    painter.setBrush(COLOR_BLUE);
    painter.drawEllipse(painthelp::getCellRect(blueg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);
    blueg = painthelp::pointAbove(blueg);
    painter.drawEllipse(painthelp::getCellRect(blueg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);

    for (int i = 0; i < 4; i++) {
        blueg = painthelp::pointLeft(blueg);
        painter.drawEllipse(painthelp::getCellRect(blueg).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);
    }

    QPoint greeng {6, 13};
    painter.setBrush(COLOR_GREEN);
    painter.drawEllipse(painthelp::getCellRect(greeng).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);
    greeng = painthelp::pointRight(greeng);
    painter.drawEllipse(painthelp::getCellRect(greeng).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);

    for (int i = 0; i < 4; i++) {
        greeng = painthelp::pointAbove(greeng);
        painter.drawEllipse(painthelp::getCellRect(greeng).center(), GUIDER_DOT_SIZE, GUIDER_DOT_SIZE);
    }
}
