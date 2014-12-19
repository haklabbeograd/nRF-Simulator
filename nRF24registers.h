#ifndef NRF24REGISTERS_H
#define NRF24REGISTERS_H

#include <stdint.h>
#include "nRF24bits_struct.h"
#include <QObject>
#include <QThread>

class nRF24registers:public QThread
{
    Q_OBJECT
    public:
        /** Default constructor */
        nRF24registers(QObject * parent = 0);
        /** Default destructor */
        ~nRF24registers();
        void printRegContents();
        bool checkIRQ();
        void setCE_HIGH();
        void setCE_LOW(){CE = false;}
        bool getCE(){return CE;}
    protected:
        byte * read_register(byte * read_command);
        void write_register(byte * bytes_to_write);
        byte addressToPype(uint64_t address);
        uint64_t getAddressFromPipe(byte pipe);
        uint64_t getAddressFromPipe_ENAA(byte pipe);
        uint8_t getARD(){return REGISTERS.sSETUP_RETR.sARD;}
        uint8_t getARC(){return REGISTERS.sSETUP_RETR.sARC;}
        uint64_t getTXaddress(){return *( (uint64_t*)register_array[eTX_ADDR] );}
        bool isDynamicACKEnabled(){return REGISTERS.sFEATURE.sEN_DYN_ACK;}
        bool isDynamicPayloadEnabled(){return REGISTERS.sFEATURE.sEN_DPL;}
        bool isFIFO_TX_EMPTY(){return REGISTERS.sFIFO_STATUS.sTX_EMPTY;}
        bool isFIFO_TX_FULL(){return REGISTERS.sFIFO_STATUS.sTX_FULL;}
        bool isFIFO_RX_EMTPY(){return REGISTERS.sFIFO_STATUS.sRX_EMPTY;}
        bool isFIFO_RX_FULL(){return REGISTERS.sFIFO_STATUS.sRX_FULL;}
        bool isRX_MODE(){return REGISTERS.sCONFIG.sPRIM_RX;}
        bool isPWRUP(){return REGISTERS.sCONFIG.sPWR_UP;}
        void clearRX_FULL(){REGISTERS.sFIFO_STATUS.sRX_FULL = 0;}
        void setRX_FULL(){REGISTERS.sFIFO_STATUS.sRX_FULL = 1;}
        void clearRX_EMPTY(){REGISTERS.sFIFO_STATUS.sRX_EMPTY = 0;}
        void setRX_EMPTY(){REGISTERS.sFIFO_STATUS.sRX_EMPTY = 1;}
        void setTX_FULL(){REGISTERS.sFIFO_STATUS.sTX_FULL = 1;}
        void clearTX_FULL(){REGISTERS.sFIFO_STATUS.sTX_FULL = 0;}
        void setTX_FULL_IRQ(){REGISTERS.sSTATUS.sTX_FULL = 1;}
        void clearTX_FULL_IRQ(){REGISTERS.sSTATUS.sTX_FULL = 0;}
        void setTX_EMPTY(){REGISTERS.sFIFO_STATUS.sTX_EMPTY = 1;}
        void clearTX_EMPTY(){REGISTERS.sFIFO_STATUS.sTX_EMPTY = 0;}
        void setRX_DR_IRQ(){REGISTERS.sSTATUS.sRX_DR = 1;}
        void clearRX_DR_IRQ(){REGISTERS.sSTATUS.sRX_DR = 0;}
        void setMAX_RT_IRQ(){REGISTERS.sSTATUS.sMAX_RT =1;}
        void setTX_DS_IRQ(){REGISTERS.sSTATUS.sTX_DS = 1;}
        void PLOS_CNT_INC();
        void clearPLOS_CNT(){REGISTERS.sOBSERVE_TX.sPLOS_CNT = 0;}
        void ARC_CNT_INC();
        void clearARC_CNT(){REGISTERS.sOBSERVE_TX.sARC_CNT = 0;}
        uint8_t getARC_CNT(){return REGISTERS.sOBSERVE_TX.sARC_CNT;}
        void setRX_P_NO(byte pipe){REGISTERS.sSTATUS.sRX_P_NO = pipe;}        
    private:
        tREGISTERS REGISTERS;
        void * register_array[0x1E];
        bool CE;
    signals:
        void CEsetHIGH(void);
        void TXmodeSet(void);
        void PWRUPset(void);
};

#endif // NRF24REGISTERS_H
