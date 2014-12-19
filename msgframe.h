#ifndef MSGFRAME_H
#define MSGFRAME_H
#include <stdint.h>
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


#endif // MSGFRAME_H
