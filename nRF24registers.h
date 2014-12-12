#ifndef NRF24REGISTERS_H
#define NRF24REGISTERS_H

#include <stdint.h>
#include "nRF24bits_struct.h"


class nRF24registers
{
    public:
        /** Default constructor */
        nRF24registers();
        /** Default destructor */
        ~nRF24registers();
        void printRegContents();
        bool checkIRQ();
        void setCE_HIGH(){CE = true;}
        void setCE_LOW(){CE = false;}
        bool getCE(){return CE;}
    protected:
        byte * read_register(byte * read_command);
        void write_register(byte * bytes_to_write);
        byte addressToPype(uint64_t address);
        uint64_t getAddressFromPipe(byte pipe);

        tREGISTERS REGISTERS;
        void * register_array[0x1E];
        bool CE;
    private:
};

#endif // NRF24REGISTERS_H
