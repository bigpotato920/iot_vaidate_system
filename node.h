#ifndef NODE_H
#define NODE_H

#include <QDateTime>

class Node
{
public:
    Node();
    Node(const Node &node);
    Node &operator =(const Node &node);

    int getType();
    int getIndex();
    int getCo2();
    double getTemperature();
    double getHumidity();
    double getIllumination();
    QDateTime getCreateTime();

    void setType(int type);
    void setIndex(int index);
    void setCo2(int co2);
    void setTemperature(double temperature);
    void setIllumination(double illumination);
    void setHumidity(double humidity);

private:

    int type;
    int index;
    int co2;
    double temperature;
    double humidity;
    double illumination;
    QDateTime createTime;
};

#endif // NODE_H
