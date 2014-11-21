#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nRF24l01plus.h"


void printBin(byte toPrint)
{
    int i =8;
    printf("0b");
    while(--i>=0)
    {
        printf("%d",(toPrint & 0b10000000)>>7 );
        if(i==4)printf(" ");
        toPrint<<=1;
    }
}
int main()
{
    nRF24l01plus test;

                      //76543210   76543210
    byte setPRX[6] = {0b00100000,0b00001011,0,0,0,0};
    byte msgReturn[33];

    test.Spi_Write(setPRX,msgReturn);
    test.setCE_HIGH();

    tMsgFrame * newFrame = new tMsgFrame;
    newFrame->Address = 0xe7e7e7e7e7;
    newFrame->Packet_Control_Field.NP_ACK = 0;
    newFrame->Packet_Control_Field.Payload_length=5;
    newFrame->Packet_Control_Field.PID = 1;
    newFrame->Payload[0] = 1;
    newFrame->Payload[1] = 2;
    newFrame->Payload[2] = 4;
    newFrame->Payload[3] = 8;
    newFrame->Payload[4] = 16;

    test.receve_frame(newFrame);
    newFrame->Payload[2] = 11;
    test.receve_frame(newFrame);
    newFrame->Packet_Control_Field.Payload_length=2;
    test.receve_frame(newFrame);


    byte readRxPayloadWid[6] = {0b01100000,0,0,0,0,0};
    test.Spi_Write(readRxPayloadWid,msgReturn);

    byte msgLen = msgReturn[1];

    byte readRxPayload[6] = {0b01100001,0,0,0,0,0};
    test.Spi_Write(readRxPayload,msgReturn);

    test.Spi_Write(readRxPayloadWid,msgReturn);
    msgLen = msgReturn[1];
    test.Spi_Write(readRxPayload,msgReturn);

    test.Spi_Write(readRxPayloadWid,msgReturn);
    msgLen = msgReturn[1];
    test.Spi_Write(readRxPayload,msgReturn);

    for(int i = 0; i < msgLen; i ++)
    {
        printf("\n%d",msgReturn[i+1]);
    }

    test.printRegContents();


    return 0;
}
