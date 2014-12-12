#include "nRF24registers.h"
#include <stdlib.h>
#include <stdio.h>

nRF24registers::nRF24registers():CE(false)
{
    register_array[eCONFIG]      = &REGISTERS.CONFIG;
    *((byte*)register_array[eCONFIG]) = 0b00001000;

    register_array[eEN_AA]       = &REGISTERS.EN_AA;
    *((byte*)register_array[eEN_AA]) = 0b00111111;

    register_array[eEN_RXADDR]   = &REGISTERS.EN_RXADDR;
    *((byte*)register_array[eEN_RXADDR]) = 0b00000011;

    register_array[eSETUP_AW]    = &REGISTERS.SETUP_AW;
    *((byte*)register_array[eSETUP_AW]) = 0b00000011;

    register_array[eSETUP_RETR]  = &REGISTERS.SETUP_RETR;
    *((byte*)register_array[eSETUP_RETR]) = 0b00000011;

    register_array[eRF_CH]       = &REGISTERS.RF_CH;
    *((byte*)register_array[eRF_CH]) = 0b00000010;

    register_array[eRF_SETUP]    = &REGISTERS.RF_SETUP;
    *((byte*)register_array[eRF_SETUP]) = 0b00001110;

    register_array[eSTATUS]      = &REGISTERS.STATUS;
    *((byte*)register_array[eSTATUS]) = 0b00001110;

    register_array[eOBSERVE_TX]  = &REGISTERS.OBSERVE_TX;
    *((byte*)register_array[eOBSERVE_TX]) = 0b00000000;

    register_array[eRPD]         = &REGISTERS.RPD;
    *((byte*)register_array[eRPD]) = 0b00000000;

    register_array[eRX_ADDR_P0]  = (uint64_t*)malloc(sizeof(uint64_t));
    *( (uint64_t*)register_array[eRX_ADDR_P0] ) = 0xE7E7E7E7E7;

    register_array[eRX_ADDR_P1]  = (uint64_t*)malloc(sizeof(uint64_t));
    *( (uint64_t*)register_array[eRX_ADDR_P1] ) = 0xC2C2C2C2C2;

    register_array[eRX_ADDR_P2]  = (byte*)malloc(sizeof(byte));
    *((byte*)register_array[eRX_ADDR_P2]) = 0xC3;

    register_array[eRX_ADDR_P3]  = (byte*)malloc(sizeof(byte));
    *((byte*)register_array[eRX_ADDR_P3]) = 0xC4;

    register_array[eRX_ADDR_P4]  = (byte*)malloc(sizeof(byte));
    *((byte*)register_array[eRX_ADDR_P4]) = 0xC5;

    register_array[eRX_ADDR_P5]  = (byte*)malloc(sizeof(byte));
    *((byte*)register_array[eRX_ADDR_P5]) = 0xC6;

    register_array[eTX_ADDR]  = (uint64_t*)malloc(sizeof(uint64_t));
    *( (uint64_t*)register_array[eTX_ADDR] ) = 0xE7E7E7E7E7;

    register_array[eRX_PW_P0]    = &REGISTERS.RX_PW_P0;
    *((byte*)register_array[eRX_PW_P0]) = 0b00000000;

    register_array[eRX_PW_P1]    = &REGISTERS.RX_PW_P1;
    *((byte*)register_array[eRX_PW_P1]) = 0b00000000;

    register_array[eRX_PW_P2]    = &REGISTERS.RX_PW_P2;
    *((byte*)register_array[eRX_PW_P2]) = 0b00000000;

    register_array[eRX_PW_P3]    = &REGISTERS.RX_PW_P3;
    *((byte*)register_array[eRX_PW_P3]) = 0b00000000;

    register_array[eRX_PW_P4]    = &REGISTERS.RX_PW_P4;
    *((byte*)register_array[eRX_PW_P4]) = 0b00000000;

    register_array[eRX_PW_P5]    = &REGISTERS.RX_PW_P5;
    *((byte*)register_array[eRX_PW_P5]) = 0b00000000;

    register_array[eFIFO_STATUS] = &REGISTERS.FIFO_STATUS;
    *((byte*)register_array[eFIFO_STATUS]) = 0b00010001;

    register_array[0x18]        = NULL;
    register_array[0x19]        = NULL;
    register_array[0x1A]        = NULL;
    register_array[0x1B]        = NULL;

    register_array[eDYNPD]       = &REGISTERS.DYNPD;
    *((byte*)register_array[eDYNPD]) = 0b00000000;

    register_array[eFEATURE]     = &REGISTERS.FEATURE;
    *((byte*)register_array[eFEATURE]) = 0b00000000;
}

nRF24registers::~nRF24registers()
{
    //dtor
    for(int i = eRX_ADDR_P0;i<=eTX_ADDR;i++)
    {
        free(register_array[i]);
    }
}

byte * nRF24registers::read_register(byte* read_command)
{
    byte addr = *read_command & 0b00011111;
    return (byte*)register_array[addr];
}

void nRF24registers::write_register(byte * bytes_to_write)
{
    byte temp;
    if(CE == false)
    {
        byte addr = bytes_to_write[0] & 0b00011111;
        //no write to these registers
        if( (addr == eOBSERVE_TX)||(addr == eRPD)||(addr == eFIFO_STATUS) )return;

        byte * where_to_write = (byte*)register_array[addr];

        if(addr!=eSTATUS)
            where_to_write[0] = bytes_to_write[1];
        else
        {//write to STATUS.. bits are cleared by writing 1 pm the,///
            temp = bytes_to_write[1] & 0b01110000; //clears bit that cant be written to
            temp = ~temp; //inverts all, 0 become 1 and  leaves those bits alone after AND op,  where 1s are bits will get cleared after AND operation
            where_to_write[0] &= temp;
        }
        printf("\nBYTE TO WRITE[0]: %X",bytes_to_write[1]);
        printf("\nBYTE WRITTEN[0]: %X",where_to_write[0]);
        if( (addr == eRX_ADDR_P0) ||  (addr == eRX_ADDR_P1) || (addr == eTX_ADDR))
        {
            where_to_write[1] = bytes_to_write[2];
            where_to_write[2] = bytes_to_write[3];
            where_to_write[3] = bytes_to_write[4];
            where_to_write[4] = bytes_to_write[5];
            printf("\nBYTE TO WRITE[1]: %X",where_to_write[1]);
            printf("\nBYTE TO WRITE[2]: %X",where_to_write[2]);
            printf("\nBYTE TO WRITE[3]: %X",where_to_write[3]);
            printf("\nBYTE TO WRITE[4]: %X",where_to_write[4]);
        }
    }
}

byte nRF24registers::addressToPype(uint64_t address)
{
    if(REGISTERS.EN_RXADDR.ERX_P0)
        if(address == *( (uint64_t *)register_array[eRX_ADDR_P0]) )
            return 0;

    if(REGISTERS.EN_RXADDR.ERX_P1)
        if(address == *( (uint64_t *)register_array[eRX_ADDR_P1]) )
            return 1;

    if(REGISTERS.EN_RXADDR.ERX_P2)
        if(address == *( (uint64_t *)register_array[eRX_ADDR_P2]) )
            return 2;

    if(REGISTERS.EN_RXADDR.ERX_P3)
        if(address == *( (uint64_t *)register_array[eRX_ADDR_P3]) )
            return 3;

    if(REGISTERS.EN_RXADDR.ERX_P4)
        if(address == *( (uint64_t *)register_array[eRX_ADDR_P4]) )
            return 4;

    if(REGISTERS.EN_RXADDR.ERX_P5)
        if(address == *( (uint64_t *)register_array[eRX_ADDR_P5]) )
            return 5;

    return -1;
}

uint64_t nRF24registers::getAddressFromPipe(byte pipe)
{
    uint64_t temp64;
    uint64_t ACK_address;
    switch(pipe)
    {
    case 0:
        if(REGISTERS.EN_RXADDR.ERX_P0) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P0] );
        else return 0;//PO RX not enabled
        break;
    case 1:
        if(REGISTERS.EN_RXADDR.ERX_P1) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return 0;//P1 RX not enabled
        break;
    case 2:
        if(REGISTERS.EN_RXADDR.ERX_P2) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return 0;//P2 RX not enabled
        temp64 = (uint64_t) ( *( (uint8_t*)(register_array[eRX_ADDR_P2]) ) );
        ACK_address &= 0xFFFFFF00;
        temp64 &= 0x000000FF;
        ACK_address |= temp64;
        break;
    case 3:
        if(REGISTERS.EN_RXADDR.ERX_P3) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return 0;//P3 RX not enabled
        temp64 = (uint64_t) ( *( (uint8_t*)(register_array[eRX_ADDR_P3]) ) );
        ACK_address &= 0xFFFFFF00;
        temp64 &= 0x000000FF;
        ACK_address |= temp64;
        break;
    case 4:
        if(REGISTERS.EN_RXADDR.ERX_P4) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return 0;//P4 RX not enabled
        temp64 = (uint64_t) ( *( (uint8_t*)(register_array[eRX_ADDR_P4]) ) );
        ACK_address &= 0xFFFFFF00;
        temp64 &= 0x000000FF;
        ACK_address |= temp64;
        break;
    case 5:
        if(REGISTERS.EN_RXADDR.ERX_P5) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return 0;//P5 RX not enabled
        temp64 = (uint64_t) ( *( (uint8_t*)(register_array[eRX_ADDR_P5]) ) );
        ACK_address &= 0xFFFFFF00;
        temp64 &= 0x000000FF;
        ACK_address |= temp64;
        break;
    default:
        return 0;
    }
    return ACK_address;
}

bool nRF24registers::checkIRQ()
{
    return ( (REGISTERS.STATUS.RX_DR && REGISTERS.CONFIG.MASK_RX_DR) || (REGISTERS.STATUS.TX_DS && REGISTERS.CONFIG.MASK_TX_DS) || (REGISTERS.STATUS.MAX_RT && REGISTERS.CONFIG.MASK_MAX_RT) );
}

void nRF24registers::printRegContents()
{
    printf("---REGISTERS---\n---CONFIG---\n");
    printf("CONFIG.Reserved => %d\n",REGISTERS.CONFIG.Reserved);
    printf("CONFIG.MASK_RX_DR => %d\n",REGISTERS.CONFIG.MASK_RX_DR);
    printf("CONFIG.MASK_TX_DS => %d\n",REGISTERS.CONFIG.MASK_TX_DS);
    printf("CONFIG.MASK_MAX_RT => %d\n",REGISTERS.CONFIG.MASK_MAX_RT);
    printf("CONFIG.EN_CRC => %d\n",REGISTERS.CONFIG.EN_CRC);
    printf("CONFIG.EN_CRC => %d\n",REGISTERS.CONFIG.CRC0);
    printf("CONFIG.PWR_UP => %d\n",REGISTERS.CONFIG.PWR_UP);
    printf("CONFIG.PRIM_RX => %d\n",REGISTERS.CONFIG.PRIM_RX);

    printf("\n---EN_AA---\n");
    printf("EN_AA.Reserved  => %d\n",REGISTERS.EN_AA.Reserved);
    printf("EN_AA.ENAA_P5  => %d\n",REGISTERS.EN_AA.ENAA_P5);
    printf("EN_AA.ENAA_P4  => %d\n",REGISTERS.EN_AA.ENAA_P4);
    printf("EN_AA.ENAA_P3  => %d\n",REGISTERS.EN_AA.ENAA_P3);
    printf("EN_AA.ENAA_P2  => %d\n",REGISTERS.EN_AA.ENAA_P2);
    printf("EN_AA.ENAA_P1  => %d\n",REGISTERS.EN_AA.ENAA_P1);
    printf("EN_AA.ENAA_P0  => %d\n",REGISTERS.EN_AA.ENAA_P0);

    printf("\n---EN_RXADDR---\n");
    printf("EN_RXADDR.Reserved => %d\n",REGISTERS.EN_RXADDR.Reserved);
    printf("EN_RXADDR.ERX_P5 => %d\n",REGISTERS.EN_RXADDR.ERX_P5);
    printf("EN_RXADDR.ERX_P4 => %d\n",REGISTERS.EN_RXADDR.ERX_P4);
    printf("EN_RXADDR.ERX_P3 => %d\n",REGISTERS.EN_RXADDR.ERX_P3);
    printf("EN_RXADDR.ERX_P2 => %d\n",REGISTERS.EN_RXADDR.ERX_P2);
    printf("EN_RXADDR.ERX_P1 => %d\n",REGISTERS.EN_RXADDR.ERX_P1);
    printf("EN_RXADDR.ERX_P0 => %d\n",REGISTERS.EN_RXADDR.ERX_P0);

    printf("\n---SETUP_AW---\n");
    printf("SETUP_AW.Reserved => %d\n",REGISTERS.SETUP_AW.Reserved);
    printf("SETUP_AW.AW => %d\n",REGISTERS.SETUP_AW.AW);

    printf("\n---SETUP_RETR---\n");
    printf("SETUP_RETR.ARD => %d\n",REGISTERS.SETUP_RETR.ARD);
    printf("SETUP_RETR.ARC => %d\n",REGISTERS.SETUP_RETR.ARC);

    printf("\n---RF_CH---\n");
    printf("RF_CH.Reserved => %d\n", REGISTERS.RF_CH.Reserved);
    printf("RF_CH.RF_CH => %d\n",REGISTERS.RF_CH.RF_CH);

    printf("\n---RF_SETUP---\n");
    printf("RF_SETUP.CONT_WAVE => %d\n",REGISTERS.RF_SETUP.CONT_WAVE);
    printf("RF_SETUP.Reserved => %d\n",REGISTERS.RF_SETUP.Reserved);
    printf("RF_SETUP.RF_DR_LOW => %d\n",REGISTERS.RF_SETUP.RF_DR_LOW);
    printf("RF_SETUP.PLL_LOCK => %d\n",REGISTERS.RF_SETUP.PLL_LOCK);
    printf("RF_SETUP.RF_DR_HIGH => %d\n",REGISTERS.RF_SETUP.RF_DR_HIGH);
    printf("RF_SETUP.RF_PWR => %d\n",REGISTERS.RF_SETUP.RF_PWR);
    printf("RF_SETUP.Obsolete => %d\n",REGISTERS.RF_SETUP.Obsolete);

    printf("\n---STATUS---\n");
    printf("STATUS.Reserved => %d\n",REGISTERS.STATUS.Reserved);
    printf("STATUS.RX_DR => %d\n",REGISTERS.STATUS.RX_DR);
    printf("STATUS.TX_DS => %d\n",REGISTERS.STATUS.TX_DS);
    printf("STATUS.MAX_RT => %d\n",REGISTERS.STATUS.MAX_RT);
    printf("STATUS.RX_P_NO => %d\n",REGISTERS.STATUS.RX_P_NO);
    printf("STATUS.TX_FULL => %d\n",REGISTERS.STATUS.TX_FULL);

    printf("\n---OBSERVE_TX---\n");
    printf("OBSERVE_TX.PLOS_CNT => %d\n",REGISTERS.OBSERVE_TX.PLOS_CNT);
    printf("OBSERVE_TX.ARC_CNT => %d\n",REGISTERS.OBSERVE_TX.ARC_CNT);

    printf("\n---RPD---\n");
    printf("RPD.Reserved => %d\n",REGISTERS.RPD.Reserved);
    printf("RPD.RPD => %d\n",REGISTERS.RPD.RPD);

    printf("\n---RX_ADDR_P0---\n");
    printf("%X%X%X%X%X\n",((byte*)register_array[eRX_ADDR_P0])[0],((byte*)register_array[eRX_ADDR_P0])[1],((byte*)register_array[eRX_ADDR_P0])[2],((byte*)register_array[eRX_ADDR_P0])[3],((byte*)register_array[eRX_ADDR_P0])[4] );
    printf("\n---RX_ADDR_P1---\n");
    printf("%X%X%X%X%X\n",((byte*)register_array[eRX_ADDR_P1])[0],((byte*)register_array[eRX_ADDR_P1])[1],((byte*)register_array[eRX_ADDR_P1])[2],((byte*)register_array[eRX_ADDR_P1])[3],((byte*)register_array[eRX_ADDR_P1])[4] );
    printf("\n---RX_ADDR_P2---\n");
    printf("%X\n",*((byte*)register_array[eRX_ADDR_P2]) );
    printf("\n---RX_ADDR_P3---\n");
    printf("%X\n",*((byte*)register_array[eRX_ADDR_P3]) );
    printf("\n---RX_ADDR_P4---\n");
    printf("%X\n",*((byte*)register_array[eRX_ADDR_P4]) );
    printf("\n---RX_ADDR_P5---\n");
    printf("%X\n",*((byte*)register_array[eRX_ADDR_P5]) );
    printf("\n---TX_ADDR---\n");
    printf("%X%X%X%X%X\n",((byte*)register_array[eTX_ADDR])[0],((byte*)register_array[eTX_ADDR])[1],((byte*)register_array[eTX_ADDR])[2],((byte*)register_array[eTX_ADDR])[3],((byte*)register_array[eTX_ADDR])[4] );

    printf("\n---RX_PW_P0---\n");
    printf("RX_PW_P0.Reserved => %d\n",REGISTERS.RX_PW_P0.Reserved);
    printf("PX_PW_P0.RX_PW_P0 => %d\n",REGISTERS.RX_PW_P0.RX_PW_P);
    printf("\n---RX_PW_P1---\n");
    printf("RX_PW_P1.Reserved => %d\n",REGISTERS.RX_PW_P1.Reserved);
    printf("PX_PW_P1.RX_PW_P => %d\n",REGISTERS.RX_PW_P1.RX_PW_P);
    printf("\n---RX_PW_P2---\n");
    printf("RX_PW_P2.Reserved => %d\n",REGISTERS.RX_PW_P2.Reserved);
    printf("PX_PW_P2.RX_PW_P => %d\n",REGISTERS.RX_PW_P2.RX_PW_P);
    printf("\n---RX_PW_P3---\n");
    printf("RX_PW_P3.Reserved => %d\n",REGISTERS.RX_PW_P3.Reserved);
    printf("PX_PW_P3.RX_PW_P => %d\n",REGISTERS.RX_PW_P3.RX_PW_P);
    printf("\n---RX_PW_P4---\n");
    printf("RX_PW_P4.Reserved => %d\n",REGISTERS.RX_PW_P4.Reserved);
    printf("PX_PW_P4.RX_PW_P => %d\n",REGISTERS.RX_PW_P4.RX_PW_P);
    printf("\n---RX_PW_P5---\n");
    printf("RX_PW_P5.Reserved => %d\n",REGISTERS.RX_PW_P5.Reserved);
    printf("PX_PW_P5.RX_PW_P => %d\n",REGISTERS.RX_PW_P5.RX_PW_P);

    printf("\n---FIFO_STATUS---\n");
    printf("FIFO_STATUS.Reserved2 => %d\n",REGISTERS.FIFO_STATUS.Reserved2);
    printf("FIFO_STATUS.TX_REUSE => %d\n",REGISTERS.FIFO_STATUS.TX_REUSE);
    printf("FIFO_STATUS.TX_FULL => %d\n",REGISTERS.FIFO_STATUS.TX_FULL);
    printf("FIFO_STATUS.TX_EMPTY => %d\n",REGISTERS.FIFO_STATUS.TX_EMPTY);
    printf("FIFO_STATUS.Reserved1 => %d\n",REGISTERS.FIFO_STATUS.Reserved1);
    printf("FIFO_STATUS.RX_FULL => %d\n",REGISTERS.FIFO_STATUS.RX_FULL);
    printf("FIFO_STATUS.RX_EMPTY => %d\n",REGISTERS.FIFO_STATUS.RX_EMPTY);

    printf("\n---DYNPD---\n");
    printf("DYNPD.Reserved => %d\n",REGISTERS.DYNPD.Reserved);
    printf("DYNPD.DPL_P5 => %d\n",REGISTERS.DYNPD.DPL_P5);
    printf("DYNPD.DPL_P4 => %d\n",REGISTERS.DYNPD.DPL_P4);
    printf("DYNPD.DPL_P3 => %d\n",REGISTERS.DYNPD.DPL_P3);
    printf("DYNPD.DPL_P2 => %d\n",REGISTERS.DYNPD.DPL_P2);
    printf("DYNPD.DPL_P1 => %d\n",REGISTERS.DYNPD.DPL_P1);
    printf("DYNPD.DPL_P0 => %d\n",REGISTERS.DYNPD.DPL_P0);

    printf("\n---FEATURE---\n");
    printf("FEATURE.Reserved => %d\n",REGISTERS.FEATURE.Reserved);
    printf("FEATURE.EN_DPL => %d\n",REGISTERS.FEATURE.EN_DPL);
    printf("FEATURE.EN_ACK_PAY => %d\n",REGISTERS.FEATURE.EN_ACK_PAY);
    printf("FEATURE.EN_DYN_ACK => %d\n",REGISTERS.FEATURE.EN_DYN_ACK);
}
