#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class InfoDialog;
}

class PlotDialog;

class InfoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InfoDialog(QWidget *parent = 0);
    void setDeviceImg(QString imgPath);
    void setDataName(QString name);
    void setCo2(QString co2);
    void setTemperature(QString tmp);
    void setHumidity(QString hmd);
    void setIllumination(QString illumination);
    ~InfoDialog();

private slots:
    void processPlotBtnClicked();
    void processPlotDialogAccept();

    void processCo2BtnClicked();
    void processTmpBtnClicked();
    void processHumBtnClicked();
    void processIllBtnClicked();
private:
    PlotDialog *plotDialog;
    QString dataName;
    Ui::InfoDialog *ui;
};

#endif // INFODIALOG_H
