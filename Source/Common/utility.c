//**************************** File_Utility_Task ****************************** 
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//***************************************************************************** 
// 
// File      : utility.c 
// Summary   : Implementation of utility functionality
// Note      : Provides functions declared in Include/Common/utility.h 
// Author    : Shani
// Date      : Feb 27, 2026 
// 
//***************************************************************************** 
 
//******************************* Include Files ******************************* 
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "stdbool.h"
#include "Common/utility.h"
#include "gzip/gzip.h"
#include "hexdump/hexdump.h"
#include "srec/srec.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define ARG_TYPE_FLAG           "-t"
#define ARG_INPUT_FLAG          "-i"
#define ARG_OUTPUT_FLAG         "-o"

#define ARG_TYPE_GZIP           "gzip"
#define ARG_TYPE_HEXDUMP        "hexdump"
#define ARG_TYPE_SREC           "srec"

//***************************** Local Variables ******************************* 

//****************************** Local Functions ******************************/

//****************************** ParseArguments ******************************
// Purpose : Extracts command-line arguments and stores them in an ARGUMENTS structure .
// Inputs  : unArgCount - total number of command-line arguments
//           pcArgv[]  - pointer to array of argument strings
//           pstArguments - pointer to ARGUMENTS structure
// Outputs : pstArguments - Arguments values are passed into it
// Return  : bool - populated structure with parsed arguments
// Notes   : 
//   - Recognizes flags: -t <type>, -i <input>, -o <output>
//   - If a flag is missing, the corresponding field remains NULL.
//   - Caller must validate that required arguments are present before use.
//*****************************************************************************

bool ParseArguments(uint16_t unArgCount, char* pcArgv[], ARGUMENTS* pstArguments) 
{
    bool blTypeFound = false;

    if (pcArgv == NULL || pstArguments == NULL) 
    {
        return blTypeFound; 
    }

    for (uint16_t unIndex = 1; unIndex < unArgCount; unIndex++) 
    {
        if (strcmp((char *)pcArgv[unIndex], ARG_TYPE_FLAG) == 0 && (unIndex + 1 < unArgCount)) 
        {
            pstArguments->pucArgumentType = (uint8_t *)pcArgv[unIndex + 1];
            blTypeFound = true;
        } 
        else if (strcmp((char *)pcArgv[unIndex], ARG_INPUT_FLAG) == 0 && (unIndex + 1 < unArgCount)) 
        {
            pstArguments->pucInputFileName = (uint8_t *)pcArgv[unIndex + 1];
            blTypeFound = true;
       } 
        else if (strcmp((char *)pcArgv[unIndex], ARG_OUTPUT_FLAG) == 0 && (unIndex + 1 < unArgCount)) 
        {
            pstArguments->pucOutputFileName = (uint8_t *)pcArgv[unIndex + 1];
            blTypeFound = true;
        }
    }

    return blTypeFound;
}

//****************************** RunUtility ******************************
// Purpose : Executes the file utility based on parsed arguments.Validates required parameters and calls the appropriate
//           processing function depending on the specified type.
// Inputs  : pstArguments - pointer to ARGUMENTS structure
// Outputs : None
// Return  : None 
// Notes   : 
//   - Currently supports "gzip", "hexdump", and "srec" types.
//   - Displays error if unsupported type is provided.
//*****************************************************************************

bool RunUtility(ARGUMENTS* pstArguments) 
{
    bool blConvertSuccess = false;

    if (pstArguments->pucArgumentType == NULL || pstArguments->pucInputFileName == NULL || pstArguments->pucOutputFileName == NULL) 
    {
        fprintf(stderr, "Error: Missing required arguments. Usage: -t <type> -i <inputfilename> -o <outputfilename>\n");
        blConvertSuccess = false;
        return blConvertSuccess;
    }

    if (strcmp((char *)pstArguments->pucArgumentType, ARG_TYPE_GZIP) == 0) 
    {
        blConvertSuccess = GzipConvert(pstArguments->pucInputFileName, pstArguments->pucOutputFileName);
    } 
    else if (strcmp((char *)pstArguments->pucArgumentType, ARG_TYPE_HEXDUMP) == 0) 
    {
        blConvertSuccess = HexdumpConvert(pstArguments->pucInputFileName, pstArguments->pucOutputFileName);
    } 
    else if (strcmp((char *)pstArguments->pucArgumentType, ARG_TYPE_SREC) == 0) 
    {
        blConvertSuccess = SrecConvert(pstArguments->pucInputFileName, pstArguments->pucOutputFileName);
    } 
    else 
    {
        fprintf(stderr, "Error: Unsupported type . Only 'gzip' 'hexdump' and 'srec' is supported.\n");
        blConvertSuccess = false;
    }
    
    return blConvertSuccess;
}
