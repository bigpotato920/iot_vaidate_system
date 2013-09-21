#include "plotdialog.h"
#include "ui_plotdialog.h"

#include <QVector>
#include <QTime>
#include <QSqlQuery>
#include <QPixmap>
#include <QPen>

PlotDialog::PlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(" ");
    dataType = CO2_TYPE;
    connect(this, SIGNAL(rejected()), this, SLOT(processDialogAccept()));
}

PlotDialog::~PlotDialog()
{
    delete ui;
}

void PlotDialog::processDialogAccept()
{
    emit plotDialogAccept();
}

void PlotDialog::setDeviceType(int deviceType)
{
    this->deviceType = deviceType;
}

void PlotDialog::setDeviceIndex(int deviceIndex)
{
    this->deviceIndex = deviceIndex;
}

void PlotDialog::setDataType(int dataType)
{
    this->dataType = dataType;
}

void PlotDialog::setupPlot()
{

    QVector<double> x, y;
    getDataFromDB(y);
    y.insert(0, 0);
    for (int i = 0; i < y.size(); i++) {
        x.push_back(i);
    }

    //ui->customPlot->plotLayout()->insertRow(0);
    //ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, "Sensening Data Co2(ppm)"));
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    QPen pen;
    pen.setColor(QColor(255, 200, 20, 200));
    pen.setStyle(Qt::DashLine);
    pen.setWidthF(2.5);
    ui->customPlot->graph()->setPen(pen);
    ui->customPlot->graph()->setBrush(QBrush(QColor(255,200,20,70)));

    ui->customPlot->graph(0)->setData(x,y);

    switch (dataType) {
    case CO2_TYPE:
        ui->customPlot->yAxis->setLabel("Sensening data Co2(ppm)");
         ui->customPlot->yAxis->setRange(0, 1000);
         break;
    case TMP_TYPE:
         ui->customPlot->yAxis->setLabel("c");
         ui->customPlot->yAxis->setRange(0, 50);
         break;
    case HUM_TYPE:
         ui->customPlot->yAxis->setLabel("%");
         ui->customPlot->yAxis->setRange(0, 100);
         break;
    case ILL_TYPE:
         ui->customPlot->yAxis->setLabel("KLux");
         ui->customPlot->yAxis->setRange(0, 100);
         break;
    }

    ui->customPlot->xAxis->setTicks(false);
    ui->customPlot->xAxis->setTickLabels(false);
    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    qDebug() << "y.size" << y.size();

    ui->customPlot->xAxis->setRange(0, y.size());

    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->replot();
}

void PlotDialog::getDataFromDB(QVector<double> &data)
{
    QSqlQuery query;
    QString sql;


    qDebug() << "device type" << deviceType << "device index" << deviceIndex << "data type" << dataType;
    switch (dataType) {

    case CO2_TYPE:
        sql = QString("select co2 from node where type = %1 and idx = %2  order by create_time")
                .arg(deviceType).arg(deviceIndex);
        break;
    case TMP_TYPE:
        sql = QString("select temperature from node where type = %1 and idx = %2  order by create_time")
                .arg(deviceType).arg(deviceIndex);
        break;
    case HUM_TYPE:
        sql = QString("select humidity from node where type = %1 and idx = %2 order by create_time")
                .arg(deviceType).arg(deviceIndex);
        break;
    case ILL_TYPE:
        sql = QString("select illumination from node where type = %1 and idx = %2  order by create_time")
                .arg(deviceType).arg(deviceIndex);
        break;

    }
    qDebug() << sql;
    query.exec(sql);
    while (query.next()) {
        qDebug() << query.value(0).toInt();
        data.push_back(query.value(0).toInt());
    }

}
