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

#define CONVERTER_COUNT (sizeof(sstConversionMap) / sizeof(sstConversionMap[0]))

//***************************** Local Variables ******************************* 
typedef bool (*pFileConverter)(uint8_t* pucInput, uint8_t* pucOutput);

typedef bool (*pArgHandler)(ARGUMENTS* pstArgs, const char* pcValue);

typedef struct _CONVERSION_MAP_
 { 
    const char*  pcConversionType; 
    pFileConverter pfnFileConverter;
} CONVERSION_MAP;

typedef enum 
{
    ARG_INVALID = 0,
    ARG_TYPE,
    ARG_INPUT_FILENAME,
    ARG_OUTPUT_FILENAME
} ARG_FLAG;

//****************************** Local Functions ******************************/
static ARG_FLAG GetArgumentFlag(const char* pcArg) ;
static bool ValidateArgumentType(uint8_t* pucArgumentType) ;
static bool HandleType(ARGUMENTS* pstArgs, const char* pcValue) ;
static bool HandleInput(ARGUMENTS* pstArgs, const char* pcValue) ;
static bool HandleOutput(ARGUMENTS* pstArgs, const char* pcValue) ;

static const CONVERSION_MAP sstConversionMap[] = 
{
    {ARG_TYPE_GZIP, GzipConvert},
    {ARG_TYPE_HEXDUMP, HexdumpConvert},
    {ARG_TYPE_SREC, SrecConvert}
};

static const pArgHandler sstArgHandlers[] = 
{
    NULL, HandleType, HandleInput, HandleOutput
};

//****************************** ExecuteApplication ***************************
// Purpose : Parses arguments and executes the appropriate utility function.
// Inputs  : lArgCount  - total number of command-line arguments
//           pcArgv[] - pointer to array of argument strings
// Outputs : None
// Return  : Returns true if execution was successful, false otherwise
// Notes   : Calls ParseArguments and RunUtility functions.
//*****************************************************************************
bool ExecuteApplication(int lArgCount, char* pcArgv[])
{
    ARGUMENTS stArguments = {0};
    bool blConversionResult = true;
    
    if (pcArgv == NULL || lArgCount <= 1) 
    {
        fprintf(stderr, FORMAT_MSG);
        blConversionResult = false;
    }

    if(blConversionResult == true) 
    {
        blConversionResult = ParseArguments(lArgCount, pcArgv, &stArguments);
    }

    if(blConversionResult == true) 
    {
        blConversionResult = RunUtility(&stArguments);

        if(blConversionResult == false)
        {
            fprintf(stderr, "Error: Utility execution failed.\n");
        }
    }
    else
    {
        fprintf(stderr, "Error: Failed to parse arguments. %s", FORMAT_MSG);
    }
   
    return blConversionResult;
}
//****************************** ParseArguments *******************************
// Purpose : Extracts command-line arguments and stores them in an ARGUMENTS 
//           structure .
// Inputs  : lArgCount - total number of command-line arguments
//           pcArgv[]  - pointer to array of argument strings
//           pstArguments - pointer to ARGUMENTS structure
// Outputs : pstArguments - Arguments values are passed into it
// Return  : true if arguments were parsed successfully, false otherwise
// Notes   : 
//   - Recognizes flags: -t <type>, -i <input>, -o <output>
//   - If a flag is missing, the corresponding field remains NULL.
//   - Caller must validate that required arguments are present before use.
//*****************************************************************************
bool ParseArguments(int lArgCount, char* pcArgv[],ARGUMENTS* pstArguments) 
{
    bool blTypeFound = true;

    if (pcArgv == NULL || pstArguments == NULL || lArgCount <= 1) 
    {
        blTypeFound = false;
    }

    if(blTypeFound == true)
    {
        for (int lIndex = 1; lIndex < lArgCount; lIndex ++) 
        {
            if (lIndex + 1 < lArgCount) 
            {
                ARG_FLAG eFlag = GetArgumentFlag(pcArgv[lIndex]);

                if (eFlag > ARG_INVALID ) 
                {
                    blTypeFound = sstArgHandlers[eFlag](pstArguments, pcArgv[lIndex + 1]);
                
                    if(blTypeFound == true) 
                    {
                        lIndex++; 
                    }
                    else 
                    {
                        break;
                    }
                }
                else 
                {
                    blTypeFound = false;
                    break;
                }
            }
            else 
            {
                blTypeFound = false;
                break;
            }
        }

    }

    return blTypeFound;
}

//****************************** HandleType *******************************
// Purpose : Handles the type argument and validates it.
// Inputs  : pstArguments - pointer to ARGUMENTS structure
//           pcValue - pointer to the argument value string
// Outputs : pstArguments - Arguments values are passed into it
// Return  : true if the type argument is valid, false otherwise
// Notes   : None
//*****************************************************************************
static bool HandleType(ARGUMENTS* pstArgs, const char* pcValue) 
{
    bool blValidateSuccess = true;

    if(pstArgs == NULL || pcValue == NULL) 
    {
        blValidateSuccess = false;
    }

    blValidateSuccess =  ValidateArgumentType((uint8_t*)pcValue);
    
    if (blValidateSuccess == true) 
    {
        pstArgs->pucArgumentType = (uint8_t*)pcValue;
    } 

    return blValidateSuccess;
}

//****************************** HandleInput *******************************
// Purpose : Handles the input filename argument and validates it.
// Inputs  : pstArguments - pointer to ARGUMENTS structure
//           pcValue - pointer to the argument value string
// Outputs : pstArguments - Arguments values are passed into it
// Return  : true if the input filename argument is valid, false otherwise
// Notes   : None
//*****************************************************************************
static bool HandleInput(ARGUMENTS* pstArgs, const char* pcValue) 
{
    bool blValidateSuccess = true;
    
    if(pstArgs == NULL || pcValue == NULL) 
    {
        blValidateSuccess = false;
    }

    if(blValidateSuccess == true) 
    {
        pstArgs->pucInputFileName = (uint8_t*)pcValue;
    }

    return blValidateSuccess;
}

//****************************** HandleOutput *******************************
// Purpose : Handles the output filename argument and validates it.
// Inputs  : pstArguments - pointer to ARGUMENTS structure
//           pcValue - pointer to the argument value string
// Outputs : pstArguments - Arguments values are passed into it
// Return  : true if the output filename argument is valid, false otherwise
// Notes   : None
//*****************************************************************************
static bool HandleOutput(ARGUMENTS* pstArgs, const char* pcValue) 
{
    bool blValidateSuccess = true;

    if(pstArgs == NULL || pcValue == NULL) 
    {
        blValidateSuccess = false;
    }

    if(blValidateSuccess == true) 
    {
        pstArgs->pucOutputFileName = (uint8_t*)pcValue;
    }

    return blValidateSuccess;
}

//****************************** ValidateArgumentType **************************
// Purpose : Validates the argument type against a list of supported types.
// Inputs  : pucArgumentType - pointer to the argument type string
// Outputs : None
// Return  : true if the argument type is valid, false otherwise
// Notes   : 
//   - Checks against a predefined list of supported conversion types.
//*****************************************************************************
static bool ValidateArgumentType(uint8_t* pucArgumentType) 
{
    bool blIsValidType = true;

    if(pucArgumentType == NULL) 
    {
        blIsValidType = false;
    }
    
    if(blIsValidType == true) 
    {
        for(uint32_t ulIndex = 0; ulIndex < CONVERTER_COUNT; ulIndex++) 
        {
            if (strcmp((char *)pucArgumentType, 
                       (char *)sstConversionMap[ulIndex].pcConversionType) == 0) 
            {
                blIsValidType = true;
                break;
            }
            else 
            {
                blIsValidType = false;
            }
        }
    }
    return blIsValidType;
}

//****************************** GetArgumentFlag *******************************
// Purpose : Identifies the flag type for a given command-line argument.
// Inputs  : pcArg - pointer to the argument string
// Outputs : None
// Return  : ARG_FLAG - the identified flag type
// Notes   : 
//   - Recognizes flags: -t <type>, -i <input>, -o <output>
//   - Returns ARG_INVALID for unrecognized flags.
//*****************************************************************************
static ARG_FLAG GetArgumentFlag(const char* pcArg) 
{
    ARG_FLAG eFoundFlag = ARG_INVALID;

    if (pcArg == NULL) 
    {
        eFoundFlag = ARG_INVALID;
    }

    if (strcmp(pcArg, ARG_TYPE_FLAG) == 0) 
    {
        eFoundFlag =  ARG_TYPE;
    } 
    else if (strcmp(pcArg, ARG_INPUT_FLAG) == 0) 
    {
        eFoundFlag =  ARG_INPUT_FILENAME;
    } 
    else if (strcmp(pcArg, ARG_OUTPUT_FLAG) == 0) 
    {
        eFoundFlag =  ARG_OUTPUT_FILENAME;
    } 
    else 
    {
        eFoundFlag =  ARG_INVALID;
    }

    return eFoundFlag;
}
//****************************** RunUtility ***********************************
// Purpose : Executes the file utility based on parsed arguments.Validates 
//           required parameters and calls the appropriate processing function
// Inputs  : pstArguments - pointer to ARGUMENTS structure
// Outputs : None
// Return  : true if utility executed successfully, false otherwise 
// Notes   : 
//   - Currently supports "gzip", "hexdump", and "srec" types.
//   - Displays error if unsupported type is provided.
//*****************************************************************************

bool RunUtility(ARGUMENTS* pstArguments) 
{
    bool blConvertSuccess = true;

    if (pstArguments->pucArgumentType == NULL  ||
        pstArguments->pucInputFileName == NULL ||
        pstArguments->pucOutputFileName == NULL) 
    {
        blConvertSuccess = false;
    }
    
    if(blConvertSuccess == true) 
    {
        for(uint32_t ulIndex = 0; ulIndex < CONVERTER_COUNT; ulIndex++) 
        {
            if (strcmp((char *)pstArguments->pucArgumentType, 
                    (char *)sstConversionMap[ulIndex].pcConversionType) == 0) 
            {
                blConvertSuccess = sstConversionMap[ulIndex].pfnFileConverter(
                                            pstArguments->pucInputFileName, 
                                            pstArguments->pucOutputFileName);
                break;
            }
        }
    }

    return blConvertSuccess;
}
