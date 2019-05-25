#include <Pawn.h>

#include <Board.h>
#include <PlayerColor.h>
#include <GameWindow.h>

#include <paint_helper.h>

#include <QDebug>

Pawn::Pawn(Board* board, PlayerColor color, int id) :
QPushButton(""), mBoard(board), mColor(color), mId(id), mPos(-1) {

    this->resize(GameWindow::PAWN_SIZE, GameWindow::PAWN_SIZE);

    QPalette palette = this->palette();
    switch (color) {
        case PlayerColor::RED:
            palette.setColor(QPalette::Button, GameWindow::COLOR_RED);
            break;
        case PlayerColor::YELLOW:
            palette.setColor(QPalette::Button, GameWindow::COLOR_YELLOW);
            break;
        case PlayerColor::BLUE:
            palette.setColor(QPalette::Button, GameWindow::COLOR_BLUE);
            break;
        case PlayerColor::GREEN:
            palette.setColor(QPalette::Button, GameWindow::COLOR_GREEN);
            break;
    } this->setPalette(palette);

    /*This line uses a slightly cunning  way to get the move positions
     *getPawnHomePos returns the cordinates by the color and index (1~4)
     *index is extracted using id of the pawn as the ids are sure to be in sequence*/
    this->setGeometry(painthelp::getPawnHomePosGeometry(color, ((id+1) % 4) + 1));

    QObject::connect(this, &QPushButton::clicked, this, [this]() {
        qInfo() << "Pawn::clicked() : emiting visualClicked(this)";
        emit this->visualClicked(this);
    });
}

Pawn::~Pawn() {}

void Pawn::attatchWindow(GameWindow *w) {
    this->setParent(w);
    QObject::connect(this, SIGNAL(visualClicked(Pawn*)), w, SLOT(pawnChosen(Pawn*)));
    QObject::connect(this, SIGNAL(clashed(Pawn*)), w, SLOT(pawnClashed(Pawn*)));
}

int Pawn::getRelPosition() {
    return mPos;
}

PlayerColor Pawn::getColor() {
    return mColor;
}

unsigned int Pawn::getId() {
    return mId;
}

QPoint Pawn::getPositionOnBoard() {
    return mBoard->getPawnCoordinates(this);
}

bool Pawn::isAtHome() {
    return mPos == HOME;
}

bool Pawn::hasReachedDestination() {
    return mPos == DEST;
}

void Pawn::changePosition(int rel) {
    if(rel > DEST || rel < HOME)
        throw std::string {"Pawn::goto(int) : Invalid move! rel == "} + std::to_string(rel);
    this->mPos = rel;
}

void Pawn::moveForward(int steps) {
    if(this->mPos + steps > DEST || steps < 1 || steps > 6)
        throw std::string {"Pawn::moveForward(int) : Invalid move! step == "} + std::to_string(steps);
    this->changePosition(this->mPos + steps);
}

void Pawn::goBackHome(bool clash) {
    qInfo() << "Pawn::goBackHome()";
    if(clash)
        emit clashed(this);
    else
        this->changePosition(HOME);
}
