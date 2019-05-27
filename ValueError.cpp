#include <ValueError.h>

ValueError::ValueError() : reason (QString("")) {}

ValueError::ValueError(QString why) : reason {why} {}

void ValueError::raise() const {
    throw this;
}

ValueError* ValueError::clone() const {
    return new ValueError(*this);
}

void ValueError::raise_new(QString why) {
    (new ValueError(why))->raise();
}

QString ValueError::why() {
    return this->reason;
}
