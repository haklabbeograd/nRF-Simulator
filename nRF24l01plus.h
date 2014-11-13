#ifndef NRF24L01PLUS_H
#define NRF24L01PLUS_H

#include <stdint.h>
#include "nRF24registers.h"
#include "nRF24bits_struct.h"

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
        byte * write_register(byte * bytes_to_write);
        byte * read_RX_payload();
        byte * write_TX_payload(byte * bytes_to_write);
        byte * flush_tx();
        byte * flush_rx();
        byte * reuse_tx_payload();
        byte * read_width_of_head_payload();
        byte * write_ack_payload(byte * bytes_to_write);
        byte * write_tx_payload_no_ack(byte * bytes_to_write);
        byte * nop();
        commands get_command(byte command);
        //interface registers
        void * register_array[0x1E];
        tREGISTERS REGISTERS;
    public:
        void Spi_Write(byte * msg,byte * msgBack);
        void printRegContents();
};

#endif // NRF24L01PLUS_H
