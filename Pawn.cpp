#include <Pawn.h>

#include <QDebug>

#include <Board.h>
#include <PlayerColor.h>
#include <GameWindow.h>
#include <SaveGameEngine.h>
#include <ValueError.h>
#include <paint_helper.h>

const qreal Pawn::GLOW_SCALE = 0.25;

Pawn::Pawn(PlayerColor color, int id) :
mColor(color), mId(id), mPos(-1) {

    //This is not the drawing size (refer to paintEvent())
    //this->setFixedSize(GameWindow::CELL_SIZE, GameWindow::CELL_SIZE);

    this->setGeometry(painthelp::getPawnHomePosGeometry(color, this->getIndex()));
}

Pawn::Pawn(SaveGameEngine *save) {
    this->mColor = static_cast<PlayerColor>(save->readInt());
    this->mId = save->readInt();
    this->mPos = save->readInt();
    this->mGlow = save->readReal();
}

void Pawn::serializeInto(SaveGameEngine *save) {
    save->writeInt(static_cast<int>(this->mColor));
    save->writeInt(this->mId);
    save->writeInt(this->mPos);
    save->writeReal(this->mGlow);
}

Pawn::~Pawn() {}

void Pawn::attatchWindow(GameWindow *w) {
    this->setParent(w->centralWidget());
    QObject::connect(this, &Pawn::visualClicked, w, &GameWindow::pawnChosen);
    QObject::connect(this, &Pawn::clashed, w, &GameWindow::pawnClashed);
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

unsigned int Pawn::getIndex() {
    //index is extracted using id of the pawn, as the ids are sure to be in sequence
    return ((this->getId()+1) % 4) + 1;
}

QPoint Pawn::getPositionOnBoard() {
    return Board::getPawnCoordinates(this);
}

bool Pawn::isAtHome() {
    return mPos == HOME;
}

bool Pawn::hasReachedDestination() {
    return mPos == DEST;
}

void Pawn::changePosition(int rel) {
    if(rel > DEST || rel < HOME)
        ValueError::raise_new(QString("Pawn::goto(int) : Invalid move! rel == ").arg(rel));

    this->mPos = rel;
}

void Pawn::moveForward(int steps) {
    if(this->mPos + steps > DEST || steps < 1 || steps > 6)
        ValueError::raise_new(QString("Pawn::moveForward(int) : Invalid move! step == ")
            .arg(steps));
    this->changePosition(this->mPos + steps);
}

void Pawn::goBackHome(bool clash) {
    qInfo() << "Pawn::goBackHome()";
    if(clash)
        emit clashed(this);
    else
        this->changePosition(HOME);
}

void Pawn::setGlow(qreal glow)  {
    if(glow < 0) {
        mGlow = 0;
    } else if(glow > 1) {
        mGlow = 1;
    } else {
        mGlow = glow;
    }

    repaint();
}

qreal Pawn::getGlow() {
    return mGlow;
}

void Pawn::setEnabled(bool state) {
    QWidget::setEnabled(state);

    if(glowEffect != nullptr) {
        delete glowEffect;
        glowEffect = nullptr;
    }

    if(state) { //Play animation
        glowEffect = new QPropertyAnimation(this, "glow", this);
        glowEffect->setStartValue(0);
        glowEffect->setKeyValueAt(0.5, 1);
        glowEffect->setEndValue(0);
        glowEffect->setLoopCount(-1);
        glowEffect->setDuration(GLOW_DURATION);
        glowEffect->start();

    } else
        setGlow(0);
}

void Pawn::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(
        GameWindow::STROKE_COLOR,
        GameWindow::STROKE_WIDTH
    ));

    QColor color {};
    switch (mColor) {
        case PlayerColor::RED:
            color = GameWindow::COLOR_RED;
            break;
        case PlayerColor::YELLOW:
            color = GameWindow::COLOR_YELLOW;
            break;
        case PlayerColor::BLUE:
            color = GameWindow::COLOR_BLUE;
            break;
        case PlayerColor::GREEN:
            color = GameWindow::COLOR_GREEN;
            break;
    }

    painter.setBrush(color);
    QRect r = this->rect();

    qreal scale = ((qreal) GameWindow::PAWN_SIZE / (qreal) GameWindow::CELL_SIZE) + (getGlow() * GLOW_SCALE);

    painter.translate(
        ((GameWindow::CELL_SIZE) - (r.width() * scale)) / 2,
        ((GameWindow::CELL_SIZE) - (r.width() * scale)) / 2
    );

    painter.drawEllipse(
        0,
        0,
        r.width() * scale,
        r.width() * scale
    );
}

void Pawn::mousePressEvent(QMouseEvent*) {
    emit visualClicked(this);
}
