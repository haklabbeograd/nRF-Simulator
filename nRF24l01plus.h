#ifndef NRF24L01PLUS_H
#define NRF24L01PLUS_H

#include <stdint.h>
#include "nRF24bits_struct.h"
#include "nRF24interface.h"
#include <queue>

class nRF24l01plus : public nRF24interface
{
    public:
        nRF24l01plus();
        virtual ~nRF24l01plus();
    protected:
    private:
        void send_frame(tMsgFrame * theFrame);
        //Eather interface placeholders
        void send_to_eathar(tMsgFrame * theFrame);
    public:
};

#endif // NRF24L01PLUS_H
