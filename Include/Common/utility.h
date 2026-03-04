//****************************** File_Utility_Task  *************************************
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : utility.h
// Summary  : Header file for utility functions
// Note     : Provides function declarations for utility functions 
// Author   : Shani 
// Date     : Feb 27, 2026 
//
//*****************************************************************************
#ifndef UTILITY_H
#define UTILITY_H

//******************************* Include Files ******************************* 
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
#define FILE_MODE_READ_BINARY  "rb"
#define FILE_MODE_WRITE_BINARY "wb"
#define FILE_MODE_WRITE_TEXT   "w"
#define BYTE_SIZE               1 
//***************************** Global Variables ****************************** 
 
 typedef struct _ARGUMENTS_
 { 
    uint8_t*  pucArgumentType; 
    uint8_t*  pucInputFileName; 
    uint8_t*  pucOutputFileName; 
} ARGUMENTS; 

//**************************** Forward Declarations *************************** 
bool ParseArguments(uint16_t unArgCount, char* pcArgv[], ARGUMENTS* pstArguments) ;
bool RunUtility(ARGUMENTS* pstArguments) ;

//*********************** Inline Method Implementations *********************** 

#endif

// EOF 