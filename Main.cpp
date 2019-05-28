#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QPixmap>
#include <QDebug>

#include <GameWindow.h>
#include <ValueError.h>

#include "ui_welcome.h"
#include "ui_about.h"

GameWindow *gameWin;

void aboutClicked() {
    QDialog *dialog = new QDialog{};
    Ui::AboutDialog aboutDialog {};
    aboutDialog.setupUi(dialog);
    dialog->show();

    QObject::connect(dialog, &QDialog::finished, dialog, [dialog]() {
        delete dialog;
        qDebug() << "Memory freed";
    });
}

void loadClicked() {
    //TODO: Open a file dialog to load save file
}

void startClick(Ui::WelcomeWindow &welcomeWindow) {

    unsigned int players {};

    if (welcomeWindow.radioTwo->isChecked())
        players = 2;
    else if (welcomeWindow.radioThree->isChecked())
        players = 3;
    else if (welcomeWindow.radioFour->isChecked())
        players = 4;

    gameWin = new GameWindow(players);
    gameWin->show();

    //Just for clearing the memory
    QObject::connect(gameWin, &GameWindow::exit, gameWin, [](){
        gameWin->close();
        delete gameWin;
        qDebug() << "Window deleted";
    });
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow welcomeWindow {};
    Ui::WelcomeWindow ui {};

    ui.setupUi(&welcomeWindow);
    ui.logo->setPixmap(QPixmap(QString(":/images/ludo_z.png")).scaledToHeight(ui.logo->height()));
    welcomeWindow.show();

    QObject::connect(ui.startButton, &QPushButton::clicked, &welcomeWindow, [&ui, &welcomeWindow](){
        startClick(ui);
        welcomeWindow.close();
    });

    QObject::connect(ui.aboutButton, &QPushButton::clicked, &welcomeWindow, [](){
        aboutClicked();
    });

    return app.exec();
}
