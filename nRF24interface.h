#ifndef NRF24INTERFACE_H
#define NRF24INTERFACE_H

#include <stdlib.h>
#include "nRF24registers.h"
#include "nRF24bits_struct.h"
#include <queue>

class nRF24interface : public nRF24registers
{
    public:
        /** Default constructor */
        nRF24interface(QObject * parent = 0);
        /** Default destructor */
        ~nRF24interface();
        byte Spi_Write(byte * msg, int msgLen, byte * msgBack);
        //move to protected
        bool receve_frame(tMsgFrame * theFrame);
    protected:
        //inteface functions
        tMsgFrame * read_RX_payload();
        tMsgFrame * getTXpacket();
        void write_TX_payload(byte * bytes_to_write, int len);
        void write_ack_payload(byte * bytes_to_write, int len);
        void write_no_ack_payload(byte * bytes_to_write, int len);
        tMsgFrame * get_ack_packet_for_pipe(uint8_t pipe);
        void flush_tx();
        void flush_rx();
        void reuse_last_transmited_payload();
        byte * reuse_tx_payload();
        uint8_t read_RX_payload_width();

        byte * nop();
        commands get_command(byte command);
        bool sREUSE_TX_PL;
        tMsgFrame * lastTransmited;
    private:
        //interface registers
        void newFrame(uint64_t Address, uint8_t PayLength, uint8_t thePID, uint8_t theNP_ACK,uint8_t * Payload);
        std::queue<tMsgFrame*> RX_FIFO;
        std::queue<tMsgFrame*> TX_FIFO;
        uint8_t PID;
signals:
        void TXpacketAdded(void);
};

#endif // NRF24INTERFACE_H
