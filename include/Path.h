#ifndef PATH_H
#define PATH_H

#include <string>

#include <QPoint>
#include <QVector>

class Path {
public:
    static const unsigned int MAX_REL;

    static QPoint getAbsoluteCordinates(unsigned int rel);
    static QPoint rotatePointToRight(QPoint p);
    static QPoint rotatePointToRight(QPoint p, int times);
private:
    static QVector<QPoint> path;
    static const int TOP = 14;
};

#endif //Path.h
