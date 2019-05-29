#ifndef PAWN_H
#define PAWN_H

#include <QPoint>
#include <QRect>
#include <QWidget>
#include <QPaintEvent>
#include <QString>
#include <QPropertyAnimation>

class Board;
class GameWindow;
class SaveGameEngine;
enum class PlayerColor;

class Pawn : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal glow READ getGlow WRITE setGlow) //Proportion of glow (ranging from 0 to 1)
public:
    //Home (start) position value for relative position of the pawn
    static const int HOME = -1;
    //Destination position value for relative position of the pawn
    static const int DEST = 56;

    //Duration of one glow cycle (expand and condense)
    static const int GLOW_DURATION = 700;

    //Scale of glowing (in range 0 to 1)
    static const qreal GLOW_SCALE;

    //Constructor: Should NOT be used externally (outside the library)
    //board - Pointer to the board which will hold the pawn
    //color - color of the Pawn, id - unique id for identification
    Pawn(PlayerColor color, int id);

    //Constructs a pawns and coppies data from save state
    Pawn(SaveGameEngine *save);

    //Saves data into save state
    void serializeInto(SaveGameEngine *save);

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

    //Returns color specific index for the pawn (refered as 'which' in many functions)
    unsigned int getIndex();

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

    /*Setter for glow property. (refer to glow property declaration)*/
    void setGlow(qreal glow);

    /*Getter for the glow property*/
    qreal getGlow();

    /* Sets usability of pawn. (i.e if it can be clicked)
     * If set to true, it also starts a glow animation which is stopped
     * when the function is called with a false value */
    void setEnabled(bool enabled);

signals:
    /* Triggered when the visual eliment for the the pawn has been clicked. This
     * should happen when the player chooses his pawn to move.
     * It is the duty of the receiver GameWindow to move it's position accordingly
     * and to call Game::playMove() after it */
    void visualClicked(Pawn* p);

    /* Triggered when this pawn is hit by another and needs to go home */
    void clashed(Pawn *p);

private:
    PlayerColor mColor {};
    unsigned int mId {};
    int mPos {}; //The relative position
    qreal mGlow {0}; //current glow proportion

    QPropertyAnimation *glowEffect {nullptr}; //Glow animation

    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
};

#endif //Pawn.h
