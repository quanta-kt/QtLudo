#include <About.h>

#include <QDebug>

//Simply creates and shows an about dialog
void AboutDialog::show() {
    QDialog *dialog = new QDialog{};
    Ui::AboutDialog aboutDialog {};
    aboutDialog.setupUi(dialog);
    dialog->show();

    QObject::connect(dialog, &QDialog::finished, dialog, [dialog]() {
        delete dialog;
        qDebug() << "Memory freed";
    });
}
