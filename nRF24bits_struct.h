#ifndef NRF24BITS_STRUCT_H_INCLUDED
#define NRF24BITS_STRUCT_H_INCLUDED

#include <stdint.h>

#define RX_MODE 1
#define TX_MODE 0

typedef uint16_t word;
typedef uint8_t byte;
namespace nrf24sim
{
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

typedef struct R_CONFIG
{
    uint8_t sPRIM_RX:1;      //0
    uint8_t sPWR_UP:1;       //1
    uint8_t sCRC0:1;         //2
    uint8_t sEN_CRC:1;       //3
    uint8_t sMASK_MAX_RT:1;  //4
    uint8_t sMASK_TX_DS:1;   //5
    uint8_t sMASK_RX_DR:1;   //6
    uint8_t sReserved:1;     //7
} tCONFIG;

typedef struct R_EN_AA
{
    uint8_t sENAA_P0:1;
    uint8_t sENAA_P1:1;
    uint8_t sENAA_P2:1;
    uint8_t sENAA_P3:1;
    uint8_t sENAA_P4:1;
    uint8_t sENAA_P5:1;
    uint8_t sReserved:2;
} tEN_AA;

typedef struct R_EN_RXADDR
{
    uint8_t sERX_P0:1;
    uint8_t sERX_P1:1;
    uint8_t sERX_P2:1;
    uint8_t sERX_P3:1;
    uint8_t sERX_P4:1;
    uint8_t sERX_P5:1;
    uint8_t sReserved:2;
} tEN_RXADDR;

typedef struct R_SETUP_AW
{
    uint8_t sAW:2;
    uint8_t sReserved:6;
}tSETUP_AW;

typedef struct R_SETUP_RETR
{
    uint8_t sARC:4;
    uint8_t sARD:4;
}tSETUP_RETR;

typedef struct R_RF_CH
{
    uint8_t sRF_CH:7;
    uint8_t sReserved:1;
}tRF_CH;

typedef struct R_RF_SETUP
{
    uint8_t sObsolete:1;
    uint8_t sRF_PWR:2;
    uint8_t sRF_DR_HIGH:1;
    uint8_t sPLL_LOCK:1;
    uint8_t sRF_DR_LOW:1;
    uint8_t sReserved:1;
    uint8_t sCONT_WAVE:1;
}tRF_SETUP;

typedef struct R_STATUS
{
    uint8_t sTX_FULL:1;
    uint8_t sRX_P_NO:3;
    uint8_t sMAX_RT:1;
    uint8_t sTX_DS:1;
    uint8_t sRX_DR:1;
    uint8_t sReserved:1;
}tSTATUS;

typedef struct R_OBSERVE_TX
{
    uint8_t sARC_CNT:4;//0123
    uint8_t sPLOS_CNT:4;//4567
}tOBSERVE_TX;

typedef struct R_RPD
{
    uint8_t sRPD:1;
    uint8_t sReserved:7;
}tRPD;

typedef struct R_RX_PW_P
{
    uint8_t sRX_PW_P:6;
    uint8_t sReserved:2;
}tRX_PW_P;


typedef struct R_FIFO_STATUS
{
    uint8_t sRX_EMPTY:1;
    uint8_t sRX_FULL:1;
    uint8_t sReserved1:2;
    uint8_t sTX_EMPTY:1;
    uint8_t sTX_FULL:1;
    uint8_t sTX_REUSE:1;
    uint8_t sReserved2:1;
}tFIFO_STATUS;

typedef struct R_DYNPD
{
    uint8_t sDPL_P0:1;
    uint8_t sDPL_P1:1;
    uint8_t sDPL_P2:1;
    uint8_t sDPL_P3:1;
    uint8_t sDPL_P4:1;
    uint8_t sDPL_P5:1;
    uint8_t sReserved:2;
}tDYNPD;

typedef struct R_FEATURE
{
    uint8_t sEN_DYN_ACK:1;
    uint8_t sEN_ACK_PAY:1;
    uint8_t sEN_DPL:1;
    uint8_t sReserved:5;
}tFEATURE;

typedef struct sREGISTERS
{
    tCONFIG         sCONFIG;
    tEN_AA          sEN_AA;
    tEN_RXADDR      sEN_RXADDR;
    tSETUP_AW       sSETUP_AW;
    tSETUP_RETR     sSETUP_RETR;
    tRF_CH          sRF_CH;
    tRF_SETUP       sRF_SETUP;
    tSTATUS         sSTATUS;
    tOBSERVE_TX     sOBSERVE_TX;
    tRPD            sRPD;
    tRX_PW_P        sRX_PW_P0;
    tRX_PW_P        sRX_PW_P1;
    tRX_PW_P        sRX_PW_P2;
    tRX_PW_P        sRX_PW_P3;
    tRX_PW_P        sRX_PW_P4;
    tRX_PW_P        sRX_PW_P5;
    tFIFO_STATUS    sFIFO_STATUS;
    tDYNPD          sDYNPD;
    tFEATURE        sFEATURE;
}tREGISTERS;

typedef struct sPACKET_CONTROL_FIELD
{
    uint8_t Payload_length:6;
    uint8_t PID:2;
    uint8_t NP_ACK:1;
}tPACKET_CONTROL_FIELD;

typedef struct sFRAME
{
    uint64_t Address;
    tPACKET_CONTROL_FIELD Packet_Control_Field;
    uint8_t Payload[32];
}tMsgFrame;
}
#endif // NRF24BITS_STRUCT_H_INCLUDED
