#ifndef DICE_H
#define DICE_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QColor>
#include <QMouseEvent>
#include <QtGlobal>
#include <QtMath>

class GameWindow;

/* This class represents dice visual on the gui
 * NOTE: This is just a widget to show on the window and has nothing to do
 * with dice logic / randomness */
class Dice : public QWidget {
    Q_OBJECT
    Q_PROPERTY(unsigned int value READ getValue WRITE setValue)
    Q_PROPERTY(QColor backColor READ getColor WRITE setColor)

    //Rotates the dice on sceen (rotation measured in degres)
    Q_PROPERTY(qreal rotation READ getRotation WRITE rotate)

public:

    static const QColor COLOR_DICE_SECONDARY;

    /* The constructor
     * init_val: the initial value to show on the dice
     * stroke_width: graphical width of the stroke */
    explicit Dice(QWidget *parent = nullptr, unsigned int init_val = 3);

    ~Dice();

    /*Getter and setter for dice face*/
    unsigned int getValue();
    void setValue(unsigned int v);

    /*Getter and setter for dice color*/
    QColor getColor();
    void setColor(QColor c);

    /* Sets the dice size
     * NOTE: This is not the absolute widget size, it is calculated by adding
     * some "extra space to it"
     * read comments on the defination for more details */
    void setVisualSize(qreal size);
    qreal getVisualSize();

    //Rotates the dice by given degrees
    void rotate(int degrees);
    //Returns the current rotation
    int getRotation();

    /* Setter and getter for usability of dice
     * If set to true, clicks are accepted and ignored if false
     * NOTE: It will NOT emit clicked() signals if set to true */
     void setEnabled(bool e);
     bool isEnabled();

signals:
    /*Emitted when dice is clicked*/
    void clicked();

private:
    unsigned int current_value {};
    QColor backColor {};
    qreal size {};
    bool enabled {true};
    qreal rotation {0}; //Rotation in degrees

    /* Here's where you draw */
    void paintEvent(QPaintEvent *e);

    void mousePressEvent(QMouseEvent *event);
};
#endif //DICE_H
