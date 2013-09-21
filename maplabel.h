#ifndef MAPLABEL_H
#define MAPLABEL_H

#include <QLabel>
#include <QPoint>
#include <QVector>

class QPixmap;
class QPropertyAnimation;


class MapLabel : public QLabel
{
    Q_OBJECT


signals:
    void labelClicked(int type, int index);
    void animationComplete();

private slots:
    void processAnimationFinished();
private:
    void setupUi();
    bool eventFilter(QObject *obj, QEvent *event);
    void setLabelPos();
    void drawPoints();
    void drawLines(QPainter *painter);
    void drawArmMovingNodes();
    void drawZigbeeMovingNodes();

    void paintEvent(QPaintEvent *event);

    QVector<QLabel*> armLabels;
    QVector<QLabel*> zigbeeLabels;
    enum {
        ARM_NUM = 5,
        ZIGBEE_NUM = 25
    };
    int armNeighborCount;
    int zigbeeNeighborCount;
    int armPathCount;
    int zigbeePathCount;
    int armPathIndex;
    int zigbeePathIndex;
    QPoint armPos[ARM_NUM];
    QPoint zigbeePos[ZIGBEE_NUM];
    int armMatrix[ARM_NUM][ARM_NUM];
    int zigbeeMatrix[ZIGBEE_NUM*2][ZIGBEE_NUM*2];
    int armPath[ARM_NUM][ARM_NUM];
    int zigbeePath[ZIGBEE_NUM][ZIGBEE_NUM];
    QPixmap *bgMap;
    QLabel *armMovingNodes[ARM_NUM];
    QLabel *zigbeeMovingNodes[ZIGBEE_NUM];
    QPropertyAnimation *armAnimations[ARM_NUM];
    QPropertyAnimation *zigbeeAnimations[ZIGBEE_NUM];

public:
    explicit MapLabel(QWidget *parent = 0);
    void setArmMatrix(int i, int j);
    void setZigbeeMatrix(int i, int j);
    void addArmNeighborCount();
    void addZigbeeNeighborCount();
    void setArmPath(int src, int n, int *dst);
    void setZigbeePath(int src, int n, int *dst);
    void addArmPathCount();
    void addZigbeePathCount();
    void drawMovingNodes();

};

#endif // MAPLABEL_H
