#include "request.h"

Request::Request(int reqType, int nodeType, int index)
{
    this->reqType = reqType;
    this->nodeType = nodeType;
    this->index = index;
}

Request::Request()
{
}

Request::Request(const Request &req)
{
    this->reqType = req.reqType;
    this->nodeType = req.nodeType;
    this->index = req.index;
}

Request &Request::operator =(const Request &req)
{
    this->reqType = req.reqType;
    this->nodeType = req.nodeType;
    this->index = req.index;

    return *this;
}

int Request::getReqType()
{
    return this->reqType;
}

int Request::getNodeType()
{
    return this->nodeType;
}

int Request::getIndex()
{
    return this->index;
}
