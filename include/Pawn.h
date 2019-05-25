#ifndef PAWN_H
#define PAWN_H

#include <string>

#include <QPoint>
#include <QRect>
#include <QPushButton>

class Board;
class GameWindow;
enum class PlayerColor;

class Pawn : public QPushButton {
    Q_OBJECT
public:
    //Home (start) position value for relative position of the pawn
    static const int HOME = -1;
    //Destination position value for relative position of the pawn
    static const int DEST = 58;

    //Constructor: Should NOT be used externally (outside the library)
    //board - Pointer to the board which will hold the pawn
    //color - color of the Pawn, id - unique id for identification
    Pawn(Board* board, PlayerColor color, int id);

    //Virtual destructor
    virtual ~Pawn();

    /* Attaches a GameWindow to the pawn, causing it to display on the GameWindow
     * This also connects pawns signals with GameWindow slots for callbacks
     * NOTE: Only one window can be (and typically, should be) attached at a time! */
    void attatchWindow(GameWindow *w);

    /*Returns the relative position of the pawn ranging from -1 to 58 (HOME TO DEST)*/
    int getRelPosition();

    //Returns the color used by the pawn
    PlayerColor getColor();

    //Returns the unique id of the pawn passed at the time of construction
    unsigned int getId();

    /* Calculates and returns the current position of the pawn on the board
     * based on its relative position as a QPoint */
    QPoint getPositionOnBoard();

    //Returns true if pawn is at home
    bool isAtHome();

    //Returns true if pawn has reached the destination
    bool hasReachedDestination();

    //Sets the relative position of pawn to given one
    void changePosition(int rel);

    //Moves the pawn forward by given steps
    void moveForward(int steps);

    /* Returns the pawn to it's home position, starting it's run over again
     * clash : is it due to a clash? */
    void goBackHome(bool clash = true);

signals:
    /* Triggered when the visual eliment for the the pawn has been clicked. This
     * should happen when the player chooses his pawn to move.
     * It is the duty of the receiver GameWindow to move it's position accordingly
     * and to call Game::playMove() after it */
    void visualClicked(Pawn* p);

    /* Triggered when this pawn is hit by another and needs to go home */
    void clashed(Pawn *p);

private:
    Board* mBoard;
    PlayerColor mColor;
    unsigned int mId;
    int mPos; //The relative position
};

#endif //Pawn.h
