#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <QObject>
#include <QPaintEvent>
#include <QWidget>

#include <paint_helper.h>

class GameWindow;

/* This class serves as GameWindow's central widget */
class GameScreen : public QWidget {
Q_OBJECT;

public:
    //Constructs a new object taking a GameWindow as parent
    explicit GameScreen(GameWindow *parent);

    virtual ~GameScreen();

private:
    //Just helper functions
    void drawHomes(QPainter &painter);
    void drawGuidePaths(QPainter &painter);

protected:
    virtual void paintEvent(QPaintEvent*);
};

#endif
