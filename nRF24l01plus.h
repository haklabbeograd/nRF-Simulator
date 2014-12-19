#ifndef NRF24L01PLUS_H
#define NRF24L01PLUS_H

#include <stdint.h>
#include "nRF24interface.h"

class Ether;

#include <queue>
#include <QTimer>

class nRF24l01plus : public nRF24interface
{
    Q_OBJECT
    public:
        nRF24l01plus(QObject * parent = 0,Ether * someEthar = NULL);
        virtual ~nRF24l01plus();
    protected:
    private:
        void send_frame(tMsgFrame * theFrame);
        //Eather interface placeholders
        void send_to_eathar(tMsgFrame * theFrame);
        void startPTX();
        tMsgFrame * TXpacket;
        QTimer * theTimer;
        uint64_t ACK_address;
        bool waitingForACK;
        bool coalision;
        Ether * theEther;
        void ackReceved(tMsgFrame * theMSG, byte pipe);
    private slots:
        void CEsetHIGH();
        void TXmodeSet();
        void TXpacketAdded();
        void PWRUPset(void);
        void noACKalarm();
        void reciveMsgFromET(tMsgFrame * theMSG);
        void setCoalision();
    signals:
        void sendMsgToET(tMsgFrame* theMSG);
};

#endif // NRF24L01PLUS_H
