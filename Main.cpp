#include <QApplication>
#include <QString>
#include <QDebug>

#include <GameWindow.h>
#include <ValueError.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        GameWindow window {};
        window.show();
        return app.exec();
    } catch (ValueError *e) {
        qDebug() << e->why() << '\n';
    }

}
