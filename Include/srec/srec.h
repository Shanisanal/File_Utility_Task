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
#include <stdbool.h>

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
 
//***************************** Global Variables ****************************** 
 
//**************************** Forward Declarations *************************** 
bool SrecConvert(uint8_t* pucInput, uint8_t* pucOutput) ;
bool WriteSrecHeaderRecord(FILE* pOutputFile);
bool WriteSrecDataRecord(FILE* pInputFile, FILE* pOutputFile, uint32_t* pulRecordCounter);
bool WriteSrecTerminationRecord(FILE *pOutputFile);
bool WriteSrecCountRecord(FILE* pOutputFile, uint32_t ulRecordCount);
uint8_t CalculateSrecChecksum(uint8_t ucCount, uint32_t ulAddr, uint8_t* pucData, uint32_t ulDataLen) ;


//*********************** Inline Method Implementations *********************** 

#endif

// EOF 