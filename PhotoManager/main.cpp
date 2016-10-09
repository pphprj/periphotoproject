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
    QTranslator translator;
    bool res = translator.load("mainwindow_ru.qm",".");

    QApplication a(argc, argv);
    res = a.installTranslator(&translator);
    MainWindow w;
    w.show();

    return a.exec();
}
