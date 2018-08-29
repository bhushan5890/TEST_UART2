/*RL78: Edited : 11/08/2018
 * 
 * Copyright (c) 2014, Analog Devices, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \author Ian Martin <martini@redwirellc.com>
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "contiki.h"
#include "watchdog.h"
#include "dsadc.h"
#include "userdefine.h"

#ifdef DRV_IN_ASM
#endif
/*---------------------------------------------------------------------------*/
int contiki_argc = 0;
char **contiki_argv;

static void delay_1sec(void)
{
  /* Delay 1 second */
  register unsigned long int i;
  for(i = 0x000FFFFFUL; i; --i) {
    asm ("nop");
  }
} 

int main(int argc, char **argv)
{

  int a=100;
  PULSELEDMODE=0;
  R_LCD_Create();
  R_LCD_Int(a,1);
  R_SAU1_Create();
  R_UART2_Start();
  bool flip_flop = false;

#ifdef DRV_IN_ASM
    asm
      ("mov P4,#0FFH");
#endif
  asm ("di");
  /* Setup clocks */
  CMC = 0x11U;                                        /* Enable XT1, disable X1 */
  CSC = 0x80U;                                        /* Start XT1 and HOCO, stop X1 */
  CKC = 0x00U;
  delay_1sec();
  OSMC = 0x00;                                       /* Supply fsub to peripherals, including Interval Timer */

  clock_init();


  /* crappy way of remembering and accessing argc/v */
  contiki_argc = argc;
  contiki_argv = argv;

  process_init();
  process_start(&etimer_process, NULL);
//  ctimer_init();
 
  autostart_start(autostart_processes);

  R_UART2_Put_char('H');

  while(1) 
  {
    watchdog_periodic();

    //R_UART2_Put_char('H');

    process_run();

    etimer_request_poll();
  }

  return 0;
}
/*---------------------------------------------------------------------------*/
void
log_message(char *m1, char *m2)
{
  printf("%s%s" NEWLINE, m1, m2);
}

