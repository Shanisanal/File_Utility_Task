//****************************** File_Utility_Task  *************************************
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : srec.h
// Summary  : Header file for srec utility module
// Note     : Provides function declarations for srec utilities 
// Author   : Shani 
// Date     : Feb 27, 2026 
//
//*****************************************************************************
#ifndef SREC_H
#define SREC_H

//******************************* Include Files ******************************* 
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
 
//***************************** Global Variables ****************************** 
 
//**************************** Forward Declarations *************************** 
void srec_file(const char *pInput, const char *pOutput) ;
uint8_t Calculate_Srec_Checksum(uint8_t ucCount, uint32_t ulAddr, uint8_t *pData, size_t DataLen) ;


//*********************** Inline Method Implementations *********************** 

#endif

// EOF 