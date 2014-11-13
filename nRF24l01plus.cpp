#include "nRF24l01plus.h"
#include <stdlib.h>
#include <stdio.h>

void nRF24l01plus::init_registers(void)
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

    register_array[eRX_ADDR_P0]  = (byte*)malloc(5*sizeof(byte));
    ((byte*)register_array[eRX_ADDR_P0])[0] = 0xE7;
    ((byte*)register_array[eRX_ADDR_P0])[1] = 0xE7;
    ((byte*)register_array[eRX_ADDR_P0])[2] = 0xE7;
    ((byte*)register_array[eRX_ADDR_P0])[3] = 0xE7;
    ((byte*)register_array[eRX_ADDR_P0])[4] = 0xE7;

    register_array[eRX_ADDR_P1]  = (byte*)malloc(5*sizeof(byte));
    ((byte*)register_array[eRX_ADDR_P1])[0] = 0xC2;
    ((byte*)register_array[eRX_ADDR_P1])[1] = 0xC2;
    ((byte*)register_array[eRX_ADDR_P1])[2] = 0xC2;
    ((byte*)register_array[eRX_ADDR_P1])[3] = 0xC2;
    ((byte*)register_array[eRX_ADDR_P1])[4] = 0xC2;

    register_array[eRX_ADDR_P2]  = (byte*)malloc(sizeof(byte));
    *((byte*)register_array[eRX_ADDR_P2]) = 0xC3;

    register_array[eRX_ADDR_P3]  = (byte*)malloc(sizeof(byte));
    *((byte*)register_array[eRX_ADDR_P3]) = 0xC4;

    register_array[eRX_ADDR_P4]  = (byte*)malloc(sizeof(byte));
    *((byte*)register_array[eRX_ADDR_P4]) = 0xC5;

    register_array[eRX_ADDR_P5]  = (byte*)malloc(sizeof(byte));
    *((byte*)register_array[eRX_ADDR_P5]) = 0xC6;

    register_array[eTX_ADDR]     = (byte*)malloc(5*sizeof(byte));
    ((byte*)register_array[eTX_ADDR])[0] = 0xE7;
    ((byte*)register_array[eTX_ADDR])[1] = 0xE7;
    ((byte*)register_array[eTX_ADDR])[2] = 0xE7;
    ((byte*)register_array[eTX_ADDR])[3] = 0xE7;
    ((byte*)register_array[eTX_ADDR])[4] = 0xE7;

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

void nRF24l01plus::clear_registers(void)
{
    for(int i = eRX_ADDR_P0;i<=eTX_ADDR;i++)
    {
        free(register_array[i]);
    }
}

byte * nRF24l01plus::read_register(byte * read_command)
{                               //76543210
    byte addr = *read_command & 0b00011111;
    return (byte*)register_array[addr];
}

void nRF24l01plus::write_register(byte* bytes_to_write)
{
    byte addr = bytes_to_write[0] & 0b00011111;
    //no write to these registers

    if( (addr == eOBSERVE_TX)||(addr == eRPD)||(addr == eFIFO_STATUS) )return;

    byte * where_to_write = (byte*)register_array[addr];

    if(addr!=eSTATUS)
        where_to_write[0] = bytes_to_write[1];
    else
        where_to_write[0] = (where_to_write[0] & 0b00001111) | (bytes_to_write[1] & 0b11110000);

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

nRF24l01plus::nRF24l01plus()
{
    //ctor
    init_registers();
}

nRF24l01plus::~nRF24l01plus()
{
    //dtor
    clear_registers();
}
//returns the command from the first byte of the message
commands nRF24l01plus::get_command(byte command)
{
    switch(command)
    {
    case 0b11111111://NOP
        return eNOP;
    case 0b01100001://R_RX_PAYLOAD
        return eR_RX_PAYLOAD;
    case 0b10100000://W_TX_PAYLOAD
        return eW_TX_PAYLOAD;
    case 0b11100001://FLUSH_TX
        return eFLUSH_TX;
    case 0b11100010://FLUSH_RX
        return eFLUSH_RX;
    case 0b11100011://REUSE_TX_PL
        return eREUSE_TX_PL;
    case 0b01100000://R_RX_PL_WID
        return eR_RX_PL_WID;
    case 0b10110000://W_TX_PAYLOAD_NO_ACK
        return eW_TX_PAYLOAD_NO_ACK;
    default:
        break;
    }
    byte three_most_significant_bits = command >> 5;
    switch(three_most_significant_bits)
    {
    case 0b000://R_REGISTER
        return eR_REGISTER;
    case 0b001://W_REGISTER
        return eW_REGISTER;
    case 101://W_ACK_PAYLOAD
        return eW_ACK_PAYLOAD;
    default:
        return eNOP;
    }
}



void nRF24l01plus::Spi_Write(byte * msg,byte * msgBack)
{
    //identify the command from the first byte
    byte * read_reg;
    commands theCommand = get_command(msg[0]);
    byte addr = msg[0] & 0b00011111;
    tMsgFrame * tempMsgFrame = NULL;

    printf("\n\nCOMMAND SENT: %X",msg[0]);
    switch(theCommand)
    {
    case eR_REGISTER:
        read_reg = read_register(msg); //load the register into temp read_reg
        if(read_reg == NULL)break;
        msgBack[1]=read_reg[0];
        printf("\nREAD BYTE[0]: %X",msgBack[1]);
        if( (addr == eRX_ADDR_P0) || (addr == eRX_ADDR_P1) || addr == eTX_ADDR)
        {
            msgBack[2]=read_reg[1];
            msgBack[3]=read_reg[2];
            msgBack[4]=read_reg[3];
            msgBack[5]=read_reg[4];
            printf("\nREAD BYTE[1]: %X",msgBack[2]);
            printf("\nREAD BYTE[2]: %X",msgBack[3]);
            printf("\nREAD BYTE[3]: %X",msgBack[4]);
            printf("\nREAD BYTE[4]: %X",msgBack[5]);
        }
        break;
    case eW_REGISTER:
        write_register(msg);
        break;
    case eR_RX_PL_WID:
        msgBack[1] = read_RX_payload_width();
        break;
    case eR_RX_PAYLOAD:
        tempMsgFrame = read_RX_payload();
        if(tempMsgFrame != NULL)
        {
            for (int i = 0;i<tempMsgFrame->Packet_Control_Field.Payload_length;i++)
            {
                msgBack[i+1] = tempMsgFrame->Payload[i];
            }
            delete tempMsgFrame;
        }
        break;
    case eFLUSH_RX:
        flush_rx();
        break;
    default:
    case eNOP:
        break;
    }

    byte statusCMD = eSTATUS;
    read_reg = read_register(&statusCMD);
    msgBack[0] = read_reg[0];
    printf("\nSTATUS: %X",msgBack[0]);
}

bool nRF24l01plus::receve_frame(tMsgFrame * theFrame)
{
    /*********check if buffer is full*******/
    if(REGISTERS.FIFO_STATUS.RX_FULL)return false;

    /******frame creation*******************/
    tMsgFrame * newFrame = new tMsgFrame;
    newFrame->Address = theFrame->Address;
    newFrame->Packet_Control_Field = theFrame->Packet_Control_Field;
    for(int i = 0; i < theFrame->Packet_Control_Field.Payload_length; i++)
    {
        newFrame->Payload[i] = theFrame->Payload[i];
    }

    /********push into RX FIFO*******/
    RX_FIFO.push(newFrame);
    if(RX_FIFO.size() == 3)REGISTERS.FIFO_STATUS.RX_FULL = 1;

    REGISTERS.STATUS.RX_DR = 1;
    //tranzmit ack
    return true;
}

uint8_t nRF24l01plus::read_RX_payload_width()
{
    if(REGISTERS.FIFO_STATUS.RX_EMPTY)return 0;

    return RX_FIFO.front()->Packet_Control_Field.Payload_length;
}

tMsgFrame * nRF24l01plus::read_RX_payload()
{
    if(REGISTERS.CONFIG.PRIM_RX == 1)
    {
        if(RX_FIFO.empty())return NULL;
        tMsgFrame * temp = RX_FIFO.front();
        RX_FIFO.pop();
        if(RX_FIFO.empty())REGISTERS.FIFO_STATUS.RX_EMPTY = 1;
        REGISTERS.FIFO_STATUS.RX_FULL = 0;
        return temp;
    }
    return NULL;
}

void nRF24l01plus::flush_rx()
{/*****chec if device is in RX mode*/
    if(REGISTERS.CONFIG.PRIM_RX == 1)
    {
        while(RX_FIFO.size())
        {
            tMsgFrame * temp = RX_FIFO.front();
            RX_FIFO.pop();
            delete temp;
        }
        REGISTERS.FIFO_STATUS.RX_EMPTY = 1;
        REGISTERS.FIFO_STATUS.RX_FULL = 0;
    }
}

void nRF24l01plus::printRegContents()
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
