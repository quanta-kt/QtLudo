#include <paint_helper.h>

#include <PlayerColor.h>
#include <GameWindow.h>
#include <ValueError.h>

#include <QDebug>

namespace painthelp {

    //Return the absolute rectangular area on screen of the cell by it's x and y cordinates
    QRect getCellRect(int x, int y) {
        return QRect(
            GameWindow::BOARD_BOUND + (x * GameWindow::CELL_SIZE),
            GameWindow::BOARD_BOUND + (y * GameWindow::CELL_SIZE),
            GameWindow::CELL_SIZE, GameWindow::CELL_SIZE);
    }

    //Eqvi. to getCellRect(int, int) but uses QPoint as param instead
    QRect getCellRect(QPoint cell) {
        return getCellRect(cell.x(), cell.y());
    }

    //Returns the rectangular region covering the home of red player
    QRect getRedHomeRect() {
        return QRect (
            GameWindow::BOARD_BOUND,
            GameWindow::BOARD_BOUND,
            GameWindow::CELL_SIZE * 6,
            GameWindow::CELL_SIZE * 6
        );
    }

    //Returns the rectangular region covering the home of yellow player
    QRect getYellowHomeRect() {
        QRect cell = getCellRect(9, 0);
        return QRect (
            cell.x(), cell.y(),
            GameWindow::CELL_SIZE * 6,
            GameWindow::CELL_SIZE * 6
        );
    }

    //Returns the rectangular region covering the home of blue player
    QRect getBlueHomeRect() {
        QRect cell = getCellRect(9, 9);
        return QRect (
            cell.x(), cell.y(),
            GameWindow::CELL_SIZE * 6,
            GameWindow::CELL_SIZE * 6
        );
    }

    //Returns the rectangular region covering the home of green player
    QRect getGreenHomeRect() {
        QRect cell = getCellRect(0, 9);
        return QRect (
            cell.x(), cell.y(),
            GameWindow::CELL_SIZE * 6,
            GameWindow::CELL_SIZE * 6
        );
    }

    //Returns the rectangular region as QRect covering the destination box
    QRect getDestRect() {
        QRect cell = getCellRect(6, 6);
        return QRect (
            cell.x(), cell.y(),
            GameWindow::CELL_SIZE * 3,
            GameWindow::CELL_SIZE * 3
        );
    }

    //Shifts the rect's corner to center of itself
    QRect shiftToCenter(QRect it) {
        return QRect (
            it.x() + (GameWindow::CELL_SIZE / 2),
            it.y() + (GameWindow::CELL_SIZE / 2),
            GameWindow::CELL_SIZE,
            GameWindow::CELL_SIZE
        );
    }

    //Returns the vector of rects where white circles inside homes are to be drawn
    QVector<QRect> getHomeCircleRects() {
        return QVector<QRect> {
            shiftToCenter(getCellRect(1, 1)),
            shiftToCenter(getCellRect(1, 3)),
            shiftToCenter(getCellRect(3, 1)),
            shiftToCenter(getCellRect(3, 3)),

            shiftToCenter(getCellRect(10, 1)),
            shiftToCenter(getCellRect(12, 1)),
            shiftToCenter(getCellRect(10, 3)),
            shiftToCenter(getCellRect(12, 3)),

            shiftToCenter(getCellRect(10, 10)),
            shiftToCenter(getCellRect(12, 10)),
            shiftToCenter(getCellRect(10, 12)),
            shiftToCenter(getCellRect(12, 12)),

            shiftToCenter(getCellRect(1, 10)),
            shiftToCenter(getCellRect(1, 12)),
            shiftToCenter(getCellRect(3, 10)),
            shiftToCenter(getCellRect(3, 12)),
        };
    }

    QRect getPawnHomePosGeometry(PlayerColor color, int which) {
        if(which < 1 || which > 4)
            ValueError::raise_new(QString("Invalid value for which : %1").arg(which));

        QVector<QRect> v = getHomeCircleRects();
        switch (color) {
            case PlayerColor::RED:
                return getPawnGeometry(v[which-1]);
            case PlayerColor::YELLOW:
                return getPawnGeometry(v[which+3]);
            case PlayerColor::BLUE:
                return getPawnGeometry(v[which+7]);
            case PlayerColor::GREEN:
                return getPawnGeometry(v[which+11]);

            //This should never happen.
            default:
                ValueError::raise_new(QString("paint_helper.cpp:getPawnHomePos : \
                Critical Error (no enum value fell for a switch)"));
        }
        #pragma GCC diagnostic ignored "-Wreturn-type"
    }
    #pragma GCC diagnostic warning "-Wreturn-type"

    /* Returns the geometry that a pawn is supposed to have according to the
     * cell it is in right now. Currently, it has same effect as calling 'getCellRect'
     * but MAY differ in future due to modifications. The size of the pawn is
     * handled internally by Pawn class itself */
    QRect getPawnGeometry(QRect cellRect) {
        return {
            cellRect.x(),
            cellRect.y(),
            GameWindow::CELL_SIZE, GameWindow::CELL_SIZE
        };
    }

    QRect getPawnGeometry(int x, int y) {
        return getPawnGeometry(getCellRect(x, y));
    }

    QRect getPawnGeometry(QPoint pos) {
        return getPawnGeometry(pos.x(), pos.y());
    }

    //Returns the point adjecent to the given point, lieing below it
    QPoint pointBelow(QPoint p) {
        return QPoint {p.x(), p.y()+1};
    }

    //Returns the point adjecent to the given point, lieing right to it
    QPoint pointRight(QPoint p) {
        return QPoint {p.x()+1, p.y()};
    }

    //Returns the point adjecent to the given point, lieing above it
    QPoint pointAbove(QPoint p) {
        return QPoint {p.x(), p.y()-1};
    }

    //Returns the point adjecent to the given point, lieing left to it
    QPoint pointLeft(QPoint p) {
        return QPoint {p.x()-1, p.y()};
    }

    //Returns the QRect on the cell where guider dots are to be drawn
    QRect getGuiderRegion(QRect cell) {

        QPoint center = cell.center();

        qDebug() << cell.x() << cell.y();
        qDebug() << center.x() << cell.y();

        QPoint tl = {
            center.x() - ((GameWindow::GUIDER_DOT_SIZE / 2) + (GameWindow::STROKE_WIDTH * 2)),
            center.x() - ((GameWindow::GUIDER_DOT_SIZE / 2) + (GameWindow::STROKE_WIDTH * 2))
        };
        QPoint br = {
            center.x() + ((GameWindow::GUIDER_DOT_SIZE / 2) + (GameWindow::STROKE_WIDTH * 2)),
            center.x() + ((GameWindow::GUIDER_DOT_SIZE / 2) + (GameWindow::STROKE_WIDTH * 2))
        };

        qDebug() << cell.x() << cell.y();
        qDebug() << center.x() << center.y();
        return QRect {tl, br};
    }

}
