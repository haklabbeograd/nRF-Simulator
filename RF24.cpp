/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include "nRF24L01.h"
#include "RF24_config.h"
#include "RF24.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <sys/time.h>
#include <unistd.h>

uint32_t millis()
{
  struct timeval theTime;
  gettimeofday(&theTime, NULL);
  uint32_t uSecs = theTime.tv_usec;
  uSecs = uSecs / 1000;
  return uSecs;
}
/****************************************************************************/

void RF24::csn(int mode)
{
}

/****************************************************************************/

void RF24::ce(int level)
{
  if(level == HIGH)
    {
        theNRF24l01plus->setCE_HIGH();
    }
    else
    {
        theNRF24l01plus->setCE_LOW();
    }
}

/****************************************************************************/

uint8_t RF24::read_register(uint8_t reg, uint8_t* buf, uint8_t len)
{
  byte sentCMD[1] = {R_REGISTER | ( REGISTER_MASK & reg )};
  return theNRF24l01plus->Spi_Write(sentCMD,len,buf);
}

/****************************************************************************/

uint8_t RF24::read_register(uint8_t reg)
{
    byte sentCMD[1] = {R_REGISTER | ( REGISTER_MASK & reg )};
    byte msgBack[1];
    theNRF24l01plus->Spi_Write(sentCMD,1,msgBack);
    printf("\nread_register(reg) reg: 0x%X valueBack: 0x%X",reg,msgBack[0]);
    return msgBack[0];
}

/****************************************************************************/

uint8_t RF24::write_register(uint8_t reg, const uint8_t* buf, uint8_t len)
{
  byte * tempBuf = (byte*)calloc(sizeof(byte),len+2);
  byte placeholder[5];
  memcpy(tempBuf+1,buf,len);
  tempBuf[0] = W_REGISTER | ( REGISTER_MASK & reg );

  return theNRF24l01plus->Spi_Write(tempBuf,len,placeholder);
}

/****************************************************************************/

uint8_t RF24::write_register(uint8_t reg, uint8_t value)
{
  printf("\nRF24: write_register(reg,value) reg: 0x%X value: 0x%X\n",reg,value);
  byte CMDsent[3]={W_REGISTER | ( REGISTER_MASK & reg ),value,0};
  byte placeholder[5];
  return theNRF24l01plus->Spi_Write(CMDsent,1,placeholder);
}

/****************************************************************************/

uint8_t RF24::write_payload(const void* buf, uint8_t len)
{
  byte * tempBuf = (byte*)calloc(sizeof(byte),len+2);
  byte placeholder[5];
  memcpy(tempBuf+1,buf,len);
  tempBuf[0] = W_TX_PAYLOAD;

  return theNRF24l01plus->Spi_Write(tempBuf,len,placeholder);
}

/****************************************************************************/

uint8_t RF24::read_payload(void* buf, uint8_t len)
{
  byte CMDsent[2] = {R_RX_PAYLOAD,0};
  return theNRF24l01plus->Spi_Write(CMDsent,1,(byte*)buf);
}

/****************************************************************************/

uint8_t RF24::flush_rx(void)
{
  byte CMDsent[2] = {FLUSH_RX,0};
  byte placeholder[5];
  return theNRF24l01plus->Spi_Write(CMDsent,1,placeholder);
}

/****************************************************************************/

uint8_t RF24::flush_tx(void)
{
  byte CMDsent[2] = {FLUSH_TX,0};
  byte placeholder[5];
  return theNRF24l01plus->Spi_Write(CMDsent,1,placeholder);
}

/****************************************************************************/

uint8_t RF24::get_status(void)
{
  byte CMDsent[2] = {NOP,0};
  byte placeholder[5];
  return theNRF24l01plus->Spi_Write(CMDsent,1,placeholder);
}

/****************************************************************************/

void RF24::print_status(uint8_t status)
{
  printf("STATUS\t\t = 0x%02x RX_DR=%x TX_DS=%x MAX_RT=%x RX_P_NO=%x TX_FULL=%x\r\n",
           status,
           (status & _BV(RX_DR))?1:0,
           (status & _BV(TX_DS))?1:0,
           (status & _BV(MAX_RT))?1:0,
           ((status >> RX_P_NO) & 0b111),
           (status & _BV(TX_FULL))?1:0
          );
}

/****************************************************************************/

void RF24::print_observe_tx(uint8_t value)
{
  printf("OBSERVE_TX=%02x: POLS_CNT=%x ARC_CNT=%x\r\n",
           value,
           (value >> PLOS_CNT) & 0b1111,
           (value >> ARC_CNT) & 0b1111
          );
}

/****************************************************************************/

void RF24::print_byte_register(const char* name, uint8_t reg, uint8_t qty)
{
  printf("\t%s =",name);
  while (qty--)
    printf(" 0x%02x",read_register(reg++));
  printf("\r\n");
}

/****************************************************************************/

void RF24::print_address_register(const char* name, uint8_t reg, uint8_t qty)
{
  printf("\t%s =",name);

  while (qty--)
  {
    uint8_t buffer[5];
    read_register(reg++,buffer,sizeof buffer);

    printf(" 0x");
    uint8_t* bufptr = buffer + sizeof buffer;
    while( --bufptr >= buffer )
      printf("%02x",*bufptr);
  }

  printf("\r\n");
}

/****************************************************************************/

RF24::RF24(uint8_t _cepin, uint8_t _cspin,nRF24l01plus * theNRFsim):
  ce_pin(_cepin), csn_pin(_cspin), wide_band(true), p_variant(false),
  payload_size(32), ack_payload_available(false), dynamic_payloads_enabled(false),
  pipe0_reading_address(0),theNRF24l01plus(theNRFsim)
{
}

/****************************************************************************/

void RF24::setChannel(uint8_t channel)
{
  // TODO: This method could take advantage of the 'wide_band' calculation
  // done in setChannel() to require certain channel spacing.
  const uint8_t max_channel = 127;
  write_register(RF_CH,std::min(channel,max_channel));
}

/****************************************************************************/

void RF24::setPayloadSize(uint8_t size)
{
  const uint8_t max_payload_size = 32;
  payload_size = std::min(size,max_payload_size);
}

/****************************************************************************/

uint8_t RF24::getPayloadSize(void)
{
  return payload_size;
}

/****************************************************************************/

static const char rf24_datarate_e_str_0[]  = "1MBPS";
static const char rf24_datarate_e_str_1[]  = "2MBPS";
static const char rf24_datarate_e_str_2[]  = "250KBPS";
static const char * const rf24_datarate_e_str_P[]  = {
  rf24_datarate_e_str_0,
  rf24_datarate_e_str_1,
  rf24_datarate_e_str_2,
};
static const char rf24_model_e_str_0[]  = "nRF24L01";
static const char rf24_model_e_str_1[]  = "nRF24L01+";
static const char * const rf24_model_e_str_P[]  = {
  rf24_model_e_str_0,
  rf24_model_e_str_1,
};
static const char rf24_crclength_e_str_0[]  = "Disabled";
static const char rf24_crclength_e_str_1[]  = "8 bits";
static const char rf24_crclength_e_str_2[]  = "16 bits" ;
static const char * const rf24_crclength_e_str_P[]  = {
  rf24_crclength_e_str_0,
  rf24_crclength_e_str_1,
  rf24_crclength_e_str_2,
};
static const char rf24_pa_dbm_e_str_0[]  = "PA_MIN";
static const char rf24_pa_dbm_e_str_1[]  = "PA_LOW";
static const char rf24_pa_dbm_e_str_2[]  = "LA_MED";
static const char rf24_pa_dbm_e_str_3[]  = "PA_HIGH";
static const char * const rf24_pa_dbm_e_str_P[]  = {
  rf24_pa_dbm_e_str_0,
  rf24_pa_dbm_e_str_1,
  rf24_pa_dbm_e_str_2,
  rf24_pa_dbm_e_str_3,
};

void RF24::printDetails(void)
{
  /*print_status(get_status());

  print_address_register(PSTR("RX_ADDR_P0-1"),RX_ADDR_P0,2);
  print_byte_register(PSTR("RX_ADDR_P2-5"),RX_ADDR_P2,4);
  print_address_register(PSTR("TX_ADDR"),TX_ADDR);

  print_byte_register(PSTR("RX_PW_P0-6"),RX_PW_P0,6);
  print_byte_register(PSTR("EN_AA"),EN_AA);
  print_byte_register(PSTR("EN_RXADDR"),EN_RXADDR);
  print_byte_register(PSTR("RF_CH"),RF_CH);
  print_byte_register(PSTR("RF_SETUP"),RF_SETUP);
  print_byte_register(PSTR("CONFIG"),CONFIG);
  print_byte_register(PSTR("DYNPD/FEATURE"),DYNPD,2);

  printf_P(PSTR("Data Rate\t = %S\r\n"),pgm_read_word(&rf24_datarate_e_str_P[getDataRate()]));
  printf_P(PSTR("Model\t\t = %S\r\n"),pgm_read_word(&rf24_model_e_str_P[isPVariant()]));
  printf_P(PSTR("CRC Length\t = %S\r\n"),pgm_read_word(&rf24_crclength_e_str_P[getCRCLength()]));
  printf_P(PSTR("PA Power\t = %S\r\n"),pgm_read_word(&rf24_pa_dbm_e_str_P[getPALevel()]));
*/
theNRF24l01plus->printRegContents();
}

/****************************************************************************/

void RF24::begin(void)
{

  ce(LOW);
  csn(HIGH);

  // Must allow the radio time to settle else configuration bits will not necessarily stick.
  // This is actually only required following power up but some settling time also appears to
  // be required after resets too. For full coverage, we'll always assume the worst.
  // Enabling 16b CRC is by far the most obvious case if the wrong timing is used - or skipped.
  // Technically we require 4.5ms + 14us as a worst case. We'll just call it 5ms for good measure.
  // WARNING: Delay is based on P-variant whereby non-P *may* require different timing.
  usleep(5000);

  // Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
  // WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
  // sizes must never be used. See documentation for a more complete explanation.
  write_register(SETUP_RETR,(0b0100 << ARD) | (0b1111 << ARC));

  // Restore our default PA level
  setPALevel( RF24_PA_MAX ) ;

  // Determine if this is a p or non-p RF24 module and then
  // reset our data rate back to default value. This works
  // because a non-P variant won't allow the data rate to
  // be set to 250Kbps.
  if( setDataRate( RF24_250KBPS ) )
  {
    p_variant = true ;
  }

  // Then set the data rate to the slowest (and most reliable) speed supported by all
  // hardware.
  setDataRate( RF24_1MBPS ) ;

  // Initialize CRC and request 2-byte (16bit) CRC
  setCRCLength( RF24_CRC_16 ) ;

  // Disable dynamic payloads, to match dynamic_payloads_enabled setting
  write_register(DYNPD,0);

  // Reset current status
  // Notice reset and flush is the last thing we do
  write_register(STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

  // Set up default configuration.  Callers can always change it later.
  // This channel should be universally safe and not bleed over into adjacent
  // spectrum.
  setChannel(76);

  // Flush buffers
  flush_rx();
  flush_tx();
}

/****************************************************************************/

void RF24::startListening(void)
{
  write_register(CONFIG, read_register(CONFIG) | _BV(PWR_UP) | _BV(PRIM_RX));
  write_register(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

  // Restore the pipe0 adddress, if exists
  if (pipe0_reading_address)
    write_register(RX_ADDR_P0, reinterpret_cast<const uint8_t*>(&pipe0_reading_address), 5);

  // Flush buffers
  flush_rx();
  flush_tx();

  // Go!
  ce(HIGH);

  // wait for the radio to come up (130us actually only needed)
  usleep(130);
}

/****************************************************************************/

void RF24::stopListening(void)
{
  ce(LOW);
  flush_tx();
  flush_rx();
}

/****************************************************************************/

void RF24::powerDown(void)
{
  write_register(CONFIG,read_register(CONFIG) & ~_BV(PWR_UP));
}

/****************************************************************************/

void RF24::powerUp(void)
{
  write_register(CONFIG,read_register(CONFIG) | _BV(PWR_UP));
}

/******************************************************************/

bool RF24::write( const void* buf, uint8_t len )
{
  bool result = false;

  // Begin the write
  startWrite(buf,len);

  // ------------
  // At this point we could return from a non-blocking write, and then call
  // the rest after an interrupt

  // Instead, we are going to block here until we get TX_DS (transmission completed and ack'd)
  // or MAX_RT (maximum retries, transmission failed).  Also, we'll timeout in case the radio
  // is flaky and we get neither.

  // IN the end, the send should be blocking.  It comes back in 60ms worst case, or much faster
  // if I tighted up the retry logic.  (Default settings will be 1500us.
  // Monitor the send
  uint8_t observe_tx;
  uint8_t status;

  uint32_t sent_at = millis();
  const uint32_t timeout = 500; //ms to wait for timeout
  do
  {
    status = read_register(OBSERVE_TX,&observe_tx,1);
  }
  while( ! ( status & ( _BV(TX_DS) | _BV(MAX_RT) ) ) && ( millis() - sent_at < timeout ) );

  // The part above is what you could recreate with your own interrupt handler,
  // and then call this when you got an interrupt
  // ------------

  // Call this when you get an interrupt
  // The status tells us three things
  // * The send was successful (TX_DS)
  // * The send failed, too many retries (MAX_RT)
  // * There is an ack packet waiting (RX_DR)
  bool tx_ok, tx_fail;
  whatHappened(tx_ok,tx_fail,ack_payload_available);

  //printf("%u%u%u\r\n",tx_ok,tx_fail,ack_payload_available);

  result = tx_ok;

  // Handle the ack packet
  if ( ack_payload_available )
  {
    ack_payload_length = getDynamicPayloadSize();
  }

  // Yay, we are done.

  // Power down
  powerDown();

  // Flush buffers (Is this a relic of past experimentation, and not needed anymore??)
  flush_tx();

  return result;
}
/****************************************************************************/

void RF24::startWrite( const void* buf, uint8_t len )
{
  // Transmitter power-up
  write_register(CONFIG, ( read_register(CONFIG) | _BV(PWR_UP) ) & ~_BV(PRIM_RX) );
  usleep(150);

  // Send the payload
  write_payload( buf, len );

  // Allons!
  ce(HIGH);
  usleep(15);
  ce(LOW);
}

/****************************************************************************/

uint8_t RF24::getDynamicPayloadSize(void)
{
  byte sentCMD[2] = {R_RX_PL_WID,0};
  byte msgBack[1];
  theNRF24l01plus->Spi_Write(sentCMD,1,msgBack);
  return msgBack[0];
}

/****************************************************************************/

bool RF24::available(void)
{
  return available(NULL);
}

/****************************************************************************/

bool RF24::available(uint8_t* pipe_num)
{
  uint8_t status = get_status();

  // Too noisy, enable if you really want lots o data!!
  //IF_SERIAL_DEBUG(print_status(status));

  bool result = ( status & _BV(RX_DR) );

  if (result)
  {
    // If the caller wants the pipe number, include that
    if ( pipe_num )
      *pipe_num = ( status >> RX_P_NO ) & 0b111;

    // Clear the status bit

    // ??? Should this REALLY be cleared now?  Or wait until we
    // actually READ the payload?

    write_register(STATUS,_BV(RX_DR) );

    // Handle ack payload receipt
    if ( status & _BV(TX_DS) )
    {
      write_register(STATUS,_BV(TX_DS));
    }
  }

  return result;
}

/****************************************************************************/

bool RF24::read( void* buf, uint8_t len )
{
  // Fetch the payload
  read_payload( buf, len );

  // was this the last of the data available?
  return read_register(FIFO_STATUS) & _BV(RX_EMPTY);
}

/****************************************************************************/

void RF24::whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready)
{
  // Read the status & reset the status in one easy call
  // Or is that such a good idea?
  uint8_t status = write_register(STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

  // Report to the user what happened
  tx_ok = status & _BV(TX_DS);
  tx_fail = status & _BV(MAX_RT);
  rx_ready = status & _BV(RX_DR);
}

/****************************************************************************/

void RF24::openWritingPipe(uint64_t value)
{
  // Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
  // expects it LSB first too, so we're good.

  write_register(RX_ADDR_P0, reinterpret_cast<uint8_t*>(&value), 5);
  write_register(TX_ADDR, reinterpret_cast<uint8_t*>(&value), 5);

  const uint8_t max_payload_size = 32;
  write_register(RX_PW_P0,std::min(payload_size,max_payload_size));
}

/****************************************************************************/

static const uint8_t child_pipe[]  =
{
  RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5
};
static const uint8_t child_payload_size[]  =
{
  RX_PW_P0, RX_PW_P1, RX_PW_P2, RX_PW_P3, RX_PW_P4, RX_PW_P5
};
static const uint8_t child_pipe_enable[]  =
{
  ERX_P0, ERX_P1, ERX_P2, ERX_P3, ERX_P4, ERX_P5
};

void RF24::openReadingPipe(uint8_t child, uint64_t address)
{
  // If this is pipe 0, cache the address.  This is needed because
  // openWritingPipe() will overwrite the pipe 0 address, so
  // startListening() will have to restore it.
  if (child == 0)
    pipe0_reading_address = address;

  if (child <= 6)
  {
    // For pipes 2-5, only write the LSB
    if ( child < 2 )
      write_register(child_pipe[child], reinterpret_cast<const uint8_t*>(&address), 5);
    else
      write_register(child_pipe[child], reinterpret_cast<const uint8_t*>(&address), 1);

    write_register(child_payload_size[child],payload_size);

    // Note it would be more efficient to set all of the bits for all open
    // pipes at once.  However, I thought it would make the calling code
    // more simple to do it this way.
    write_register(EN_RXADDR,read_register(EN_RXADDR) | _BV(child_pipe_enable[child]));
  }
}

/****************************************************************************/

void RF24::toggle_features(void)
{
  byte sentCMD[3] = {ACTIVATE,0x73,0};
  byte msgBack[1];
  theNRF24l01plus->Spi_Write(sentCMD,1,msgBack);
}

/****************************************************************************/

void RF24::enableDynamicPayloads(void)
{
  // Enable dynamic payload throughout the system
  write_register(FEATURE,read_register(FEATURE) | _BV(EN_DPL) );

  // If it didn't work, the features are not enabled
  if ( ! read_register(FEATURE) )
  {
    // So enable them and try again
    toggle_features();
    write_register(FEATURE,read_register(FEATURE) | _BV(EN_DPL) );
  }

  // Enable dynamic payload on all pipes
  //
  // Not sure the use case of only having dynamic payload on certain
  // pipes, so the library does not support it.
  write_register(DYNPD,read_register(DYNPD) | _BV(DPL_P5) | _BV(DPL_P4) | _BV(DPL_P3) | _BV(DPL_P2) | _BV(DPL_P1) | _BV(DPL_P0));

  dynamic_payloads_enabled = true;
}

/****************************************************************************/

void RF24::enableAckPayload(void)
{
  //
  // enable ack payload and dynamic payload features
  //

  write_register(FEATURE,read_register(FEATURE) | _BV(EN_ACK_PAY) | _BV(EN_DPL) );

  // If it didn't work, the features are not enabled
  if ( ! read_register(FEATURE) )
  {
    // So enable them and try again
    toggle_features();
    write_register(FEATURE,read_register(FEATURE) | _BV(EN_ACK_PAY) | _BV(EN_DPL) );
  }

  //
  // Enable dynamic payload on pipes 0 & 1
  //

  write_register(DYNPD,read_register(DYNPD) | _BV(DPL_P1) | _BV(DPL_P0));
}

/****************************************************************************/

void RF24::writeAckPayload(uint8_t pipe, const void* buf, uint8_t len)
{
  byte * tempBuf = (byte*)calloc(sizeof(byte),len+2);
  byte placeholder[5];
  memcpy(tempBuf+1,buf,len);
  tempBuf[0] = W_ACK_PAYLOAD | ( pipe & 0b111 );

  theNRF24l01plus->Spi_Write(tempBuf,len,placeholder);
}

/****************************************************************************/

bool RF24::isAckPayloadAvailable(void)
{
  bool result = ack_payload_available;
  ack_payload_available = false;
  return result;
}

/****************************************************************************/

bool RF24::isPVariant(void)
{
  return p_variant ;
}

/****************************************************************************/

void RF24::setAutoAck(bool enable)
{
  if ( enable )
    write_register(EN_AA, 0b111111);
  else
    write_register(EN_AA, 0);
}

/****************************************************************************/

void RF24::setAutoAck( uint8_t pipe, bool enable )
{
  if ( pipe <= 6 )
  {
    uint8_t en_aa = read_register( EN_AA ) ;
    if( enable )
    {
      en_aa |= _BV(pipe) ;
    }
    else
    {
      en_aa &= ~_BV(pipe) ;
    }
    write_register( EN_AA, en_aa ) ;
  }
}

/****************************************************************************/

bool RF24::testCarrier(void)
{
  return ( read_register(CD) & 1 );
}

/****************************************************************************/

bool RF24::testRPD(void)
{
  return ( read_register(RPD) & 1 ) ;
}

/****************************************************************************/

void RF24::setPALevel(rf24_pa_dbm_e level)
{
  uint8_t setup = read_register(RF_SETUP) ;
  setup &= ~(_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;

  // switch uses RAM (evil!)
  if ( level == RF24_PA_MAX )
  {
    setup |= (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;
  }
  else if ( level == RF24_PA_HIGH )
  {
    setup |= _BV(RF_PWR_HIGH) ;
  }
  else if ( level == RF24_PA_LOW )
  {
    setup |= _BV(RF_PWR_LOW);
  }
  else if ( level == RF24_PA_MIN )
  {
    // nothing
  }
  else if ( level == RF24_PA_ERROR )
  {
    // On error, go to maximum PA
    setup |= (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;
  }

  write_register( RF_SETUP, setup ) ;
}

/****************************************************************************/

rf24_pa_dbm_e RF24::getPALevel(void)
{
  rf24_pa_dbm_e result = RF24_PA_ERROR ;
  uint8_t power = read_register(RF_SETUP) & (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;

  // switch uses RAM (evil!)
  if ( power == (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) )
  {
    result = RF24_PA_MAX ;
  }
  else if ( power == _BV(RF_PWR_HIGH) )
  {
    result = RF24_PA_HIGH ;
  }
  else if ( power == _BV(RF_PWR_LOW) )
  {
    result = RF24_PA_LOW ;
  }
  else
  {
    result = RF24_PA_MIN ;
  }

  return result ;
}

/****************************************************************************/

bool RF24::setDataRate(rf24_datarate_e speed)
{
  bool result = false;
  uint8_t setup = read_register(RF_SETUP) ;

  // HIGH and LOW '00' is 1Mbs - our default
  wide_band = false ;
  setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH)) ;
  if( speed == RF24_250KBPS )
  {
    // Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
    // Making it '10'.
    wide_band = false ;
    setup |= _BV( RF_DR_LOW ) ;
  }
  else
  {
    // Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
    // Making it '01'
    if ( speed == RF24_2MBPS )
    {
      wide_band = true ;
      setup |= _BV(RF_DR_HIGH);
    }
    else
    {
      // 1Mbs
      wide_band = false ;
    }
  }
  write_register(RF_SETUP,setup);

  // Verify our result
  if ( read_register(RF_SETUP) == setup )
  {
    result = true;
  }
  else
  {
    wide_band = false;
  }

  return result;
}

/****************************************************************************/

rf24_datarate_e RF24::getDataRate( void )
{
  rf24_datarate_e result ;
  uint8_t dr = read_register(RF_SETUP) & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));

  // switch uses RAM (evil!)
  // Order matters in our case below
  if ( dr == _BV(RF_DR_LOW) )
  {
    // '10' = 250KBPS
    result = RF24_250KBPS ;
  }
  else if ( dr == _BV(RF_DR_HIGH) )
  {
    // '01' = 2MBPS
    result = RF24_2MBPS ;
  }
  else
  {
    // '00' = 1MBPS
    result = RF24_1MBPS ;
  }
  return result ;
}

/****************************************************************************/

void RF24::setCRCLength(rf24_crclength_e length)
{
  uint8_t config = read_register(CONFIG) & ~( _BV(CRCO) | _BV(EN_CRC)) ;

  // switch uses RAM (evil!)
  if ( length == RF24_CRC_DISABLED )
  {
    // Do nothing, we turned it off above.
  }
  else if ( length == RF24_CRC_8 )
  {
    config |= _BV(EN_CRC);
  }
  else
  {
    config |= _BV(EN_CRC);
    config |= _BV( CRCO );
  }
  write_register( CONFIG, config ) ;
}

/****************************************************************************/

rf24_crclength_e RF24::getCRCLength(void)
{
  rf24_crclength_e result = RF24_CRC_DISABLED;
  uint8_t config = read_register(CONFIG) & ( _BV(CRCO) | _BV(EN_CRC)) ;

  if ( config & _BV(EN_CRC ) )
  {
    if ( config & _BV(CRCO) )
      result = RF24_CRC_16;
    else
      result = RF24_CRC_8;
  }

  return result;
}

/****************************************************************************/

void RF24::disableCRC( void )
{
  uint8_t disable = read_register(CONFIG) & ~_BV(EN_CRC) ;
  write_register( CONFIG, disable ) ;
}

/****************************************************************************/
void RF24::setRetries(uint8_t delay, uint8_t count)
{
 write_register(SETUP_RETR,(delay&0xf)<<ARD | (count&0xf)<<ARC);
}

// vim:ai:cin:sts=2 sw=2 ft=cpp

