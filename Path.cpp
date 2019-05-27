#include <Path.h>

#include <QString>

#include <ValueError.h>

QVector<QPoint> Path::path = {
    QPoint(1, 6),
    QPoint(2, 6),
    QPoint(3, 6),
    QPoint(4, 6),
    QPoint(5, 6),

    QPoint(6, 5),
    QPoint(6, 4),
    QPoint(6, 3),
    QPoint(6, 2),
    QPoint(6, 1),
    QPoint(6, 0),

    QPoint(7, 0),
    QPoint(8, 0),

    QPoint(8, 1),
    QPoint(8, 2),
    QPoint(8, 3),
    QPoint(8, 4),
    QPoint(8, 5),

    QPoint(9, 6),
    QPoint(10, 6),
    QPoint(11, 6),
    QPoint(12, 6),
    QPoint(13, 6),
    QPoint(14, 6),

    QPoint(14, 7),
    QPoint(14, 8),

    QPoint(13, 8),
    QPoint(12, 8),
    QPoint(11, 8),
    QPoint(10, 8),
    QPoint(9, 8),

    QPoint(8, 9),
    QPoint(8, 10),
    QPoint(8, 11),
    QPoint(8, 12),
    QPoint(8, 13),
    QPoint(8, 14),

    QPoint(7, 14),
    QPoint(6, 14),

    QPoint(6, 13),
    QPoint(6, 12),
    QPoint(6, 11),
    QPoint(6, 10),
    QPoint(6, 9),

    QPoint(5, 8),
    QPoint(4, 8),
    QPoint(3, 8),
    QPoint(2, 8),
    QPoint(1, 8),
    QPoint(0, 8),

    QPoint(0, 7),
    QPoint(1, 7),
    QPoint(2, 7),
    QPoint(3, 7),
    QPoint(4, 7),
    QPoint(5, 7),
};

const unsigned int Path::MAX_REL = Path::path.size() - 1;

QPoint Path::getAbsoluteCordinates(unsigned int rel) {
    if(rel >= (unsigned int)path.size())
        ValueError::raise_new(QString("Path::getAbsoluteCordinates(unsigned long int) \
        : Invalid argument : rel == %1").arg(rel));

    return path[rel];
}

QPoint Path::rotatePointToRight(QPoint point) {
    return QPoint {TOP-point.y(), point.x()};
};

QPoint Path::rotatePointToRight(QPoint point, int times) {
    if (times == 0)
        return point;
    return rotatePointToRight(rotatePointToRight(point), times-1);
};
