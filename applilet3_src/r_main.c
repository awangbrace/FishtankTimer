/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2010, 2011 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : Applilet3 for RL78/G13 V1.03.00 [15 Sep 2011]
* Device(s)    : R5F100LE
* Tool-Chain   : IAR Systems iccrl78
* Description  : This file implements main function.
* Creation Date: 01-10-2012
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte0 = 0xFFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte1 = 0xFFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte2 = 0xE8U;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte3 = 0x85U;

/* Set security ID */
#pragma location = "SECUID"
__root const uint8_t secuid[10] = 
    {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

// negative
#define USER_LED_CATHODE P7_bit.no7
#define USER_LED_CATHODE_PM PM7_bit.no7

// Cathode is minus. The cup that holds the junction.
#define DETECTOR_LED_CATHODE P5_bit.no4
#define DETECTOR_LED_CATHODE_PM PM5_bit.no4
//#define DETECTOR_LED_CATHODE_PIM 
#define DETECTOR_LED_CATHODE_PU PU5_bit.no4

// anode is plus; the wedge that pushes upwards.
#define DETECTOR_LED_ANODE P5_bit.no5
#define DETECTOR_LED_ANODE_PM PM5_bit.no5
//#define DETECTOR_LED_ANODE_PIM 
#define DETECTOR_LED_ANODE_PU PU5_bit.no5

#define PM_OUTPUT 0
#define PM_INPUT 1
#define PU_DISABLE 0
#define PIM_NORMAL 0

#define COUNT_LIMIT (4294967295UL) // 2^32-1

void init(void)
{
  // turn on
  USER_LED_CATHODE_PM = PM_OUTPUT;
  USER_LED_CATHODE = 0;

  DETECTOR_LED_ANODE_PM = PM_OUTPUT;
  DETECTOR_LED_ANODE = 0;
}

uint32_t measure(void)
{
  // Charge
  DETECTOR_LED_CATHODE_PM = PM_OUTPUT; // output
  DETECTOR_LED_CATHODE = 1;
  
  for (int i = 0; i < 1000; i++)
  {
    asm("nop"); 
  }

  // turn off
  USER_LED_CATHODE = 1;

  // Set as input
  DETECTOR_LED_CATHODE_PU = PU_DISABLE;
#ifdef DETECTOR_LED_CATHODE_PIM
  DETECTOR_LED_CATHODE_PIM = PIM_NORMAL;
#endif
  DETECTOR_LED_CATHODE_PM = PM_INPUT; // INPUT

  // Count
  for (uint32_t i = 0; i < COUNT_LIMIT; i++)
  {
    if (DETECTOR_LED_CATHODE == 0)
    {
      USER_LED_CATHODE = 0;
      return i;
    }
  }
  
  USER_LED_CATHODE = 0;
  return 0;
}

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void  main(void)
{
  static volatile uint32_t intensity = 0;
  
    /* Start user code. Do not edit comment generated here */
    while (1U)
    {
       intensity = measure();
    }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
