#ifndef PATH_H
#define PATH_H

#include <string>

#include <QPoint>
#include <QVector>

/* This class provides a way to know where exactly on the board a pawn should be
 * placed. It converts a pawn's relative position to cordinates on the board grid
 * NOTE: This class does not computes anything but rather has all the cordinates
 * pre-stored as a static member which is used to return the cordinates */
class Path {
public:
    static const unsigned int MAX_REL; //The maximum value relative position a pawn can have

    /*Returns the pawn's cordinates on the board grid using it's relative position*/
    static QPoint getAbsoluteCordinates(unsigned int rel);
    static QPoint rotatePointToRight(QPoint p);
    static QPoint rotatePointToRight(QPoint p, int times);
private:
    static QVector<QPoint> path; //Data structure to store the coordinates
    static const int TOP = 14; //Total number of cells in a row (or colomn)
};

#endif //Path.h
