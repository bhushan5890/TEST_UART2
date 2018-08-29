/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2013, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sau.c
* Version      : Code Generator for RL78/I1B V1.03.00.01 [23 Mar 2017]
* Device(s)    : R5F10MME
* Tool-Chain   : CCRL
* Description  : This file implements device driver for SAU module.
* Creation Date: 8/25/2018
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "macrodriver.h"
#include "uart2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "userdefine.h"
#include "iodefine.h"
#define NO asm ("mov P4,#0FFH")
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment	 generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_uart2_tx_address;        /* uart2 transmit buffer address */
volatile uint16_t  g_uart2_tx_count;           /* uart2 transmit data number */
volatile uint8_t * gp_uart2_rx_address;        /* uart2 receive buffer address */
volatile uint16_t  g_uart2_rx_count;           /* uart2 receive data number */
volatile uint16_t  g_uart2_rx_length;          /* uart2 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU1_Create
* Description  : This function initializes the SAU1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void HOCOON1()
{
	HIOSTOP=0;
	HOCODIV=0;
	CSS=0;
	while(CLS);
	HOCOFC=1;
}
void R_SAU1_Create(void)
{
    HOCOON1();
    SAU1EN = 1U;    /* supply SAU1 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS1 = _0004_SAU_CK00_FCLK_4 | _0040_SAU_CK01_FCLK_4;
//  SPS1 = _0000_SAU_CK00_FCLK_0;
    R_UART2_Create();
}
/***********************************************************************************************************************
* Function Name: R_UART2_Create
* Description  : This function initializes the UART2 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART2_Create(void)
{

    ST1 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART2 receive and transmit */
    STMK2 = 1U;    /* disable INTST2 interrupt */
    STIF2 = 0U;    /* clear INTST2 interrupt flag */
    SRMK2 = 1U;    /* disable INTSR2 interrupt */
    SRIF2 = 0U;    /* clear INTSR2 interrupt flag */
    SREMK2 = 1U;   /* disable INTSRE2 interrupt */
    SREIF2 = 0U;   /* clear INTSRE2 interrupt flag */
    /* Set INTST2 low priority */
    STPR12 = 0U;
    STPR02 = 1U;
    /* Set INTSR2 low priority */
    SRPR12 = 0U;
    SRPR02 = 1U;
    SMR10 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR10 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | 
            _0010_SAU_STOP_1 | _0007_SAU_LENGTH_8;
    SDR10 = _9A00_UART2_TRANSMIT_DIVISOR;
    NFEN0 |= _10_SAU_RXD2_FILTER_ON;
    SIR11 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR11 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR11 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK | _0000_SAU_PARITY_NONE | _0080_SAU_LSB | _0010_SAU_STOP_1 | 
            _0007_SAU_LENGTH_8;
    SDR11 = _9A00_UART2_RECEIVE_DIVISOR;
    SO1 |= _0001_SAU_CH0_DATA_OUTPUT_1;
    SOL1 |= _0000_SAU_CHANNEL0_NORMAL;    /* output level normal */
    SOE1 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART2 output */
    /* Set RxD2 pin */
    PM0 |= 0x01U;
    /* Set TxD2 pin */
    P0 |= 0x02U;
    PM0 &= 0xFDU;
}
/***********************************************************************************************************************
* Function Name: R_UART2_Start
* Description  : This function starts the UART2 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART2_Start(void)
{
    SO1 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE1 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART2 output */
    SS1 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART2 receive and transmit */
    STIF2 = 0U;    /* clear INTST2 interrupt flag */
    SRIF2 = 0U;    /* clear INTSR2 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_UART2_Stop
* Description  : This function stops the UART2 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_UART2_Stop(void)
{
    STMK2 = 1U;    /* disable INTST2 interrupt */
    SRMK2 = 1U;    /* disable INTSR2 interrupt */
    ST1 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART2 receive and transmit */
    SOE1 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable UART2 output */
    STIF2 = 0U;    /* clear INTST2 interrupt flag */
    SRIF2 = 0U;    /* clear INTSR2 interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
void R_UART2_Put_char(char ch)
{
        STMK2 = 1U;    /* disable INTST2 interrupt */
        TXD2 = ch;
        while(!STIF2) ;
        STIF2 = 0;
        STMK2 = 0U;    /* enable INTST2 interrupt */
}
char R_UART2_Get_char()
{
  char c;
  c = RXD2;
  while(!SRIF2) ;
  SRIF2 = 0;
  return c;
}
