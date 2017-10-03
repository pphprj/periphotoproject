#include "mainwindow.h"
#include <QApplication>

void debugMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile outFile("periphotoproject.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QDateTime nowTime = QDateTime::currentDateTime();
    QTextStream textStream(&outFile);
    textStream << nowTime.toString() << " " << msg << endl;
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
