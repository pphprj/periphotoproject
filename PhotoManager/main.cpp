#include "mainwindow.h"
#include <QApplication>

void debugMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile outFile("periphotoproject.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream textStream(&outFile);
    textStream << msg << endl;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(debugMessageOutput);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
