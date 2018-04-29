/*******************************************************
 * Copyright (C) 2018 Michael Grabenschweiger <michael@grabenschweiger.com>
 * 
 * This file is part of Grabenschweiger Graupner SUMD Adapter.
 * 
 * Grabenschweiger SUDM Adapter can not be copied and/or distributed without the express
 * permission of Michael Grabenschweiger
 *******************************************************/
#include "SUMD.h"

#define LENGTH(x) (sizeof(x)/sizeof(*x))


static SUMD* locSUMD = NULL;
void runloop()
{
  if(locSUMD != NULL) locSUMD->loop();
}

void SUMD::begin(HardwareSerial  *serial)
{
  this->serial = serial;
  this->serial->begin(115200);

  locSUMD = this;
}





void SUMD::loop()
{
  while (serial->available() > 0)
  {
    c = serial->read();
    t_us = micros();
    dt_us = t_us - mt_us;

    if(dt_us > 5000)
    {
      State = 0; // Must be Startbyte if there was a pause with minimum 500us
      actChannel = 0;
      for(int i = 0; i < LENGTH(iChannel);i++) iChannel[i] = 0;
    } 

    if     (State == 0 && c == 0xA8            ){ State = 1;                            } // SUMD_Header_0 Vendor ID
    else if(State == 1 && c == 0x01            ){ State = 2;   FailSafe = false;        } // SUMD_Header_1 Status
    else if(State == 1 && c == 0x81            ){ State = 2;   FailSafe = true;         } // SUMD_Header_1 Status  Valid in FailSafeMode
    else if(State == 1                         ){ State = 99;                           } // SUMD_Header_1 Status  InValid
    else if(State == 2 && c  <= LENGTH(Channel)*2 ){ State = 3;   Length = c/2;            } // SUMD_Header_2 Length
    else if(State == 2                         ){ State = 99;                           } // SUMD_Header_2 Length Invalid
    else if(State == 3 && actChannel < Length*2-1)
    {
      iChannel[actChannel/2] += (uint16_t)c << (8*(1-actChannel%2));
      actChannel++;
    }
    else if(State == 3) {State = 100;}
   
    if(State == 99)
    {
      isValid = false;
    }
    if(State ==  100)
    {
      isValid = true;
      for(int i = 0; i < LENGTH(iChannel);i++) Channel[i] = ((int32_t)iChannel[i]-12000)*150.0f / 4800.0f;
    }

    mt_us = t_us;
  }
}
