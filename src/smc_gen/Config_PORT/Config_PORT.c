/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2021, 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_PORT.c
* Component Version: 1.4.0
* Device(s)        : R5F121BCxFP
* Description      : This file implements device driver for Config_PORT.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_PORT.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_PORT_Create
* Description  : This function initializes the port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_PORT_Create(void)
{
    /* Set PORT0 registers */
    PU0 = _80_PUn7_PULLUP_ON | _40_PUn6_PULLUP_ON | _00_PUn5_PULLUP_OFF | _00_PUn4_PULLUP_OFF | _00_PUn3_PULLUP_OFF | 
          _00_PUn2_PULLUP_OFF | _00_PUn1_PULLUP_OFF | _00_PUn0_PULLUP_OFF;
    PMC0 = _01_PMC0_DEFAULT | _00_PMCn7_DIGITAL_ON | _00_PMCn6_DIGITAL_ON | _20_PMCn5_NOT_USE | _10_PMCn4_NOT_USE | 
           _08_PMCn3_NOT_USE | _04_PMCn2_NOT_USE | _02_PMCn1_NOT_USE | _01_PMCn0_NOT_USE;
    PM0 = _80_PMn7_MODE_INPUT | _40_PMn6_MODE_INPUT | _20_PMn5_NOT_USE | _10_PMn4_NOT_USE | _08_PMn3_NOT_USE | 
          _04_PMn2_NOT_USE | _02_PMn1_NOT_USE | _01_PMn0_NOT_USE;
    /* Set PORT1 registers */
    P1 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _04_Pn2_OUTPUT_1 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PU1 = _80_PUn7_PULLUP_ON | _40_PUn6_PULLUP_ON | _00_PUn5_PULLUP_OFF | _00_PUn4_PULLUP_OFF | _00_PUn3_PULLUP_OFF | 
          _00_PUn2_PULLUP_OFF | _00_PUn1_PULLUP_OFF | _00_PUn0_PULLUP_OFF;
    POM1 = _00_POMn7_NCH_OFF | _00_POMn6_NCH_OFF | _00_POMn4_NCH_OFF;
    PM1 = _80_PMn7_MODE_INPUT | _40_PMn6_MODE_INPUT | _00_PMn5_MODE_OUTPUT | _10_PMn4_NOT_USE | _08_PMn3_NOT_USE | 
          _00_PMn2_MODE_OUTPUT | _02_PMn1_NOT_USE | _01_PMn0_NOT_USE;
    /* Set PORT2 registers */
    P2 = _00_Pn3_OUTPUT_0 | _04_Pn2_OUTPUT_1 | _02_Pn1_OUTPUT_1 | _00_Pn0_OUTPUT_0;
    POM2 = _00_POMn2_NCH_OFF | _00_POMn0_NCH_OFF;
    PMC2 = _F0_PMC2_DEFAULT | _08_PMCn3_NOT_USE | _00_PMCn2_DIGITAL_ON | _00_PMCn1_DIGITAL_ON | _01_PMCn0_NOT_USE;
    PM2 = _F0_PM2_DEFAULT | _08_PMn3_NOT_USE | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _01_PMn0_NOT_USE;
    /* Set PORT12 registers */
    P12 = _00_Pn5_OUTPUT_0 | _04_Pn2_OUTPUT_1 | _02_Pn1_OUTPUT_1;
    PM12 = _D9_PM12_DEFAULT | _20_PMn5_NOT_USE | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT;

    R_Config_PORT_Create_UserInit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

