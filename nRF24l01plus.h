#ifndef NRF24L01PLUS_H
#define NRF24L01PLUS_H

#include <stdint.h>
#include "nRF24bits_struct.h"
#include "nRF24interface.h"
#include <queue>
#include <QTimer>

class nRF24l01plus : public nRF24interface
{
    public:
        nRF24l01plus(QObject * parent = 0);
        virtual ~nRF24l01plus();
    protected:
    private:
        void send_frame(tMsgFrame * theFrame);
        //Eather interface placeholders
        void send_to_eathar(tMsgFrame * theFrame);
        void startPTX();
        tMsgFrame * TXpacket;
        QTimer * theTimer;
    private slots:
        void CEsetHIGH();
        void TXmodeSet();
        void TXpacketAdded();
        void ackReceved();
        void noACKalarm();
};

#endif // NRF24L01PLUS_H
