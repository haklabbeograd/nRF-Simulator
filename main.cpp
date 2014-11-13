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
    byte returnMsg[6];

    byte comand[6];
              //76543210
    comand[0]=0b00000111;
    test.Spi_Write(comand,returnMsg);

                //76543210
    comand[0] = 0b00100111;
    comand[1] = 0b10101010;
    comand[2] = 0b10101010;
    comand[3] = 0b10101010;
    comand[4] = 0b10101010;
    comand[5] = 0b10101010;
    test.Spi_Write(comand,returnMsg);
                //76543210
    comand[0] = 0b00000111;
    test.Spi_Write(comand,returnMsg);

    return 0;
}
