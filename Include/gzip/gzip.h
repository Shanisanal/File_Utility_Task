//****************************** File_Utility_Task  *************************************
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : gzip.h
// Summary  : Header file for gzip compression module
// Note     : Provides function declarations for gzip compression utilities 
// Author   : Shani 
// Date     : Feb 26, 2026 
//
//*****************************************************************************

#ifndef GZIP_H 
#define GZIP_H

//******************************* Include Files ******************************* 
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

//******************************* Global Types ******************************** 
 
//***************************** Global Constants ****************************** 
 
//***************************** Global Variables ****************************** 
 
//**************************** Forward Declarations *************************** 

bool GzipConvert(uint8_t* pucInput, uint8_t* pucOutput);

//***************************** Class Declaration ***************************** 
 
//*********************** Inline Method Implementations *********************** 

#endif

// EOF 