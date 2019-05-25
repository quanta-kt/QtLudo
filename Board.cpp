#include <Board.h>

#include <Pawn.h>
#include <PlayerColor.h>
#include <Path.h>

#include <QDebug>

Board::Board(unsigned int players) :
mPlayers(players) {

    /* Initialize the player vector
     * NOTE : Fall-through is intended here and is not a mistake (Supress or ignore compiler warnings) */
    switch(players) {
        case 4:
            mPawns.append(new Pawn(this, PlayerColor::GREEN, 0));
            mPawns.append(new Pawn(this, PlayerColor::GREEN, 1));
            mPawns.append(new Pawn(this, PlayerColor::GREEN, 2));
            mPawns.append(new Pawn(this, PlayerColor::GREEN, 3));
        case 3:
            mPawns.append(new Pawn(this, PlayerColor::YELLOW, 4));
            mPawns.append(new Pawn(this, PlayerColor::YELLOW, 5));
            mPawns.append(new Pawn(this, PlayerColor::YELLOW, 6));
            mPawns.append(new Pawn(this, PlayerColor::YELLOW, 7));
        case 2:
            mPawns.append(new Pawn(this, PlayerColor::BLUE, 8));
            mPawns.append(new Pawn(this, PlayerColor::BLUE, 9));
            mPawns.append(new Pawn(this, PlayerColor::BLUE, 10));
            mPawns.append(new Pawn(this, PlayerColor::BLUE, 11));

            mPawns.append(new Pawn(this, PlayerColor::RED, 12));
            mPawns.append(new Pawn(this, PlayerColor::RED, 13));
            mPawns.append(new Pawn(this, PlayerColor::RED, 14));
            mPawns.append(new Pawn(this, PlayerColor::RED, 15));
            break;

        default:
            throw std::string {"Invalid number of players. Expected value \
            between 2 to 4 but got "} + std::to_string(players);
    }
}

/* Destory all the resources used by the class */
Board::~Board() {
    for (auto p : mPawns)
        delete p;
}

/* Returns the coordinates of a pawn if it's color and relpos were same as given
 * This should not be called when the pawn is at home (relpos == -1) */
QPoint Board::getPawnCoordinates(PlayerColor color, unsigned int relpos) {
    qInfo() << "Board::getPawnCoordinates(PlayerColor, int) : relpos == " << relpos;

    if (relpos > Path::MAX_REL)
        throw std::string {"Board::getPawnCoordinates(PlayerColor, unsigned int)\
        : Invalid value for relpos == "} + std::to_string(relpos);

    switch (color) {
        case PlayerColor::RED:
            return Path::getAbsoluteCordinates(relpos);
        case PlayerColor::YELLOW:
            return Path::rotatePointToRight(Path::getAbsoluteCordinates(relpos));
        case PlayerColor::BLUE:
            return Path::rotatePointToRight(Path::getAbsoluteCordinates(relpos), 2);
        case PlayerColor::GREEN:
            return Path::rotatePointToRight(Path::getAbsoluteCordinates(relpos), 3);
        }

    //This should never happen
    throw std::string {"Board::getPawnCoordinates(PlayerColor, int) : Invalid PlayerColor"};
}

/* Returns the coordinates of the pawn passed by pointer. Pawn is not required to be on board
 * This should not be called when the pawn is at home */
QPoint Board::getPawnCoordinates(Pawn* p) {
    return getPawnCoordinates(p->getColor(), p->getRelPosition());
}

//Returns a vector, which holds all the Pawns on the board, including the ones won or at home
QVector<Pawn*> Board::getAllPawns() {
    return mPawns;
}

//Returns a vector holding all the Pawns located at the given point on the board
QVector<Pawn*> Board::getPawnsAt(QPoint point) {
    qInfo() << "Board::getPawnsAt(QPoint)";

    QVector<Pawn*> result {};
    for (auto pawn : mPawns) {
        if(pawn->isAtHome()) //Skip this one
            continue;
        if(getPawnCoordinates(pawn).x() == point.x() && getPawnCoordinates(pawn).y() == point.y())
            result.append(pawn);
    }

    return result;
}

/* Returns a pointer to the pawn with the passed id. Ids are unique to all pawns
 * on a board. Returned pawn is supposed to be on the board */
Pawn* Board::getPawnById(unsigned int id) {
    return mPawns[id];
}


/* Returns a vector of pawn pointers matching the given color */
QVector<Pawn*> Board::getAllPawnsByColor(PlayerColor color) {
    if(color == PlayerColor::YELLOW && mPlayers == 2)
        throw std::string {"Board::getAllPawnsByColor : Current game does not have YELLOW color"};
    if(color == PlayerColor::GREEN && mPlayers != 4)
        throw std::string {"Board::getAllPawnsByColor : Current game does not have GREEN color"};

    QVector<Pawn*> result {};
    for (auto p : mPawns) {
        if(p->getColor() == color)
            result.append(p);
    }

    return result;
}

/* Returns a pointer to the Pawn identified by it's color and index (1~4)
 * Returns the nullptr if the pawn does not exist */
Pawn* Board::getPawn(PlayerColor color, unsigned int which) {
    if(color == PlayerColor::YELLOW && mPlayers == 2)
        return nullptr;
    if(color == PlayerColor::GREEN && mPlayers != 4)
        return nullptr;
    if(which > 4)
        return nullptr;

    int id {};
    switch (color) {
        case PlayerColor::GREEN:
            id = 0;
            break;
        case PlayerColor::YELLOW:
            id = 4;
            break;
        case PlayerColor::BLUE:
            id = 8;
            break;
        case PlayerColor::RED:
            id = 12;
            break;
    }

    return mPawns[id+which-1];
}
