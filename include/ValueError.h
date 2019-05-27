#ifndef VALUE_ERROR_H
#define VALUE_ERROR_H

#include <QException>
#include <QString>

/* This class defines the exception that is throwen across the peoject when a
 * value used or passed to a funtion is invallid */
class ValueError : public QException {
public:

    //Default constructor - empty reason
    ValueError();

    void raise() const override;
    ValueError *clone() const override;

    /*Builds a new value error with passed reason*/
    ValueError(QString why);

    /*Build and throw a new value error*/
    static void raise_new(QString why);

    /*Returns the reason of the exception*/
    QString why();
private:
    QString reason {};
};

#endif //VALUE_ERROR_H
