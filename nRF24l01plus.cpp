#include "nRF24l01plus.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//constructor
nRF24l01plus::nRF24l01plus(QObject *parent):nRF24interface(parent)
{
    //ctor
    theTimer = new QTimer(this);
    connect(this,SIGNAL(CEsetHIGH()),this,SLOT(CEsetHIGH()));
    connect(this,SIGNAL(TXmodeSet()),this,SLOT(TXmodeSet()));
    connect(this,SIGNAL(TXpacketAdded()),this,SLOT(TXpacketAdded()));
    connect(this->theTimer,SIGNAL(timeout()),this,SLOT(noACKalarm()));
}

nRF24l01plus::~nRF24l01plus()
{
    //dtor
}

/*
 * Start transmision...
 *
 */
void nRF24l01plus::startPTX()
{   //
    if(getCE() == false)return;
    if(isRX_MODE())return;
    tMsgFrame * packetToSend = getTXpacket();
    if(packetToSend == NULL)return;
    //packet found in TX that is not ACK packet
    //load with TX address
    packetToSend->Address = getTXaddress();
    clearARC_CNT();
}

/*
 * in PTX if CE is set HIGH and there is a packet waiting transmit packet
 *
 */
void nRF24l01plus::CEsetHIGH()
{
    //if nRF is in RX mode nothing shoud happen;
    if(isRX_MODE())return;
    startPTX();
}

/*
 * If TX mode is set while CE is high and there is a packet waiting transmit packet
 *
 */
void nRF24l01plus::TXmodeSet()
{
    if(getCE() == false) return;
    startPTX();

}

void nRF24l01plus::TXpacketAdded()
{
    if(getCE() == false) return;
    if(isRX_MODE())return;
    startPTX();
}

void nRF24l01plus::ackReceved()
{
    theTimer->stop();
}

void nRF24l01plus::noACKalarm()
{
    if(getARC_CNT() == getARC())
    {//number of max retransmits acchived
     //failed to reach targe set aproprite flags
        PLOS_CNT_INC();
        setMAX_RT_IRQ();
        theTimer->stop();
    }
    else
    {//retransmit message and increasse reTransCounter and start timer again
        //send_to_ethar!!!
        //setup wait for ack...
        ARC_CNT_INC();
        theTimer->start(getARD() * 10);//10ms real life 250us
    }
}




