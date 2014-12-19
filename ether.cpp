#include <QDebug>
#include "ether.h"

Ether::Ether(QObject *parent) :
    QThread(parent)
{
    myTimer = new QTimer(this);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(alarm()));
}

void Ether::sendMSG(tMsgFrame *theMSG)
{
    if(mutex.tryLock(0))
    {
        MSG = theMSG;
        myTimer->start(60);
    }
    else
    {
        emit coalisionSig();
    }
}

void Ether::alarm()
{
    mutex.unlock();
    myTimer->stop();
    emit dispachMsg(MSG);
    MSG = NULL;
}
