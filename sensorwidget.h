#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QWidget>
#include <QVector>
#include "node.h"
#include "request.h"

namespace Ui {
class SensorWidget;
}

class QProcess;
class QTreeWidgetItem;
class InfoDialog;
class QSqlQuery;
class QTimer;

class SensorWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SensorWidget(QWidget *parent = 0);
    ~SensorWidget();

signals:
    void startBtnEnabled();
private slots:
    void processLabelClicked(int type, int index);
    void processTreeItemClicked(QTreeWidgetItem* item, int index);
    void processReadyRead();
    void processFinishRead();
    void processStartBtnClicked();
    void processReqTimeout();

private:
    void initDatabase();
    void initReqQueue();
    void initArmQueue();
    void initZigbeeQueue();
    void setupDeviceTree();

    void sendReqest();

    QVector<Request> reqQueue;
    QVector<Node> armQueue;
    QVector<Node> zigbeeQueue;
    bool startBtnFlag;
    int currentReqIndex;
    QProcess *process;
    InfoDialog *infoDialog;
    QSqlQuery *query;
    int armPathCount;
    int zigbeePathCount;

    QTimer *reqTimer;
    enum {
        ARM_NUM = 5,
        ZIGBEE_NUM = 25
    };

    Ui::SensorWidget *ui;

};

#endif // SENSORWIDGET_H
