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

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 

//***************************** Local Variables ******************************* 
 typedef struct 
 { 
    uint8_t  *pType; 
    uint8_t  *pInput; 
    uint8_t  *pOutput; 
} ARGUMENTS; 
//****************************** Local Functions ******************************/

//****************************** ParseArguments ******************************
// Purpose : Extracts command-line arguments and stores them in an ARGUMENTS structure .
// Inputs  : unArgc  - total number of command-line arguments
//           pArgv[] - array of argument strings
// Outputs : ARGUMENTS structure containing parsed values:
//              pType   - gzip / hexdump / srec
//              pInput  - input filename
//              pOutput - output filename 
// Return  : ARGUMENTS - populated structure with parsed arguments
// Notes   : 
//   - Recognizes flags: -t <type>, -i <input>, -o <output>
//   - If a flag is missing, the corresponding field remains NULL.
//   - Caller must validate that required arguments are present before use.
//*****************************************************************************

ARGUMENTS ParseArguments(uint16_t unArgc, uint8_t *pArgv[]) 
{
    ARGUMENTS args = {0};

    for (uint16_t unIndex = 1; unIndex < unArgc; unIndex++) 
    {
        if (strcmp((char *)pArgv[unIndex], "-t") == 0 && (unIndex + 1 < unArgc)) 
        {
            args.pType = pArgv[unIndex + 1];
        } 
        else if (strcmp((char *)pArgv[unIndex], "-i") == 0 && (unIndex + 1 < unArgc)) 
        {
            args.pInput = pArgv[unIndex + 1];
        } 
        else if (strcmp((char *)pArgv[unIndex], "-o") == 0 && (unIndex + 1 < unArgc)) 
        {
            args.pOutput = pArgv[unIndex + 1];
        }
    }

    return args;
}

//****************************** RunUtility ******************************
// Purpose : Executes the file utility based on parsed arguments.Validates required parameters and calls the appropriate
//           processing function depending on the specified type.
// Inputs  : args - ARGUMENTS structure
// Outputs : Processed file written to the specified output filename, depending on the operation type.
// Return  : None 
// Notes   : 
//   - Currently supports "gzip", "hexdump", and "srec" types.
//   - Displays error if unsupported type is provided.
//*****************************************************************************

void RunUtility(ARGUMENTS args) 
{
    if (args.pType == NULL || args.pInput == NULL || args.pOutput == NULL) 
    {
        fprintf(stderr, "Error: Missing required arguments. Usage: -t <type> -i <inputfilename> -o <outputfilename>\n");
        return;
    }

    if (strcmp((char *)args.pType, "gzip") == 0) 
    {
        gzip_file((char *)args.pInput, (char *)args.pOutput);
    } 
    else if (strcmp((char *)args.pType, "hexdump") == 0) 
    {
        Hexdump_file((char *)args.pInput, (char *)args.pOutput);
    } 
    else if (strcmp((char *)args.pType, "srec") == 0) 
    {
        srec_file((char *)args.pInput, (char *)args.pOutput);
    } 
    else 
    {
        fprintf(stderr, "Error: Unsupported type . Only 'gzip' 'hexdump' and 'srec' is supported.\n");
    }
}
