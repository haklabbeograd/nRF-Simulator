#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nRF24l01plus.h"
#include "RF24.h"

/*
Comentting back storrry

ether should be a list of pointers to receve_frame functions...
which should copy data over to themselves and return either fail or otherwise..

each brfClass object should have send_frame function which attempts to send a frame onto ehter,
but ether tells it if it's busy or not...

if(busy) fail;
else
{
    if(setBusy() == FALSE) fail;
    else
    {//bussy has been set, you own ethers ass now.
     coppy over the Frame
     exit and enjoy the show :D:D:D
    }
}

if ether is in one thread...

QT comments
qthread x;
x->some_public_func()
iako definisana u qthread klasi izvrsava se kod u main (pozivajucem thread-u)
emit signal()
x->slot()
se izvrsava u qthreadu.
*/

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
    nRF24l01plus * test = new nRF24l01plus();

    RF24 * radio = new RF24(9,10,test);
    radio->begin();

    radio->setRetries(15,15);


    radio->setPayloadSize(8);

    radio->openWritingPipe(0xF0F0F0F0E1);


    radio->openReadingPipe(1, 0xF0F0F0F0D2);

    system("cls");
    getchar();
    radio->startListening();
    getchar();
    radio->printDetails();
    getchar();
    radio->stopListening();
    getchar();
    radio->printDetails();
    getchar();
    radio->available();

    return 0;
}


