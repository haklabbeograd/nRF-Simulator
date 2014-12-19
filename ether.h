#ifndef ETHER_H
#define ETHER_H

#include <QThread>
#include <QTimer>
#include <QMutex>

#include "msgframe.h"

class Ether : public QThread
{
    Q_OBJECT
public:
    explicit Ether(QObject *parent = 0);
signals:
    void dispachMsg(tMsgFrame * theMSG);
    void coalisionSig();
public slots:
    void sendMSG(tMsgFrame * theMSG);
private:
    QMutex mutex;
    QTimer * myTimer;
    tMsgFrame * MSG;
private slots:
    void alarm();

};

#endif // ETHER_H
