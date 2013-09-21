#include "sensorwidget.h"
#include "ui_sensorwidget.h"

#include <QLabel>
#include <QVector>
#include <QList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "sqlhelper.h"
#include <QDebug>
#include <QProcess>
#include <QtSql/QSqlQuery>
#include <QPainter>
#include <QPixmap>
#include <QTreeWidgetItem>
#include <QDesktopWidget>
#include <QTimer>

#include "infodialog.h"
SensorWidget::SensorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorWidget)
{
    ui->setupUi(this);

    setupDeviceTree();
    initDatabase();
    initReqQueue();
    initArmQueue();
    initZigbeeQueue();

    process = new QProcess;
    infoDialog = new InfoDialog;
    currentReqIndex = 0;
    startBtnFlag = false;
    armPathCount = 1;
    zigbeePathCount = 1;

    reqTimer = new QTimer();
    reqTimer->setInterval(60 * 1000);
    reqTimer->start();


    connect(ui->mapLabel, SIGNAL(labelClicked(int,int)), this, SLOT(processLabelClicked(int,int)));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processReadyRead()));
    connect(process, SIGNAL(finished(int)), this, SLOT(processFinishRead()));

    connect(ui->co2RadioBtn, SIGNAL(clicked()), infoDialog, SLOT(processCo2BtnClicked()));
    connect(ui->tmpRadioBtn, SIGNAL(clicked()), infoDialog, SLOT(processTmpBtnClicked()));
    connect(ui->humRadioBtn, SIGNAL(clicked()), infoDialog, SLOT(processHumBtnClicked()));
    connect(ui->illuRadioBtn, SIGNAL(clicked()), infoDialog, SLOT(processIllBtnClicked()));

    connect(ui->playBtn, SIGNAL(clicked()), this, SLOT(processStartBtnClicked()));
    connect(reqTimer, SIGNAL(timeout()), this, SLOT(processReqTimeout()));

    QDesktopWidget *desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    sendReqest();
}


SensorWidget::~SensorWidget()
{
    delete ui;
}

//设置设备树
void SensorWidget::setupDeviceTree()
{
   QTreeWidget *root = ui->deviceTree;
   QTreeWidgetItem *pcItemRoot = new QTreeWidgetItem(QStringList(QObject::tr("PC节点")));
   QTreeWidgetItem *armItemRoot = new QTreeWidgetItem(QStringList(QObject::tr("ARM节点")));
   QTreeWidgetItem *zigbeeItemRoot = new QTreeWidgetItem(QStringList(QObject::tr("ZIGBEE节点")));

   QList<QTreeWidgetItem*> armItems;
   QList<QTreeWidgetItem*> zigbeeItems;
   for (int i = 0; i < ARM_NUM; i++) {
       armItems.append(new QTreeWidgetItem(QStringList(QObject::tr("ARM节点%1").arg(i))));

   }
   for (int i = 0; i < ZIGBEE_NUM; i++) {
       zigbeeItems.append(new QTreeWidgetItem(QStringList(QObject::tr("ZIGBEE节点%1").arg(i))));
   }

   armItemRoot->addChildren(armItems);
   zigbeeItemRoot->addChildren(zigbeeItems);

   root->addTopLevelItem(pcItemRoot);
   root->addTopLevelItem(armItemRoot);
   root->addTopLevelItem(zigbeeItemRoot);
   root->expand(root->model()->index(1,0));
   root->expand(root->model()->index(2,0));

   connect(root, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
           this, SLOT(processTreeItemClicked(QTreeWidgetItem*,int)));
}


//初始化数据库,建立连接
void SensorWidget::initDatabase()
{

    bool result = SQLHelper::createConnection();

    if (result)
        qDebug() << "connected";
    query = new QSqlQuery;
}

/**
 * @brief 初始化请求队列
 */
void SensorWidget::initReqQueue()
{
    for (int i = 0; i < ARM_NUM; i++) {
        Request neighborReq(Request::NEIGHBOR_REQ, Request::ARM_NODE, i);
        Request dataReq(Request::DATA_REQ, Request::ARM_NODE, i);
        if (i > 0) {
            Request pathReq(Request::PATH_REQ, Request::ARM_NODE, i);
            reqQueue.push_back(pathReq);
        }

        reqQueue.push_back(neighborReq);
        reqQueue.push_back(dataReq);

    }

    for (int i = 1; i < ZIGBEE_NUM; i++) {
        Request neighborReq(Request::NEIGHBOR_REQ, Request::ZIGBEE_NODE, i);
        Request dataReq(Request::DATA_REQ, Request::ZIGBEE_NODE, i);
        Request pathReq(Request::PATH_REQ, Request::ZIGBEE_NODE, i);
        reqQueue.push_back(neighborReq);
        reqQueue.push_back(dataReq);
        reqQueue.push_back(pathReq);
    }
}


void SensorWidget::initArmQueue()
{
    for (int i = 0; i < ARM_NUM; i++) {
        Node node;
        armQueue.push_back(node);
    }

}


void SensorWidget::initZigbeeQueue()
{
    for (int i = 0; i < ZIGBEE_NUM; i++) {
        Node node;
        zigbeeQueue.push_back(node);
    }
}

void SensorWidget::processTreeItemClicked(QTreeWidgetItem *item, int index)
{

}

//响应节点的点击事件
void SensorWidget::processLabelClicked(int type, int index)
{
    QString arg;
    Node node;
    if (type == 0) {
        arg.sprintf("9floor/Neighbor/arm/%02d", index+1);
        node = armQueue[index];
        infoDialog->setDeviceImg(":/arm_large.png");
    } else if (type == 1) {
        arg.sprintf("9floor/Neighbor/node/%02d", index+1);
        node = zigbeeQueue[index];
        infoDialog->setDeviceImg(":/zigbee_large.png");
    }
    ui->indexEdit->setText(arg);
    ui->valueEdit->setText(QString("Co2:%1ppm, T:%2C H:%3%, I:%4Lux").arg(node.getCo2()).arg(node.getTemperature()).arg(100*node.getHumidity()).arg(node.getIllumination()));
    infoDialog->setDataName(arg);

    infoDialog->setCo2(QString::number(node.getCo2()));
    infoDialog->setTemperature(QString::number(node.getTemperature()));
    infoDialog->setHumidity(QString::number(node.getHumidity()));
    infoDialog->setIllumination(QString::number(node.getIllumination()));
    infoDialog->setModal(true);
    infoDialog->exec();
}


//读取外部程序的标准输出
void SensorWidget::processReadyRead()
{

    int type;
    int index;
    QString result = process->readAllStandardOutput();

    type = reqQueue[currentReqIndex-1].getNodeType();

    index = reqQueue[currentReqIndex-1].getIndex();

    if (result.startsWith("[Data]")) {

        int co2;
        double temperature;
        double humidity;
        double illumination;
        Node node;
        if (type == 0)
            node = armQueue[index];
        else if (type == 1)
            node = zigbeeQueue[index];

        sscanf(result.toStdString().c_str(), "%*[^T]T:%lf H:%lf L:%lf CO2:%d", &temperature,
               &humidity, &illumination, &co2);

        node.setType(type);
        node.setIndex(index);
        node.setCo2(co2);
        node.setTemperature(temperature);
        node.setHumidity(humidity);
        node.setIllumination(illumination);

        if (type == 0)
            armQueue[index] = node;
        else if (type == 1)
            zigbeeQueue[index] = node;
        QString sql = QString("insert into node(type, idx, co2, temperature, humidity, illumination)"
                              " values(%1, %2, %3, %4, %5, %6)")
                .arg(type).arg(index).arg(co2).arg(temperature).arg(humidity).arg(illumination);
        //qDebug() << sql;
        //query->exec(sql);
        ui->debugEdit->append(QString("[Data] T:%1 H:%2 I:%3 C:%4").arg(temperature).arg(humidity).
                              arg(illumination).arg(co2));
    } else if (result.startsWith("[Neighbor]")) {
        MapLabel *mapLabel = ui->mapLabel;
        QStringList argList = result.split(' ');
        argList.removeAt(0);

        if (type == 0) {
            mapLabel->addArmNeighborCount();
        } else if (type == 1) {
            mapLabel->addZigbeeNeighborCount();
        }

        for (int i = 0; i < argList.size(); i++) {

            if (type == 0) {
                mapLabel->setArmMatrix(index+1, argList[i].toInt());

            } else if (type == 1) {
                mapLabel->setZigbeeMatrix(index+1, argList[i].toInt());
            }
        }
        ui->debugEdit->append(QString("[Neighbor] %1 ----> %2").arg(index+1).arg(argList.join(" ")));

    } else if (result.startsWith("[Path]")) {

        MapLabel *mapLabel = ui->mapLabel;
        QStringList argList = result.split(' ');
        argList.removeAt(0);
        int dst[ARM_NUM+ZIGBEE_NUM];

        for (int i = 0; i < argList.size(); i++) {
            dst[i] = argList[i].toInt();
        }


        if (type == 0) {
            mapLabel->setArmPath(index+1, argList.size(), dst);
            armPathCount++;
        } else if (type == 1) {
            mapLabel->setZigbeePath(index+1, argList.size(), dst);
            zigbeePathCount++;
        }
        if (armPathCount == ARM_NUM && zigbeePathCount == ZIGBEE_NUM) {
            ui->playBtn->setEnabled(true);
            emit startBtnEnabled();
        }

        ui->debugEdit->append(QString("[Path] %1 ---> %2").arg(index+1).arg(argList.join(" ")));
    }

    QTextCursor cursor = ui->debugEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->debugEdit->setTextCursor(cursor);
}

//外部程序推出进行相应处理
void SensorWidget::processFinishRead()
{
    sendReqest();
}

void SensorWidget::processStartBtnClicked()
{
    ui->mapLabel->drawMovingNodes();
}

void SensorWidget::processReqTimeout()
{
    qDebug() << "timeout";
    sendReqest();
}


void SensorWidget::sendReqest()
{
    if (currentReqIndex ==  (ZIGBEE_NUM-1)*3 + ARM_NUM*3-1) {
        currentReqIndex = 0;
        return;
    }

    Request req = reqQueue[currentReqIndex];
    QString arg = "9floor/";
    QString index;

    switch(req.getNodeType()) {
    case Request::ARM_NODE:
        arg += "arm/";
        break;
    case Request::ZIGBEE_NODE:
        arg += "node/";
        break;
    }


    switch(req.getReqType()) {

    case Request::NEIGHBOR_REQ:
        arg += "neighbor/";
        break;
    case Request::PATH_REQ:
        arg += "path/";
        break;
    case Request::DATA_REQ:
        arg += "co2/";
        break;
    }

    index.sprintf("%02d", req.getIndex()+1);
    arg += index;
    //qDebug() << arg;
    currentReqIndex++;

    QString qstrExeDir =  "./test";
    QStringList qstrlstParams;
    qstrlstParams << arg;
    process->start(qstrExeDir, qstrlstParams);
}

