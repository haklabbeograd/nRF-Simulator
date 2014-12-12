#include "nRF24interface.h"
#include <stdio.h>
#include <string.h>
nRF24interface::nRF24interface():nRF24registers(),PID(0),REUSE_TX_PL(false),lastTransmited(NULL)
{
    //ctor
}

nRF24interface::~nRF24interface()
{
    //dtor
}

commands nRF24interface::get_command(byte command)
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

void nRF24interface::Spi_Write(byte * msg, byte* msgBack)
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

uint8_t nRF24interface::read_RX_payload_width()
{
    if(isFIFO_RX_EMTPY())return 0;
    tMsgFrame * temp = RX_FIFO.front();

    return temp->Packet_Control_Field.Payload_length;
}

tMsgFrame * nRF24interface::read_RX_payload()
{
    if(isRX_MODE())
    {
        if(RX_FIFO.empty())return NULL;
        //Not empty
        tMsgFrame * frontToDel = RX_FIFO.front();
        tMsgFrame * temp = new tMsgFrame;
        memcpy(temp,frontToDel,sizeof(tMsgFrame) );
        RX_FIFO.pop();
        delete frontToDel;
        if(RX_FIFO.empty())
        {
            setRX_EMPTY();
            setRX_P_NO(7);//empty
        }
        else
        {//still has frames in FIFO change waiting pipe
            temp = RX_FIFO.front();
            setRX_P_NO( addressToPype(temp->Address) );
        }
        clearRX_FULL();
        return temp;
    }
    return NULL;
}

void nRF24interface::write_TX_payload(byte * bytes_to_write)
{
    if(isFIFO_TX_FULL())return;
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
    newFrame->Address = getTXaddress();
    TX_FIFO.push(newFrame);
    clearTX_EMPTY();
    if(TX_FIFO.size() == 3)
    {
        setTX_FULL();
        setTX_FULL_IRQ();
    }
    REUSE_TX_PL = false;
}

void nRF24interface::write_no_ack_payload(byte * bytes_to_write)
{
    if(!isDynamicACKEnabled())return;
    if(isFIFO_TX_FULL())return;
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
    newFrame->Address = getTXaddress();
    TX_FIFO.push(newFrame);
    clearTX_EMPTY();
    if(TX_FIFO.size() == 3)
    {
        setTX_FULL();
        setTX_FULL_IRQ();
    }
}

void nRF24interface::write_ack_payload(byte * bytes_to_write)
{
    //check if tx fifo is full
    if(isFIFO_TX_FULL() == 1)return;
    //check if Dynamic Payload is enabled
    if(isDynamicPayloadEnabled() == 0) return;

    byte pipe = bytes_to_write[0] & 0b00000111; //3 lsb bist are the pype;

    uint64_t ACK_address = getAddressFromPipe(pipe);
    if(ACK_address == 0 )return;

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
    clearTX_EMPTY();
    if(TX_FIFO.size() == 3)
    {
        setTX_FULL();
        setTX_FULL_IRQ();
    }
}

void nRF24interface::flush_rx()
{/*****check if device is in RX mode*/
    if(isRX_MODE() == 1)
    {
        while(RX_FIFO.size())
        {
            tMsgFrame * temp = RX_FIFO.front();
            RX_FIFO.pop();
            delete temp;
        }
        setRX_EMPTY();
        clearRX_FULL();
    }
}

void nRF24interface::flush_tx()
{/********chec if device is in TX mode*******************/
    if(isRX_MODE() == 0)
    {
        while(TX_FIFO.size())
        {
            tMsgFrame * temp = TX_FIFO.front();
            TX_FIFO.pop();
            delete temp;
        }
        clearTX_FULL();
        setTX_EMPTY();
        clearTX_FULL_IRQ();
        if(lastTransmited != NULL) delete lastTransmited;
        REUSE_TX_PL = false;
    }
}
void nRF24interface::reuse_last_transmited_payload(void)
{
    REUSE_TX_PL = true;
}

tMsgFrame * nRF24interface::get_ack_packet_for_pipe(uint8_t pipe)
{
    //feature not enabled
    if(!isDynamicACKEnabled()) return NULL;
    //No ACK payload saved in TX fifo (tx fifo empty)
    if(isFIFO_TX_EMPTY()) return NULL;
    //
    uint64_t pipe_address = getAddressFromPipe_ENAA(pipe);
    if(pipe_address == 0)return NULL;
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

bool nRF24interface::receve_frame(tMsgFrame * theFrame)
{
    /*********check if buffer is full*******/
    if(isFIFO_RX_FULL())return false;
    /************check if chip enable is set**/
    if(getCE() == false)return false;
    /*************Check if in RX mode*********/
    if(isRX_MODE() == 0)return false;
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
    if(isFIFO_RX_EMTPY())
        setRX_P_NO(pipe);
    RX_FIFO.push(newFrame);

    if(RX_FIFO.size() == 3)setRX_FULL();

    setRX_DR_IRQ();//issue irq...
    clearRX_EMPTY();
    //tranzmit ack
    return true;
}
