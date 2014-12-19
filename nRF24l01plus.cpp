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
    connect(this,SIGNAL(PWRUPset()),this,SLOT(PWRUPset()));
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
    if(getCE() == false || isPWRUP() == false)return;
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
        setTX_DS_IRQ();
    }
}

/*
 * in PTX if CE is set HIGH and there is a packet waiting transmit packet
 *
 */
void nRF24l01plus::CEsetHIGH()
{
    //if nRF is in RX mode nothing shoud happen;
    if(isRX_MODE() == true || isPWRUP() == false)return;
    startPTX();
}

/*
 * If TX mode is set while CE is high and there is a packet waiting transmit packet
 *
 */
void nRF24l01plus::TXmodeSet()
{
    if(getCE() == false || isPWRUP() == false) return;
    startPTX();

}

void nRF24l01plus::TXpacketAdded()
{
    if(getCE() == false || isRX_MODE() == true || isPWRUP() == false) return;
    startPTX();
}

void nRF24l01plus::PWRUPset()
{
    if(getCE() == false || isRX_MODE() == true)return;
    startPTX();
}

void nRF24l01plus::ackReceved(tMsgFrame *theMSG,byte pipe)
{
    if(isDynamicACKEnabled())
    {//coud be ACK with payload
        receve_frame(theMSG,pipe);
        waitingForACK = false;
    }
    else
    {//could be regular ack
        if(theMSG->Packet_Control_Field.Payload_length == 0)
        {//regular ACK receved :D
            waitingForACK = false;
        }
    }

    if(waitingForACK == false)
    {
        theTimer->stop();

        if(lastTransmited != NULL)
        {
            delete lastTransmited;
            lastTransmited = NULL;
        }
        lastTransmited = TXpacket;
        setTX_DS_IRQ();
    }
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
    {//coalision did not happen
        if(isPWRUP() && getCE())
        {//in Standby mode (PWRUP = 1 && CE = 1)
            byte pipe = addressToPype(theMSG->Address);
            if(isRX_MODE())
            {//receving
             //check if address is one off th
                if(pipe != 0xFF)
                {//pipe is open ready to receve
                    //fill RX buffer
                    receve_frame(theMSG,pipe);
                }
            }
            else if(waitingForACK)
            {//waiting for ack
                if(pipe == addressToPype(getTXaddress()))
                {//addess is the P0 address, this is ACK packet
                    ackReceved(theMSG,pipe);
                }
            }
        }
    }
    coalision = false;
}

void nRF24l01plus::setCoalision()
{
    coalision = true;
}




