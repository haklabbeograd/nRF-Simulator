#ifndef NRF24L01PLUS_H
#define NRF24L01PLUS_H

#include <stdint.h>
#include "nRF24bits_struct.h"
#include <queue>

typedef uint16_t word;
typedef uint8_t byte;

typedef enum {eNOP=0,eR_REGISTER, eW_REGISTER,eR_RX_PAYLOAD,eW_TX_PAYLOAD,eFLUSH_TX,eFLUSH_RX,eREUSE_TX_PL,eR_RX_PL_WID,eW_ACK_PAYLOAD,eW_TX_PAYLOAD_NO_ACK}commands;
typedef enum {
    eCONFIG      =0x00,  eEN_AA       =0x01,  eEN_RXADDR   =0x02,
    eSETUP_AW    =0x03,  eSETUP_RETR  =0x04,  eRF_CH       =0x05,
    eRF_SETUP    =0x06,  eSTATUS      =0x07,  eOBSERVE_TX  =0x08,
    eRPD         =0x09,  eRX_ADDR_P0  =0x0A,  eRX_ADDR_P1  =0x0B,
    eRX_ADDR_P2  =0x0C,  eRX_ADDR_P3  =0x0D,  eRX_ADDR_P4  =0x0E,
    eRX_ADDR_P5  =0x0F,  eTX_ADDR     =0x10,  eRX_PW_P0    =0x11,
    eRX_PW_P1    =0x12,  eRX_PW_P2    =0x13,  eRX_PW_P3    =0x14,
    eRX_PW_P4    =0x15,  eRX_PW_P5    =0x16,  eFIFO_STATUS =0x17,
    eDYNPD       =0x1C,  eFEATURE     =0x1D,   }registers;

class nRF24l01plus
{
    public:
        nRF24l01plus();
        virtual ~nRF24l01plus();
    protected:
    private:
        //inteface functions
        void init_registers(void);//alocates memmory for registers
        void clear_registers(void);//frees memmory for registers
        byte * read_register(byte * read_command);
        void write_register(byte * bytes_to_write);
        tMsgFrame * read_RX_payload();
        void write_TX_payload(byte * bytes_to_write);
        void write_ack_payload(byte * bytes_to_write);
        void flush_tx();
        void flush_rx();
        byte addressToPype(uint64_t address);
        byte * reuse_tx_payload();
        uint8_t read_RX_payload_width();

        byte * nop();
        commands get_command(byte command);
        void send_frame(tMsgFrame * theFrame);

        //interface registers
        void * register_array[0x1E];
        tREGISTERS REGISTERS;
        std::queue<tMsgFrame*> RX_FIFO;
        std::queue<tMsgFrame*> TX_FIFO;
        bool CE;
        uint8_t PID;
        tMsgFrame * lastTransmited;
        tMsgFrame * assemble_ack_packet();

        //Eather interface placeholders
        void send_to_eathar(tMsgFrame * theFrame);
    public:
        void Spi_Write(byte * msg,byte * msgBack);
        void printRegContents();
        bool receve_frame(tMsgFrame * theFrame);
        bool checkIRQ();

        void setCE_HIGH(){CE = true;}
        void setCE_LOW(){CE = false;}
        bool getCE(){return CE;}
};

#endif // NRF24L01PLUS_H
