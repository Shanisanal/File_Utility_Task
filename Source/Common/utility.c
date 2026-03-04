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

#define FORMAT_MSG              "Usage: -t <type> -i <input> -o <output>\n"

#define CONVERTER_COUNT (sizeof(sstConversionMap) / sizeof(sstConversionMap[0]))
//***************************** Local Variables ******************************* 
typedef bool (*pfnFileConverter)(uint8_t* pucInput, uint8_t* pucOutput);

typedef struct _CONVERSION_MAP_
 { 
    const char*  pcConversionType; 
    pfnFileConverter pfnFileConverter;
} CONVERSION_MAP;

static const CONVERSION_MAP sstConversionMap[] = 
{
    {ARG_TYPE_GZIP, GzipConvert},
    {ARG_TYPE_HEXDUMP, HexdumpConvert},
    {ARG_TYPE_SREC, SrecConvert}
};

//****************************** Local Functions ******************************/
//****************************** ExecuteApplication ***************************
// Purpose : Manages the application lifecycle from parsing to execution.
// Inputs  : lArgCount  - total number of command-line arguments
//           pcArgv[] - pointer to array of argument strings
// Outputs : None
// Return  : bool - Returns true if execution was successful, false otherwise
// Notes   : Calls ParseArguments and RunUtility functions.
//*****************************************************************************
bool ExecuteApplication(int lArgCount, char* pcArgv[])
{
    ARGUMENTS stArguments = {0};
    bool blParseResult = false;
    bool blRunResult = false;

    if (pcArgv == NULL || lArgCount <= 1) 
    {
        fprintf(stderr, FORMAT_MSG);
        return false;
    }

    blParseResult = ParseArguments(lArgCount, pcArgv, &stArguments);

    if(blParseResult == false) 
    {
        fprintf(stderr, "Error: Failed to parse arguments. %s", FORMAT_MSG);
        return false;
    }
    else
    {
        blRunResult = RunUtility(&stArguments);
        if(blRunResult == false) 
        {
            fprintf(stderr, "Error: Utility execution failed.\n");
            return false;
        }
        else
        {
            return true;
        }
    }
   
    return true;
}
//****************************** ParseArguments *******************************
// Purpose : Extracts command-line arguments and stores them in an ARGUMENTS 
//           structure .
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

bool ParseArguments(uint16_t unArgCount, char* pcArgv[],ARGUMENTS* pstArguments) 
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

//****************************** RunUtility ***********************************
// Purpose : Executes the file utility based on parsed arguments.Validates 
//           required parameters and calls the appropriate processing function
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

    if (pstArguments->pucArgumentType == NULL  ||
        pstArguments->pucInputFileName == NULL ||
        pstArguments->pucOutputFileName == NULL) 
    {
        fprintf(stderr, "Error: Missing required arguments. %s", FORMAT_MSG);
        blConvertSuccess = false;
        return blConvertSuccess;
    }

    for(uint32_t ulIndex = 0; ulIndex < CONVERTER_COUNT; ulIndex++) 
    {
        if (strcmp((char *)pstArguments->pucArgumentType, 
                   (char *)sstConversionMap[ulIndex].pucConversionType) == 0) 
        {
            blConvertSuccess = sstConversionMap[ulIndex].pfnFileConverter(
                                           pstArguments->pucInputFileName, 
                                           pstArguments->pucOutputFileName);
            break;
        }
    }
    
    return blConvertSuccess;
}
