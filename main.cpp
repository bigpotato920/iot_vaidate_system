#include "sensorwidget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    SensorWidget w;
    w.setWindowTitle(" ");
    w.show();
    
    return a.exec();
}
