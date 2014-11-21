#ifndef NRF24BITS_STRUCT_H_INCLUDED
#define NRF24BITS_STRUCT_H_INCLUDED

#include <stdint.h>

typedef struct R_CONFIG
{
    uint8_t PRIM_RX:1;      //0
    uint8_t PWR_UP:1;       //1
    uint8_t CRC0:1;         //2
    uint8_t EN_CRC:1;       //3
    uint8_t MASK_MAX_RT:1;  //4
    uint8_t MASK_TX_DS:1;   //5
    uint8_t MASK_RX_DR:1;   //6
    uint8_t Reserved:1;     //7
} tCONFIG;

typedef struct R_EN_AA
{
    uint8_t ENAA_P0:1;
    uint8_t ENAA_P1:1;
    uint8_t ENAA_P2:1;
    uint8_t ENAA_P3:1;
    uint8_t ENAA_P4:1;
    uint8_t ENAA_P5:1;
    uint8_t Reserved:2;
} tEN_AA;

typedef struct R_EN_RXADDR
{
    uint8_t ERX_P0:1;
    uint8_t ERX_P1:1;
    uint8_t ERX_P2:1;
    uint8_t ERX_P3:1;
    uint8_t ERX_P4:1;
    uint8_t ERX_P5:1;
    uint8_t Reserved:2;
} tEN_RXADDR;

typedef struct R_SETUP_AW
{
    uint8_t AW:2;
    uint8_t Reserved:6;
}tSETUP_AW;

typedef struct R_SETUP_RETR
{
    uint8_t ARC:4;
    uint8_t ARD:4;
}tSETUP_RETR;

typedef struct R_RF_CH
{
    uint8_t RF_CH:7;
    uint8_t Reserved:1;
}tRF_CH;

typedef struct R_RF_SETUP
{
    uint8_t Obsolete:1;
    uint8_t RF_PWR:2;
    uint8_t RF_DR_HIGH:1;
    uint8_t PLL_LOCK:1;
    uint8_t RF_DR_LOW:1;
    uint8_t Reserved:1;
    uint8_t CONT_WAVE:1;
}tRF_SETUP;

typedef struct R_STATUS
{
    uint8_t TX_FULL:1;
    uint8_t RX_P_NO:3;
    uint8_t MAX_RT:1;
    uint8_t TX_DS:1;
    uint8_t RX_DR:1;
    uint8_t Reserved:1;
}tSTATUS;

typedef struct R_OBSERVE_TX
{
    uint8_t ARC_CNT:4;//0123
    uint8_t PLOS_CNT:4;//4567
}tOBSERVE_TX;

typedef struct R_RPD
{
    uint8_t RPD:1;
    uint8_t Reserved:7;
}tRPD;

typedef struct R_RX_PW_P
{
    uint8_t RX_PW_P:6;
    uint8_t Reserved:2;
}tRX_PW_P;


typedef struct R_FIFO_STATUS
{
    uint8_t RX_EMPTY:1;
    uint8_t RX_FULL:1;
    uint8_t Reserved1:2;
    uint8_t TX_EMPTY:1;
    uint8_t TX_FULL:1;
    uint8_t TX_REUSE:1;
    uint8_t Reserved2:1;
}tFIFO_STATUS;

typedef struct R_DYNPD
{
    uint8_t DPL_P0:1;
    uint8_t DPL_P1:1;
    uint8_t DPL_P2:1;
    uint8_t DPL_P3:1;
    uint8_t DPL_P4:1;
    uint8_t DPL_P5:1;
    uint8_t Reserved:2;
}tDYNPD;

typedef struct R_FEATURE
{
    uint8_t EN_DYN_ACK:1;
    uint8_t EN_ACK_PAY:1;
    uint8_t EN_DPL:1;
    uint8_t Reserved:5;
}tFEATURE;

typedef struct sREGISTERS
{
    tCONFIG         CONFIG;
    tEN_AA          EN_AA;
    tEN_RXADDR      EN_RXADDR;
    tSETUP_AW       SETUP_AW;
    tSETUP_RETR     SETUP_RETR;
    tRF_CH          RF_CH;
    tRF_SETUP       RF_SETUP;
    tSTATUS         STATUS;
    tOBSERVE_TX     OBSERVE_TX;
    tRPD            RPD;
    tRX_PW_P        RX_PW_P0;
    tRX_PW_P        RX_PW_P1;
    tRX_PW_P        RX_PW_P2;
    tRX_PW_P        RX_PW_P3;
    tRX_PW_P        RX_PW_P4;
    tRX_PW_P        RX_PW_P5;
    tFIFO_STATUS    FIFO_STATUS;
    tDYNPD          DYNPD;
    tFEATURE        FEATURE;
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

#endif // NRF24BITS_STRUCT_H_INCLUDED
