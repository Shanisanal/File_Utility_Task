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
// Inputs  : lArgCount  - total number of command-line arguments
//           pcArgv[] - array of argument strings
// Outputs : None
// Return  : int - Returns 0 upon successful completion,
// Notes   : None
//*****************************************************************************
int main(int lArgCount, char* pcArgv[]) 
{
    ARGUMENTS stArguments = {0}; 

    if(pcArgv == NULL || lArgCount <= 1) 
    {
        fprintf(stderr, "Error: Command line format -t <type> -i <inputfilename> -o <outputfilename>\n");
        return 1;
    }   

    // bool blParseResult = ParseArguments(lArgCount, pcArgv , &stArguments);

    // if (blParseResult == true) 
    // {
    //     if(RunUtility(&stArguments) == true)
    //     {
    //         printf("Utility executed successfully.\n");
    //     }
    //     else
    //     {
    //         fprintf(stderr, "Error: Utility execution failed.\n");
    //         return 1;
    //     }
    // }
    // else
    // {
    //     fprintf(stderr, "Error parsing command-line arguments\n");
    //     return 1;
    // }

    if(ParseArguments(lArgCount, pcArgv , &stArguments) == false) 
    {
        fprintf(stderr, "Error parsing command-line arguments\n");
        return 1;
    }

    if(RunUtility(&stArguments) == false)
    {
        fprintf(stderr, "Error: Utility execution failed.\n");
        return 1;
    }

    return 0;
}

// EOF
