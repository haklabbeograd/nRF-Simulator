#ifndef NRF24REGISTERS_H
#define NRF24REGISTERS_H

#include <stdint.h>
#include "nRF24bits_struct.h"


class nRF24registers
{
    public:
        /** Default constructor */
        nRF24registers();
        /** Default destructor */
        ~nRF24registers();
        void printRegContents();
        bool checkIRQ();
        void setCE_HIGH(){CE = true;}
        void setCE_LOW(){CE = false;}
        bool getCE(){return CE;}
    protected:
        byte * read_register(byte * read_command);
        void write_register(byte * bytes_to_write);
        byte addressToPype(uint64_t address);
        uint64_t getAddressFromPipe(byte pipe);
        uint64_t getAddressFromPipe_ENAA(byte pipe);
        uint64_t getTXaddress(){return *( (uint64_t*)register_array[eTX_ADDR] );}
        bool isDynamicACKEnabled(){return REGISTERS.FEATURE.EN_DYN_ACK;}
        bool isDynamicPayloadEnabled(){return REGISTERS.FEATURE.EN_DPL;}
        bool isFIFO_TX_EMPTY(){return REGISTERS.FIFO_STATUS.TX_EMPTY;}
        bool isFIFO_TX_FULL(){return REGISTERS.FIFO_STATUS.TX_FULL;}
        bool isFIFO_RX_EMTPY(){return REGISTERS.FIFO_STATUS.RX_EMPTY;}
        bool isFIFO_RX_FULL(){return REGISTERS.FIFO_STATUS.RX_FULL;}
        bool isRX_MODE(){return REGISTERS.CONFIG.PRIM_RX;}
        void clearRX_FULL(){REGISTERS.FIFO_STATUS.RX_FULL = 0;}
        void setRX_FULL(){REGISTERS.FIFO_STATUS.RX_FULL = 1;}
        void clearRX_EMPTY(){REGISTERS.FIFO_STATUS.RX_EMPTY = 0;}
        void setRX_EMPTY(){REGISTERS.FIFO_STATUS.RX_EMPTY = 1;}
        void setTX_FULL(){REGISTERS.FIFO_STATUS.TX_FULL = 1;}
        void clearTX_FULL(){REGISTERS.FIFO_STATUS.TX_FULL = 0;}
        void setTX_FULL_IRQ(){REGISTERS.STATUS.TX_FULL = 1;}
        void clearTX_FULL_IRQ(){REGISTERS.STATUS.TX_FULL = 0;}
        void setTX_EMPTY(){REGISTERS.FIFO_STATUS.TX_EMPTY = 1;}
        void clearTX_EMPTY(){REGISTERS.FIFO_STATUS.TX_EMPTY = 0;}
        void setRX_DR_IRQ(){REGISTERS.STATUS.RX_DR = 1;}
        void clearRX_DR_IRQ(){REGISTERS.STATUS.RX_DR = 0;}
        void setRX_P_NO(byte pipe){REGISTERS.STATUS.RX_P_NO = pipe;}
    private:
        tREGISTERS REGISTERS;
        void * register_array[0x1E];
        bool CE;
};

#endif // NRF24REGISTERS_H
