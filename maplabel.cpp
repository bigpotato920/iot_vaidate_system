#include "maplabel.h"
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPoint>
#include <QPushButton>

#include <QDebug>

MapLabel::MapLabel(QWidget *parent) :
    QLabel(parent)
{

    for (int i = 0; i < ARM_NUM; i++) {
        for (int j = 0; j < ARM_NUM; j++)
            armMatrix[i][j] = 0;
    }

    for (int i = 0; i < ZIGBEE_NUM; i++) {
        for (int j = 0; j < ZIGBEE_NUM; j++)
            zigbeeMatrix[i][j] = 0;
    }

    for (int i = 0; i < ARM_NUM; i++) {
        for (int j = 0; j < ARM_NUM; j++)
            armPath[i][j] = 0;
    }

    for (int i = 0; i < ZIGBEE_NUM; i++) {
        for (int j = 0; j < ZIGBEE_NUM; j++)
            zigbeePath[i][j] = 0;
    }

    armNeighborCount =0;
    zigbeeNeighborCount = 0;
    armPathCount = 1;
    zigbeePathCount = 1;

    setupUi();
    setLabelPos();
    drawPoints();

}

/**
 * @brief 设置ARM节点的邻居表
 */
void MapLabel::setArmMatrix(int i, int j)
{
    armMatrix[i-1][j-1] = 1;

    if (armNeighborCount == ARM_NUM) {
        //this->repaint();
        armNeighborCount = 0;
    }

}

/**
 * @brief 设置ZIGBEE节点的邻居表
 */
void MapLabel::setZigbeeMatrix(int i, int j)
{
    zigbeeMatrix[i-1][j-1] = 1;
    if (i-1 >= ZIGBEE_NUM || j-1 >= ZIGBEE_NUM) {
         qDebug() << "buggy" << i-1 << j-1;
    }

    if (zigbeeNeighborCount == ZIGBEE_NUM-1) {

        this->repaint();
        zigbeeNeighborCount = 0;
    }


}

void MapLabel::addArmNeighborCount()
{
    armNeighborCount++;

}

void MapLabel::addZigbeeNeighborCount()
{
    zigbeeNeighborCount++;

}

void MapLabel::setArmPath(int src, int n, int *dst)
{
    for (int i = 0; i < n; i++) {
        armPath[src-1][i] = dst[i]-1;
    }
    armPathCount++;
    if (armPathCount == ARM_NUM) {
        armPathCount = 1;
    }
}

void MapLabel::setZigbeePath(int src, int n, int *dst)
{
    for (int i = 0; i < n; i++) {
        zigbeePath[src-1][i] = dst[i]-1;

    }
    zigbeePathCount++;
    if (zigbeePathCount == ZIGBEE_NUM) {
        zigbeePathCount = 1;
    }
}




void MapLabel::setupUi()
{
    bgMap = new QPixmap(":/map.jpg");

    for (int i = 0; i < ARM_NUM; i++) {
        armMovingNodes[i] = new QLabel(this);
        armMovingNodes[i]->setPixmap(QPixmap(":/bullet_red.png"));
        armMovingNodes[i]->move(-40, -40);
        armAnimations[i] = new QPropertyAnimation(armMovingNodes[i], "pos");
        connect(armAnimations[i], SIGNAL(finished()), this, SLOT(processAnimationFinished()));
    }

    for (int i = 0; i < ZIGBEE_NUM; i++) {
        zigbeeMovingNodes[i] = new QLabel(this);
        zigbeeMovingNodes[i]->setPixmap(QPixmap(":/bullet_blue.png"));
        zigbeeMovingNodes[i]->move(-40, -40);
        zigbeeAnimations[i] = new QPropertyAnimation(zigbeeMovingNodes[i], "pos");
        connect(zigbeeAnimations[i], SIGNAL(finished()), this, SLOT(processAnimationFinished()));
    }
}

/**
 * @brief 设置地图上点的位置
 */
void MapLabel::setLabelPos()
{
    armPos[0] = QPoint(50, 360);
    armPos[1] = QPoint(150, 610);
    armPos[2] = QPoint(430, 480);
    armPos[3] = QPoint(700, 550);
    armPos[4] = QPoint(710, 150);

    zigbeePos[0] = QPoint(60, 370);
    zigbeePos[1] = QPoint(80, 450);
    zigbeePos[2] = QPoint(60, 600);
    zigbeePos[3] = QPoint(220, 620);
    zigbeePos[4] = QPoint(170, 450);
    zigbeePos[5] = QPoint(180, 360);
    zigbeePos[6] = QPoint(170, 260);
    zigbeePos[7] = QPoint(40, 100);
    zigbeePos[8] = QPoint(190, 130);
    zigbeePos[9] = QPoint(270, 230);
    zigbeePos[10] = QPoint(320, 140);
    zigbeePos[11] = QPoint(430, 200);
    zigbeePos[12] = QPoint(460, 100);
    zigbeePos[13] = QPoint(520, 220);
    zigbeePos[14] = QPoint(590, 110);
    zigbeePos[15] = QPoint(690, 230);
    zigbeePos[16] = QPoint(640, 350);
    zigbeePos[17] = QPoint(620, 450);
    zigbeePos[18] = QPoint(670, 580);
    zigbeePos[19] = QPoint(600, 510);
    zigbeePos[20] = QPoint(530, 600);
    zigbeePos[21] = QPoint(490, 520);
    zigbeePos[22] = QPoint(410, 590);
    zigbeePos[23] = QPoint(350, 500);
    zigbeePos[24] = QPoint(270, 550);
}

/**
 * @brief 绘制地图上的节点并为节点设置事件过滤
 */
void MapLabel::drawPoints()
{
    for (int i = 0; i < ARM_NUM; i++) {
        QLabel *armLabel = new QLabel(this);
        armLabel->setPixmap(QPixmap(":/arm_small.png"));
        armLabel->installEventFilter(this);

        armLabels.push_back(armLabel);
    }

    for (int i = 0; i < ZIGBEE_NUM; i++) {
        QLabel *zigbeeLabel = new QLabel(this);
        zigbeeLabel->setPixmap(QPixmap(":/zigbee_small.png"));
        zigbeeLabel->installEventFilter(this);

        zigbeeLabels.push_back(zigbeeLabel);
    }

    for (int i = 0; i < ARM_NUM; i++)
        armLabels[i]->move(armPos[i].x(), armPos[i].y());

    for (int i = 0; i< ZIGBEE_NUM; i++)
        zigbeeLabels[i]->move(zigbeePos[i].x(), zigbeePos[i].y());
}

/**
 * @brief 根据节点上传的邻居表绘制拓扑图
 */
void MapLabel::drawLines(QPainter *painter)
{

    int x = armLabels[0]->width() / 2;
    int y = armLabels[0]->height() / 2;
    //painter->setPen(QPen(Qt::black, 3, Qt::DotLine, Qt::RoundCap));
    painter->setPen(QPen(Qt::black, 2,Qt::DotLine, Qt::RoundCap));
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->drawLine(armPos[1]+QPoint(x,y), armPos[0]+QPoint(x,y));
    for (int i = 1; i < ARM_NUM; i++) {
        for (int j = i+1; j < ARM_NUM; j++) {
            if (armMatrix[i][j] == 1) {
                painter->drawLine(armPos[i]+QPoint(x,y), armPos[j]+QPoint(x,y));
            }
        }
    }

    painter->setPen(QPen(Qt::gray, 2,Qt::DotLine, Qt::RoundCap));
    painter->drawLine(zigbeePos[1]+QPoint(x,y), zigbeePos[0]+QPoint(x,y));
    for (int i = 1; i < ZIGBEE_NUM; i++) {
        for (int j = i+1; j < ZIGBEE_NUM; j++) {
            if (zigbeeMatrix[i][j] == 1) {
                painter->drawLine(zigbeePos[i]+QPoint(x,y), zigbeePos[j]+QPoint(x,y));
            }
        }
    }

}

void MapLabel::drawArmMovingNodes()
{
    int hopCount = 1;

    for (int i = 1; i < ARM_NUM; i++) {

        for (int j = 0; j < ARM_NUM; j++) {
            if (armPath[i][j] > 0)
                hopCount++;
        }

        armAnimations[i]->setDuration(hopCount*2000);
        armAnimations[i]->setStartValue(armPos[i]);

        for (int j = 0; j < hopCount-1; j++) {
            armAnimations[i]->setKeyValueAt((j+1)*1.0/hopCount, armPos[armPath[i][j]]);

        }

        armAnimations[i]->setEndValue(armPos[0]);
        armAnimations[i]->setLoopCount(1);
        armAnimations[i]->start();
        hopCount = 1;
    }

}

void MapLabel::drawZigbeeMovingNodes()
{

    int hopCount = 1;
    for (int i = 1; i < ZIGBEE_NUM; i++) {

        for (int j = 0; j < ZIGBEE_NUM; j++) {
            if (zigbeePath[i][j] > 0)
                hopCount++;
        }

        zigbeeAnimations[i]->setDuration(hopCount*2000);
        zigbeeAnimations[i]->setStartValue(zigbeePos[i]);

        for (int j = 0; j < hopCount-1; j++) {
            zigbeeAnimations[i]->setKeyValueAt((j+1)*1.0/hopCount, zigbeePos[zigbeePath[i][j]]);
        }
        zigbeeAnimations[i]->setEndValue(zigbeePos[0]);
        zigbeeAnimations[i]->setLoopCount(1);
        zigbeeAnimations[i]->start();
        hopCount = 1;
    }

}


void MapLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //对pixmap进行放大
    QPixmap tmp;
    tmp = bgMap->scaled(this->width(), this->height());
    painter.drawPixmap(0, 0, tmp);

   drawLines(&painter);
}

/**
 * @brief 响应节点的点击事件
 */
bool MapLabel::eventFilter(QObject *obj, QEvent *event)
{


    for (int i = 0; i < ARM_NUM; i++) {
          if (obj == armLabels[i]) {
              if (event->type() == QEvent::MouseButtonPress) {

                  emit labelClicked(0, i);
                  return true;
              } else if (event->type() == QEvent::MouseTrackingChange){
                  qDebug() << "move";
                  return true;
              } else {
                  return false;
              }
          }
      }

      for (int i = 0; i < ZIGBEE_NUM; i++) {
          if (obj == zigbeeLabels[i]) {
              if (event->type() == QEvent::MouseButtonPress) {

                  emit labelClicked(1, i);
                  return true;
              } else if (event->type() == QEvent::MouseTrackingChange){
                  qDebug() << "move";
                  return true;
              } else {
                  return false;
              }
          }
      }
      return QWidget::eventFilter(obj, event);
}


void MapLabel::drawMovingNodes()
{
    drawArmMovingNodes();
    drawZigbeeMovingNodes();
}


void MapLabel::processAnimationFinished()
{
    QObject *obj = sender();

    for (int i = 0; i < ARM_NUM; i++) {
        if (obj == armAnimations[i]) {
            armMovingNodes[i]->move(-40, -40);
        }
    }
    for (int i = 0; i < ZIGBEE_NUM; i++) {
        if (obj == zigbeeAnimations[i]) {
            zigbeeMovingNodes[i]->move(-40, -40);
        }
    }
}



