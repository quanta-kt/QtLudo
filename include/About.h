/* This file helps creating about application dialog */
#ifndef ABOUT_H
#define ABOUT_H

#include "ui_about.h"

namespace AboutDialog {

//Simply creates and shows an about dialog
void show() {
    QDialog *dialog = new QDialog{};
    Ui::AboutDialog aboutDialog {};
    aboutDialog.setupUi(dialog);
    dialog->show();

    QObject::connect(dialog, &QDialog::finished, dialog, [dialog]() {
        delete dialog;
        qDebug() << "Memory freed";
    });
}

#endif //ABOUT_H
