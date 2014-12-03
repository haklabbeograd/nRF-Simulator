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

void nRF24l01plus::clear_registers(void)
{
    for(int i = eRX_ADDR_P0;i<=eTX_ADDR;i++)
    {
        free(register_array[i]);
    }
}

bool nRF24l01plus::checkIRQ()
{
    return ( (REGISTERS.STATUS.RX_DR && REGISTERS.CONFIG.MASK_RX_DR) || (REGISTERS.STATUS.TX_DS && REGISTERS.CONFIG.MASK_TX_DS) || (REGISTERS.STATUS.MAX_RT && REGISTERS.CONFIG.MASK_MAX_RT) );
}

byte * nRF24l01plus::read_register(byte * read_command)
{                               //76543210
    byte addr = *read_command & 0b00011111;
    return (byte*)register_array[addr];
}

void nRF24l01plus::write_register(byte* bytes_to_write)
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
//constructor
nRF24l01plus::nRF24l01plus():CE(false),PID(0),REUSE_TX_PL(false),lastTransmited(NULL)
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
byte nRF24l01plus::addressToPype(uint64_t address)
{//Return address number if the RX address pipe is turned on and the address matches
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
bool nRF24l01plus::receve_frame(tMsgFrame * theFrame)
{
    /*********check if buffer is full*******/
    if(REGISTERS.FIFO_STATUS.RX_FULL == true)return false;
    /************check if chip enable is set**/
    if(CE == false)return false;
    /*************Check if in RX mode*********/
    if(REGISTERS.CONFIG.PRIM_RX == 0)return false;
    /******Check address********************/
    byte pipe = addressToPype(theFrame->Address);
    if(pipe == -1) return false;

    /***Receve the frame***/
    tMsgFrame * newFrame = new tMsgFrame;
    newFrame->Address = theFrame->Address;
    newFrame->Packet_Control_Field = theFrame->Packet_Control_Field;
    for(int i = 0; i < theFrame->Packet_Control_Field.Payload_length; i++)
    {
        newFrame->Payload[i] = theFrame->Payload[i];
    }

    /********push into RX FIFO*******/
    if(REGISTERS.FIFO_STATUS.RX_EMPTY)
        REGISTERS.STATUS.RX_P_NO = pipe;
    RX_FIFO.push(newFrame);

    if(RX_FIFO.size() == 3)REGISTERS.FIFO_STATUS.RX_FULL = 1;

    REGISTERS.STATUS.RX_DR = 1;
    REGISTERS.FIFO_STATUS.RX_EMPTY = 0;
    //tranzmit ack
    return true;
}

uint8_t nRF24l01plus::read_RX_payload_width()
{
    if(REGISTERS.FIFO_STATUS.RX_EMPTY)return 0;
    tMsgFrame * temp = RX_FIFO.front();

    return temp->Packet_Control_Field.Payload_length;
}

tMsgFrame * nRF24l01plus::read_RX_payload()
{
    if(REGISTERS.CONFIG.PRIM_RX == 1)
    {
        if(RX_FIFO.empty())return NULL;
        //Not empty
        tMsgFrame * temp = RX_FIFO.front();
        RX_FIFO.pop();
        if(RX_FIFO.empty())
        {
            REGISTERS.FIFO_STATUS.RX_EMPTY = 1;
            REGISTERS.STATUS.RX_P_NO = 7;//empty
        }
        else
        {//still has frames in FIFO change waiting pipe
            temp = RX_FIFO.front();
            REGISTERS.STATUS.RX_P_NO = addressToPype(temp->Address);
        }
        REGISTERS.FIFO_STATUS.RX_FULL = 0;
        return temp;
    }
    return NULL;
}

void nRF24l01plus::write_TX_payload(byte * bytes_to_write)
{
    if(REGISTERS.FIFO_STATUS.TX_FULL)return;
    tMsgFrame * newFrame = new tMsgFrame;
    int i = 1;
    while( (bytes_to_write[i]!= 0) && (i < 33) )
    {//writes all the bytes into the payload
        newFrame->Payload[i-1] = bytes_to_write[i];
        i++;
    }
    //Set PCF payload length
    newFrame->Packet_Control_Field.Payload_length = i;
    //Set NO_ACK flag to zero (request ACK);
    newFrame->Packet_Control_Field.NP_ACK=0;
    //Set PID                            //76543210
    newFrame->Packet_Control_Field.PID = 0b00000011 & PID++;
    //Set Address
    newFrame->Address = *( (uint64_t*)register_array[eTX_ADDR] );
    TX_FIFO.push(newFrame);
    REGISTERS.FIFO_STATUS.TX_EMPTY = 0;
    if(TX_FIFO.size() == 3)
    {
        REGISTERS.FIFO_STATUS.TX_FULL = 1;
        REGISTERS.STATUS.TX_FULL = 1;
    }
    REUSE_TX_PL = false;
}

void nRF24l01plus::write_no_ack_payload(byte * bytes_to_write)
{
    if(!REGISTERS.FEATURE.EN_DYN_ACK)return;
    if(REGISTERS.FIFO_STATUS.TX_FULL)return;
    tMsgFrame * newFrame = new tMsgFrame;
    int i = 1;
    while( (bytes_to_write[i]!= 0) && (i < 33) )
    {//writes all the bytes into the payload
        newFrame->Payload[i-1] = bytes_to_write[i];
        i++;
    }
    //Set PCF payload length
    newFrame->Packet_Control_Field.Payload_length = i;
    //Set NO_ACK flag to zero (request ACK);
    newFrame->Packet_Control_Field.NP_ACK=1;
    //Set PID                            //76543210
    newFrame->Packet_Control_Field.PID = 0b00000011 & PID++;
    //Set Address
    newFrame->Address = *( (uint64_t*)register_array[eTX_ADDR] );
    TX_FIFO.push(newFrame);
    REGISTERS.FIFO_STATUS.TX_EMPTY = 0;
    if(TX_FIFO.size() == 3)
    {
        REGISTERS.FIFO_STATUS.TX_FULL = 1;
        REGISTERS.STATUS.TX_FULL = 1;
    }
}
void nRF24l01plus::write_ack_payload(byte * bytes_to_write)
{
    //check if tx fifo is full
    if(REGISTERS.FIFO_STATUS.TX_FULL == 1)return;
    //check if Dynamic Payload is enabled
    if(REGISTERS.FEATURE.EN_DPL == 0) return;

    byte pipe = bytes_to_write[0] & 0b00000111; //3 lsb bist are the pype;
    uint64_t temp64;
    uint64_t ACK_address;
    switch(pipe)
    {
    case 0:
        if(REGISTERS.EN_RXADDR.ERX_P0) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P0] );
        else return;//PO RX not enabled
        break;
    case 1:
        if(REGISTERS.EN_RXADDR.ERX_P1) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return;//P1 RX not enabled
        break;
    case 2:
        if(REGISTERS.EN_RXADDR.ERX_P2) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return;//P2 RX not enabled
        temp64 = (uint64_t) ( *( (uint8_t*)(register_array[eRX_ADDR_P2]) ) );
        ACK_address &= 0xFFFFFF00;
        temp64 &= 0x000000FF;
        ACK_address |= temp64;
        break;
    case 3:
        if(REGISTERS.EN_RXADDR.ERX_P3) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return;//P3 RX not enabled
        temp64 = (uint64_t) ( *( (uint8_t*)(register_array[eRX_ADDR_P3]) ) );
        ACK_address &= 0xFFFFFF00;
        temp64 &= 0x000000FF;
        ACK_address |= temp64;
        break;
    case 4:
        if(REGISTERS.EN_RXADDR.ERX_P4) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return;//P4 RX not enabled
        temp64 = (uint64_t) ( *( (uint8_t*)(register_array[eRX_ADDR_P4]) ) );
        ACK_address &= 0xFFFFFF00;
        temp64 &= 0x000000FF;
        ACK_address |= temp64;
        break;
    case 5:
        if(REGISTERS.EN_RXADDR.ERX_P5) ACK_address = *( (uint64_t*)register_array[eRX_ADDR_P1] );
        else return;//P5 RX not enabled
        temp64 = (uint64_t) ( *( (uint8_t*)(register_array[eRX_ADDR_P5]) ) );
        ACK_address &= 0xFFFFFF00;
        temp64 &= 0x000000FF;
        ACK_address |= temp64;
        break;
    default:
        return;
    }

    tMsgFrame * newFrame = new tMsgFrame;

    int i = 1;
    while( (bytes_to_write[i]!= 0) && (i < 32) )
    {//writes all the bytes into the payload
        newFrame->Payload[i-1] = bytes_to_write[i];
        i++;
    }
    //Set PCF payload length
    newFrame->Packet_Control_Field.Payload_length = i;
    //Set NO_ACK flag to one ( does not request ACK);
    newFrame->Packet_Control_Field.NP_ACK=1;
    newFrame->Packet_Control_Field.PID = 0;
    //Set Address
    newFrame->Address = ACK_address;

    TX_FIFO.push(newFrame);
    REGISTERS.FIFO_STATUS.TX_EMPTY = 0;
    if(TX_FIFO.size() == 3)
    {
        REGISTERS.FIFO_STATUS.TX_FULL = 1;
        REGISTERS.STATUS.TX_FULL = 1;
    }
}

void nRF24l01plus::flush_rx()
{/*****check if device is in RX mode*/
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

void nRF24l01plus::flush_tx()
{/********chec if device is in TX mode*******************/
    if(REGISTERS.CONFIG.PRIM_RX == 0)
    {
        while(TX_FIFO.size())
        {
            tMsgFrame * temp = TX_FIFO.front();
            TX_FIFO.pop();
            delete temp;
        }
        REGISTERS.FIFO_STATUS.TX_FULL = 0;
        REGISTERS.FIFO_STATUS.TX_EMPTY = 1;
        REGISTERS.STATUS.TX_FULL = 0;
        if(lastTransmited != NULL) delete lastTransmited;
        REUSE_TX_PL = false;
    }
}
void nRF24l01plus::reuse_last_transmited_payload(void)
{
    REUSE_TX_PL = true;
}

tMsgFrame * nRF24l01plus::get_ack_packet_for_pipe(uint8_t pipe)
{
    //feature not enabled
    if(!REGISTERS.FEATURE.EN_DYN_ACK) return NULL;
    //No ACK payload saved in TX fifo (tx fifo empty)
    if(REGISTERS.FIFO_STATUS.TX_EMPTY) return NULL;
    uint64_t pipe_address = 0;
    uint8_t last_byte = 0;
    switch(pipe)
    {
    case 0:
        if(REGISTERS.EN_RXADDR.ERX_P0 && REGISTERS.EN_AA.ENAA_P0)
        {
            pipe_address = *((uint64_t *)register_array[eRX_ADDR_P0]);
        }
        break;
    case 1:
        if(REGISTERS.EN_RXADDR.ERX_P1 && REGISTERS.EN_AA.ENAA_P1)
        {
            pipe_address = *((uint64_t *)register_array[eRX_ADDR_P1]);
        }
        break;
    case 2:
        if(REGISTERS.EN_RXADDR.ERX_P2 && REGISTERS.EN_AA.ENAA_P2)
        {
            pipe_address = *((uint64_t *)register_array[eRX_ADDR_P1]);
            last_byte = *((uint8_t*)register_array[eRX_ADDR_P2]);
            pipe_address = (pipe_address & 0xFFFFFFFFFFFFFF00) | (uint64_t)last_byte;
        }
        break;
    case 3:
        if(REGISTERS.EN_RXADDR.ERX_P3 && REGISTERS.EN_AA.ENAA_P3)
        {
            pipe_address = *((uint64_t *)register_array[eRX_ADDR_P1]);
            last_byte = *((uint8_t*)register_array[eRX_ADDR_P3]);
            pipe_address = (pipe_address & 0xFFFFFFFFFFFFFF00) | (uint64_t)last_byte;
        }
        break;
    case 4:
        if(REGISTERS.EN_RXADDR.ERX_P4 && REGISTERS.EN_AA.ENAA_P4)
        {
            pipe_address = *((uint64_t *)register_array[eRX_ADDR_P1]);
            last_byte = *((uint8_t*)register_array[eRX_ADDR_P4]);
            pipe_address = (pipe_address & 0xFFFFFFFFFFFFFF00) | (uint64_t)last_byte;
        }
        break;
    case 5:
        if(REGISTERS.EN_RXADDR.ERX_P5 && REGISTERS.EN_AA.ENAA_P5)
        {
            pipe_address = *((uint64_t *)register_array[eRX_ADDR_P1]);
            last_byte = *((uint8_t*)register_array[eRX_ADDR_P5]);
            pipe_address = (pipe_address & 0xFFFFFFFFFFFFFF00) | (uint64_t)last_byte;
        }
        break;
    default:
        return NULL;
    }
    //test address loaded, check through TX fifo for a match.
    tMsgFrame * the_ack_payload = NULL;
    for(uint8_t i = 0; i < TX_FIFO.size(); i++)
    {
        tMsgFrame * temp = TX_FIFO.front();
        //checking code here
        if(pipe_address == temp->Address)
        {//only load first
            if(the_ack_payload == NULL)
                the_ack_payload = temp;
        }
        TX_FIFO.pop();
        TX_FIFO.push(temp);
    }
    return the_ack_payload;
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
