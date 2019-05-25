#ifndef BOARD_H
#define BOARD_H

#include <string>

#include <QVector>
#include <QPoint>

class Pawn;
enum class PlayerColor;

class Board {
public:

    //Constructor. The players - no. of players playing the game
    Board(unsigned int players);

    //Virtual destructor
    virtual ~Board();

    //Returns the coordinates of a pawn if it's color and relpos were same as given
    static QPoint getPawnCoordinates(PlayerColor color, unsigned int relpos);

    //Returns the coordinates of the pawn passed by pointer. Pawn is not required to be on board
    static QPoint getPawnCoordinates(Pawn* p);

    //Returns a vector, which holds all the Pawns on the board, including the ones won or at home
    QVector<Pawn*> getAllPawns();

    //Returns a vector holding all the Pawns located at the given point on the board
    QVector<Pawn*> getPawnsAt(QPoint point);

    //Returns a pointer to the pawn with the passed id. Ids are unique to all pawns
    //on a board. Returned pawn is supposed to be on the board
    Pawn* getPawnById(unsigned int id);

    //Returns a vector of pawn pointers matching the given color
    QVector<Pawn*> getAllPawnsByColor(PlayerColor color);

    //Returns a pointer to the Pawn identified by it's color and index (1~4)
    //Returns the nullptr if the pawn does not exist
    Pawn* getPawn(PlayerColor color, unsigned int which);

private:
    unsigned int mPlayers {}; //Count of players
    QVector<Pawn*> mPawns {}; //Vector holding pawn handles
};

#endif //Board.h
