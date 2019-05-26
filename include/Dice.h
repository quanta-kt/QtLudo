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

class GameWindow;

/* This class represents dice visual on the gui
 * NOTE: This is just a widget to show on the window and has nothing to do
 * with dice logic / randomness */
class Dice : public QWidget {
    Q_OBJECT
    Q_PROPERTY(unsigned int value READ getValue WRITE setValue)
    Q_PROPERTY(QColor color READ getColor WRITE setColor)

public:

    static const QColor COLOR_DICE_SECONDARY;

    /* The constructor
     * init_val: the initial value to show on the dice
     * stroke_width: graphical width of the stroke */
    explicit Dice(QWidget *parent = nullptr, qreal stroke_width = 2, unsigned int init_val = 3);

    ~Dice();

    /*Getter and setter for dice face*/
    unsigned int getValue();
    void setValue(unsigned int v);

    /*Getter and setter for dice color*/
    QColor getColor();
    void setColor(QColor c);

    /*Sets the dice size*/
    void setSize(qreal size);
    qreal getSize();

    /* Setter and getter for usability of dice
     * If set to true, clicks are accepted and ignored if false
     * NOTE: It will NOT emit clicked() signals if set to true */
     void setEnabled(bool e);
     bool isEnabled();

signals:
    /*Emitted when dice is clicked*/
    void clicked();

private:
    qreal stroke_width {};
    unsigned int current_value {};
    QColor color {};
    qreal size {};
    bool enabled {true};

    /* Here's where you draw */
    void paintEvent(QPaintEvent *e);

    void mousePressEvent(QMouseEvent *event);
};
#endif //DICE_H
