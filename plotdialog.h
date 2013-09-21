#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <QVector>

namespace Ui {
class PlotDialog;
}

class PlotDialog : public QDialog
{
    Q_OBJECT
    
public:

    enum {
        CO2_TYPE = 0,
        TMP_TYPE = 1,
        HUM_TYPE = 2,
        ILL_TYPE = 3
    };
    explicit PlotDialog(QWidget *parent = 0);
    void setupPlot();
    void setDeviceType(int deviceType);
    void setDeviceIndex(int deviceIndex);
    void setDataType(int dataType);
    ~PlotDialog();
private slots:
    void processDialogAccept();
signals:
    void plotDialogAccept();
private:

    void getDataFromDB(QVector<double> &data);
    int deviceType;
    int deviceIndex;
    int dataType;

    Ui::PlotDialog *ui;
};

#endif // PLOTDIALOG_H
