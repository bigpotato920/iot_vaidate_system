#include "node.h"


Node::Node()
{
    this->type = 0;
    this->index = 0;
    this->co2 = 0;
    this->temperature = 0.0;
    this->humidity = 0.0;
    this->illumination = 0.0;

}

Node::Node(const Node &node)
{
    this->type = node.type;
    this->index = node.index;
    this->co2 = node.co2;
    this->temperature = node.temperature;
    this->humidity = node.humidity;
    this->illumination = node.illumination;
    this->createTime = node.createTime;
}

Node & Node::operator = (const Node &node) {
    this->type = node.type;
    this->index = node.index;
    this->co2 = node.co2;
    this->temperature = node.temperature;
    this->humidity = node.humidity;
    this->illumination = node.illumination;
    this->createTime = node.createTime;
    return *this;
}

int Node::getType()
{
    return this->type;
}

int Node::getIndex()
{
    return this->index;
}

int Node::getCo2()
{
    return this->co2;
}

double Node::getTemperature()
{
    return this->temperature;
}

double Node::getHumidity()
{
    return this->humidity;
}

double Node::getIllumination()
{
    return this->illumination;
}

QDateTime Node::getCreateTime()
{
    return this->createTime;
}

void Node::setType(int type)
{
    this->type = type;
}

void Node::setIndex(int index)
{
    this->index = index;
}

void Node::setCo2(int co2)
{
    this->co2 = co2;
}

void Node::setTemperature(double temperature)
{
    this->temperature = temperature;
}


void Node::setIllumination(double illumination)
{
    this->illumination = illumination;
}

void Node::setHumidity(double humidity)
{
    this->humidity = humidity;
}
