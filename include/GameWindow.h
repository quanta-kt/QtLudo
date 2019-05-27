#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QString>
#include <QObject>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QTimer>

class Game;
class Board;
class Pawn;
class Dice;
enum class PlayerColor;

class GameWindow : public QWidget {
    Q_OBJECT;
public:
    /* Defines game state.
     * Rolling : dice needs to be rolled.
     * MOVING : player needs to choose a pawn to move
     * ANIMATING : An animation is playing */
    enum GameState {ROLLING, MOVING, ANIMATING};

    //Colors for each player, background and stroke
    static const QColor COLOR_RED;
    static const QColor COLOR_YELLOW;
    static const QColor COLOR_BLUE;
    static const QColor COLOR_GREEN;
    static const QColor BG_COLOR;
    static const QColor STROKE_COLOR;

    //Color for dice background
    static const QColor COLOR_RED_LIGHT;
    static const QColor COLOR_YELLOW_LIGHT;
    static const QColor COLOR_BLUE_LIGHT;
    static const QColor COLOR_GREEN_LIGHT;

    //Space to leave on all sides, apart from board
    static const int BOARD_BOUND = 5;

    //Width and height of each cell
    static const int CELL_SIZE = 40;

    //guider-dot size
    static const int GUIDER_DOT_SIZE = 4;

    //Pawn visual size
    static const int PAWN_SIZE = 25;

    //Dice size
    static const int DICE_SIZE = 50;

    //Stroke size
    static const int STROKE_WIDTH = 2;

    //Time taken for animating single step move of a pawn
    static const int ANIMATION_DURATION = 120;

    //Time taken for dice roll animation
    static const int DICE_ANIMATION_DURATION = 800;

    //Distance the dice is shaken
    static const int DICE_SHAKE_DISTANCE = 25;

    //Constructor and destructor
    explicit GameWindow();
    ~GameWindow();

    /*Returns a user-friendly string representation of user name*/
    QString getUserName(PlayerColor color);

    /*Properly updates the UI according to the value of `state` and current player*/
    void updateUi();

protected:
    void paintEvent(QPaintEvent *e);

public slots:
    void rollDiceClicked();
    void pawnChosen(Pawn *p);

    /* Called when a pawn has clashed and needs to go back home
     * pawn: the pawn which is to be sent home */
    void pawnClashed(Pawn *pawn);

    /* Called when the pawn visual animation is finished */
    void pawnAnimationFinished();

    /* Called when the dice roll animation is finished */
    void diceAnimationFinished();

private:
    unsigned int players_count {}; //No of players
    Game *mGame {}; //current game
    Board *mBoard {};
    GameState state {};
    Pawn* currentPawn {}; //Stores pawn which is currently playing (for use of pawnAnimationFinished())

    QWidget *footer {}; //Widget for user-interaction
    QVBoxLayout *footerLayout {}; //Layout for footer
    Dice *dice; //Dice control
    QLabel *hintLabel {}; //Hint text
    QSequentialAnimationGroup *animationGroup {}; //Pawn animation
    QParallelAnimationGroup *diceAnimationGroup {}; //Dice Rolling animation

    /*Causes the pawn visual to move*/
    void movePawnVisual(Pawn *p, int oldpos);

    //Moves the pawn visuals through an animation
    void animateVisualMovement(Pawn* p, int startRel);

    //Animates the dice before showing of it's value
    void animateDiceRoll();

    //Just helper functions
    void drawHomes(QPainter &painter);
    void drawGuidePaths(QPainter &painter);
};
#endif //GameWindow.h
