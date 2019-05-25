#include <string>
#include <iostream>

#include <QApplication>

#include <GameWindow.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        GameWindow window {};
        window.show();
        return app.exec();

    } catch (std::string e) {
        std::cout << e << '\n';
    }

}
