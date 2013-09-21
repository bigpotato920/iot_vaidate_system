#include "infodialog.h"
#include "ui_infodialog.h"
#include "plotdialog.h"
#include <QPixmap>
#include <stdio.h>
InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(" ");
    plotDialog = new PlotDialog(this);
    connect(ui->plotBtn, SIGNAL(clicked()), this, SLOT(processPlotBtnClicked()));
    connect(plotDialog, SIGNAL(plotDialogAccept()), this, SLOT(processPlotDialogAccept()));
}

void InfoDialog::setDeviceImg(QString imgPath)
{
    ui->deviceLabel->setPixmap(QPixmap(imgPath));
}

void InfoDialog::setDataName(QString name)
{
    this->dataName = name;
    ui->nameEdit->setText(name);
}

void InfoDialog::setCo2(QString co2)
{
    ui->co2Edit->setText(co2);
}

void InfoDialog::setTemperature(QString tmp)
{
    ui->tmpEdit->setText(tmp);
}

void InfoDialog::setHumidity(QString hmd)
{
    ui->humiEdit->setText(hmd);
}

void InfoDialog::setIllumination(QString illumination)
{
    ui->illuEdit->setText(illumination);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::processPlotBtnClicked()
{
    this->hide();
    int index;
    if (dataName.contains("arm"))
        plotDialog->setDeviceType(0);
    else if (dataName.contains("node"))
        plotDialog->setDeviceType(1);

    sscanf(dataName.toStdString().c_str(), "%*[^/]/%*[^/]/%*[^/]/%d", &index);\
    plotDialog->setDeviceIndex(index-1);
    plotDialog->setupPlot();
    plotDialog->exec();

}

void InfoDialog::processPlotDialogAccept()
{
    this->show();
}

void InfoDialog::processCo2BtnClicked()
{
    plotDialog->setDataType(PlotDialog::CO2_TYPE);
}

void InfoDialog::processTmpBtnClicked()
{
    plotDialog->setDataType(PlotDialog::TMP_TYPE);
}

void InfoDialog::processHumBtnClicked()
{
    plotDialog->setDataType(PlotDialog::HUM_TYPE);
}

void InfoDialog::processIllBtnClicked()
{
    plotDialog->setDataType(PlotDialog::ILL_TYPE);
}
