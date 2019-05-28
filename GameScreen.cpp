#include <GameScreen.h>

#include <GameWindow.h>

GameScreen::GameScreen(GameWindow *parent) : QWidget(parent) {}

GameScreen::~GameScreen() {}

void GameScreen::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush backBrush (GameWindow::BG_COLOR);
    painter.setBrush(backBrush);

    QRect outer(GameWindow::BOARD_BOUND, GameWindow::BOARD_BOUND, GameWindow::CELL_SIZE * 15, GameWindow::CELL_SIZE * 15);
    painter.drawRect(outer);

    painter.setPen(QPen(QBrush(GameWindow::STROKE_COLOR), GameWindow::STROKE_WIDTH));

    //Printing the grids (Like some little kid's math notebook, lmao!)
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            QRect g (GameWindow::BOARD_BOUND + (i * GameWindow::CELL_SIZE), GameWindow::BOARD_BOUND + (j * GameWindow::CELL_SIZE), GameWindow::CELL_SIZE, GameWindow::CELL_SIZE);
            painter.drawRect(g);
        }
    }

    painter.drawRect(painthelp::getDestRect());

    drawHomes(painter);
    drawGuidePaths(painter);
}

//Draws homes for all players
void GameScreen::drawHomes(QPainter &painter) {
    QBrush redBrush (GameWindow::COLOR_RED);
    painter.setBrush(redBrush);
    painter.drawRect(painthelp::getRedHomeRect());

    QBrush yellowBrush (GameWindow::COLOR_YELLOW);
    painter.setBrush(yellowBrush);
    painter.drawRect(painthelp::getYellowHomeRect());

    QBrush blueBrush (GameWindow::COLOR_BLUE);
    painter.setBrush(blueBrush);
    painter.drawRect(painthelp::getBlueHomeRect());

    QBrush greenBrush (GameWindow::COLOR_GREEN);
    painter.setBrush(greenBrush);
    painter.drawRect(painthelp::getGreenHomeRect());

    //Draw four white circles on each home which holds the pawns
    painter.setBrush(QBrush(GameWindow::BG_COLOR));
    for (auto e : painthelp::getHomeCircleRects())
        painter.drawEllipse(e);
}


//Colors some proper cells to guide the pawns
void GameScreen::drawGuidePaths(QPainter &painter) {
    QPoint redg {1, 6};
    painter.setBrush(GameWindow::COLOR_RED);
    painter.drawEllipse(painthelp::getCellRect(redg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    redg = painthelp::pointBelow(redg);
    painter.drawEllipse(painthelp::getCellRect(redg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);

    for (int i = 0; i < 4; i++) {
        redg = painthelp::pointRight(redg);
        painter.drawEllipse(painthelp::getCellRect(redg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    }

    QPoint yellowg {8, 1};
    painter.setBrush(GameWindow::COLOR_YELLOW);
    painter.drawEllipse(painthelp::getCellRect(yellowg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    yellowg = painthelp::pointLeft(yellowg);
    painter.drawEllipse(painthelp::getCellRect(yellowg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);

    for (int i = 0; i < 4; i++) {
        yellowg = painthelp::pointBelow(yellowg);
        painter.drawEllipse(painthelp::getCellRect(yellowg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    }

    QPoint blueg {13, 8};
    painter.setBrush(GameWindow::COLOR_BLUE);
    painter.drawEllipse(painthelp::getCellRect(blueg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    blueg = painthelp::pointAbove(blueg);
    painter.drawEllipse(painthelp::getCellRect(blueg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);

    for (int i = 0; i < 4; i++) {
        blueg = painthelp::pointLeft(blueg);
        painter.drawEllipse(painthelp::getCellRect(blueg).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    }

    QPoint greeng {6, 13};
    painter.setBrush(GameWindow::COLOR_GREEN);
    painter.drawEllipse(painthelp::getCellRect(greeng).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    greeng = painthelp::pointRight(greeng);
    painter.drawEllipse(painthelp::getCellRect(greeng).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);

    for (int i = 0; i < 4; i++) {
        greeng = painthelp::pointAbove(greeng);
        painter.drawEllipse(painthelp::getCellRect(greeng).center(), GameWindow::GUIDER_DOT_SIZE, GameWindow::GUIDER_DOT_SIZE);
    }
}
