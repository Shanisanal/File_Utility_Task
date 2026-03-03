//**************************** File_Utility_Task ****************************** 
//  Copyright (c) 2026 Trenser Technology Solutions
//  All Rights Reserved 
//*****************************************************************************
//
// File      : main.c
// Summary   : Entry point for file compression utility
// Note      : Parses command-line arguments (-t, -i, -o), and calls gzip functions to compress
//             files.
// Author    : Shani
// Date      : Feb 26, 2026
//
//***************************************************************************** 

//******************************* Include Files *******************************
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "Include/gzip/gzip.h"
#include "Include/hexdump/hexdump.h"
#include "Include/srec/srec.h"
#include "Include/Common/utility.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
 
//***************************** Local Variables ******************************* 

//****************************** Local Functions ****************************** 

//****************************** main ******************************************
// Purpose : Entry point for the file utility application.
//           Parses command-line arguments and delegates execution
//           to the RunUtility() function.
// Inputs  : iArgc  - total number of command-line arguments
//           pArgv[] - array of argument strings
// Outputs : Executes the requested operation and writes results to the specified output file.
// Return  : int - Returns 0 upon successful completion,
//           non-zero if an error occurs during argument parsing or execution.
// Notes   : 
//   - Calls ParseArguments() to extract type, input, and output parameters.
//   - Calls RunUtility() to perform the requested operation.
//*****************************************************************************
int main(int iArgCount, char *pArgv[]) 
{
    ARGUMENTS stArguments = {0}; 
    bool blParseResult = ParseArguments((uint16_t)iArgCount, (uint8_t **)pArgv , &stArguments);

    if (blParseResult) 
    {
        RunUtility(&stArguments);
    }

    return 0;
}

// EOF
