#ifndef REQUEST_H
#define REQUEST_H

class Request
{
public:
    Request(int reqType, int nodeType, int index);
    Request();
    Request(const Request &req);
    Request& operator = (const Request &req);
    int getReqType();
    int getNodeType();
    int getIndex();

    enum {
        NEIGHBOR_REQ = 0,
        DATA_REQ = 1,
        PATH_REQ = 2
    };

    enum {
        ARM_NODE = 0,
        ZIGBEE_NODE = 1,
        PC_NODE = 2
    };

private:


    int reqType;
    int nodeType;
    int index;
};

#endif // REQUEST_H
