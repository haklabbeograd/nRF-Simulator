#include "nRF24l01plus.h"
#include "ether.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//constructor
nRF24l01plus::nRF24l01plus(QObject *parent,Ether * someEthar):nRF24interface(parent),coalision(false),theEther(someEthar)
{
    //ctor
    theTimer = new QTimer(this);
    connect(this,SIGNAL(CEsetHIGH()),this,SLOT(CEsetHIGH()));
    connect(this,SIGNAL(TXmodeSet()),this,SLOT(TXmodeSet()));
    connect(this,SIGNAL(TXpacketAdded()),this,SLOT(TXpacketAdded()));
    connect(this->theTimer,SIGNAL(timeout()),this,SLOT(noACKalarm()));

    connect(theEther,SIGNAL(dispachMsg(tMsgFrame*)),this,SLOT(reciveMsgFromET(tMsgFrame*)));
    connect(this,SIGNAL(sendMsgToET(tMsgFrame*)),theEther,SLOT(sendMSG(tMsgFrame*)));
    connect(theEther,SIGNAL(coalisionSig()),this,SLOT(setCoalision()));
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
    emit sendMsgToET((tMsgFrame*)packetToSend);
    //check if ack expected
    if((packetToSend->Packet_Control_Field.NP_ACK == 0) && (getARC()!=0))
    {
        //set for ACK recipt..
        ACK_address = getTXaddress();
        waitingForACK = true;
        clearARC_CNT();
        theTimer->start(getARD()*10);
        TXpacket = packetToSend;
    }
    else
    {//no ack last transmited is the one that is
        if(lastTransmited != NULL)
        {
            delete lastTransmited;
            lastTransmited = NULL;
        }
        lastTransmited = packetToSend;
    }
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
    waitingForACK = false;
    if(lastTransmited != NULL)
    {
        delete lastTransmited;
        lastTransmited = NULL;
    }
    lastTransmited = TXpacket;
}

void nRF24l01plus::noACKalarm()
{
    if(getARC_CNT() == getARC())
    {//number of max retransmits acchived
     //failed to reach targe set aproprite flags
        PLOS_CNT_INC();
        setMAX_RT_IRQ();
        if(lastTransmited != NULL)
            delete lastTransmited;
        lastTransmited = TXpacket;
        theTimer->stop();
        waitingForACK = false;
    }
    else
    {//retransmit message and increasse reTransCounter and start timer again
        emit sendMsgToET(TXpacket);
        //setup wait for ack...
        ARC_CNT_INC();
        theTimer->start(getARD() * 10);//10ms real life 250us
    }
}

void nRF24l01plus::reciveMsgFromET(tMsgFrame *theMSG)
{
    if(!coalision)
    {//do the shit

    }
    coalision = false;
}

void nRF24l01plus::setCoalision()
{
    coalision = true;
}




