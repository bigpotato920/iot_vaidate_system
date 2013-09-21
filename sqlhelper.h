#ifndef SQLHELPER_H
#define SQLHELPER_H
#include <QObject>

class SQLHelper: public QObject
{
private:
    SQLHelper();

public:

    static bool createConnection();
};

#endif // SQLHELPER_H
