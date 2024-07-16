/**********************************************************************************************************************
    Program Name    : Sample program for Renesas Flash Sample Program (RFSP Type01 for RL78/G15,RL78/G16)
    
    File Name       : sample_types.h
    Program Version : V1.10.00
    Device(s)       : RL78/G15,RL78/G16 microcontroller
    Description     : Sample type definitions header file
**********************************************************************************************************************/

/**********************************************************************************************************************
    DISCLAIMER
    This software is supplied by Renesas Electronics Corporation and is only intended for use with
    Renesas products. No other uses are authorized. This software is owned by Renesas Electronics
    Corporation and is protected under all applicable laws, including copyright laws.
    THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
    TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR
    ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
    BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
    Renesas reserves the right, without notice, to make changes to this software and to discontinue the
    availability of this software. By using this software, you agree to the additional terms and conditions
    found by accessing the following link:
    http://www.renesas.com/disclaimer
    
    Copyright (C) 2021-2022 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

#ifndef SAMPLE_TYPES_H
#define SAMPLE_TYPES_H

/**
 * @enum e_sample_ret_t
 * Definition of the sample operation status
 */
typedef enum e_sample_ret
{
    SAMPLE_ENUM_RET_STS_OK                 = 0x00u,
    SAMPLE_ENUM_RET_ERR_PARAMETER          = 0x10u,
    SAMPLE_ENUM_RET_ERR_CONFIGURATION      = 0x11u,
    SAMPLE_ENUM_RET_ERR_MODE_MISMATCHED    = 0x12u,
    SAMPLE_ENUM_RET_ERR_CHECK_WRITE_DATA   = 0x13u,
    SAMPLE_ENUM_RET_ERR_CFDF_SEQUENCER     = 0x20u,
    SAMPLE_ENUM_RET_ERR_ACT_ERASE          = 0x22u,
    SAMPLE_ENUM_RET_ERR_ACT_WRITE          = 0x23u,
    SAMPLE_ENUM_RET_ERR_CMD_ERASE          = 0x30u,
    SAMPLE_ENUM_RET_ERR_CMD_WRITE          = 0x31u,
} e_sample_ret_t;

#endif /* end of SAMPLE_TYPES_H */
