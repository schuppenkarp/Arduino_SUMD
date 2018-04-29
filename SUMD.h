/*******************************************************
 * Copyright (C) 2018 Michael Grabenschweiger <michael@grabenschweiger.com>
 * 
 * This file is part of Grabenschweiger Graupner SUMD Adapter.
 * 
 * Grabenschweiger SUMD Adapter can not be copied and/or distributed without the express
 * permission of Michael Grabenschweiger
 *******************************************************/
#ifndef SUMD_H
#define SUMD_H

	#include <Arduino.h>
  



	
	class SUMD
	{
    private:
      HardwareSerial *serial;
      int State = 0;
      
      int actChannel = 0;

      uint64_t mt_us = 0;
      uint64_t  t_us = 0;
      uint64_t  dt_us = 0;
      char c;
		public:
      bool  isValid = false;
      bool  FailSafe = false;
      int   Length = 0;
      float Channel[16] = {0};

      uint16_t iChannel[16] = {0};
			void begin(HardwareSerial *serial);
      void loop();
	};


#endif
