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

class Game;
class Board;
class Pawn;
enum class PlayerColor;

class GameWindow : public QWidget {
    Q_OBJECT;
public:
    enum GameState {ROLLING, MOVING, ANIMATING}; //Defines game state. Rolling : dice needs to be rolled. MOVING : player needs to choose a pawn to move

    //Colors for each player, background and stroke
    static const QColor COLOR_RED;
    static const QColor COLOR_YELLOW;
    static const QColor COLOR_BLUE;
    static const QColor COLOR_GREEN;
    static const QColor BG_COLOR;
    static const QColor STROKE_COLOR;

    //Space to leave on all sides, apart from board
    static const int BOARD_BOUND = 5;
    //Width and height of each cell
    static const int CELL_SIZE = 40;
    //guider-dot size
    static const int GUIDER_DOT_SIZE = 8;
    //Pawn visual size
    static const int PAWN_SIZE = 26;
    //Stroke size
    static const int STROKE_WIDTH = 2;

    static const int ANIMATION_DURATION = 120;

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

private:
    unsigned int players_count {}; //No of players
    Game *mGame {}; //current game
    Board *mBoard {};
    GameState state {};
    Pawn* currentPawn {}; //Stores pawn which is currently playing (for use of pawnAnimationFinished())

    QWidget *footer {}; //Widget for user-interaction
    QVBoxLayout *footerLayout {}; //Layout for footer
    QPushButton *diceButton {}; //Button to roll the dice
    QLabel *hintLabel {}; //Hint text
    QSequentialAnimationGroup *animationGroup {};
    QPropertyAnimation *animation {};

    /*Causes the pawn visual to move*/
    void movePawnVisual(Pawn *p, int oldpos);

    //Moves the pawn visuals through an animation
    void animateVisualMovement(Pawn* p, int startRel);

    //Just helper functions
    void drawHomes(QPainter &painter);
    void drawGuidePaths(QPainter &painter);
};
#endif //GameWindow.h
