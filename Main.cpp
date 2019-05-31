#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QPixmap>
#include <QDebug>

#include <GameWindow.h>
#include <ValueError.h>
#include <About.h>

#include <paint_helper.h>
#include <SaveGameEngine.h>

#include "ui_welcome.h"

void aboutClicked() {
    AboutDialog::show();
}

void loadClicked(QMainWindow &welcomeWindow) {
     GameWindow *gameWin;

    QString filename = QFileDialog::getOpenFileName(
        &welcomeWindow,
        QString("Load Game"),
        QString(),
        QString("Ludo Z+ game save (*.lzs)"));

    if(filename == "")
        return; //Canceled

    SaveGameEngine *saveState = SaveGameEngine::deserialize(filename);

    gameWin = new GameWindow(saveState);
    gameWin->show();

    welcomeWindow.close();

    //Just for clearing the memory
    QObject::connect(gameWin, &GameWindow::exit, gameWin, [gameWin](){
        gameWin->close();
        delete gameWin;
        qDebug() << "Window deleted";
    });
}

void startClick(Ui::WelcomeWindow &ui, QMainWindow &welcomeWindow) {
    GameWindow *gameWin;

    unsigned int players {};

    if (ui.radioTwo->isChecked())
        players = 2;
    else if (ui.radioThree->isChecked())
        players = 3;
    else if (ui.radioFour->isChecked())
        players = 4;

    gameWin = new GameWindow(players);
    gameWin->show();
    welcomeWindow.close();

    //Just for clearing the memory
    QObject::connect(gameWin, &GameWindow::exit, gameWin, [gameWin](){
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
        startClick(ui, welcomeWindow);
    });

    QObject::connect(ui.loadButton, &QPushButton::clicked, &welcomeWindow, [&welcomeWindow]() {
        loadClicked(welcomeWindow);
    });

    QObject::connect(ui.aboutButton, &QPushButton::clicked, &welcomeWindow, [](){
        aboutClicked();
    });

    //Test
    QRect cr = painthelp::getCellRect(0,0);
    qDebug() << cr.x() << cr.y();
    QRect g = painthelp::getPawnGeometry(cr);
    qDebug() << g.x() << g.y();

    try {
        return app.exec();
    } catch (ValueError *e) {
        qDebug() << e->why();
    }
}
