//**************************** File_Utility_Task ****************************** 
//  Copyright (c) 2026 Trenser Technology Solutions
//  All Rights Reserved 
//*****************************************************************************
//
// File      : main.c
// Summary   : Entry point for file compression utility
// Note      : Parses command-line arguments (-t, -i, -o), and calls gzip 
//             functions to compress files.
// Author    : Shani
// Date      : Feb 26, 2026
//
//***************************************************************************** 

//******************************* Include Files *******************************
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
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
// Return  : Returns 0 upon successful completion,
// Notes   : None
//*****************************************************************************
int main(int lArgCount, char* pcArgv[]) 
{
    bool blExecutionResult = false;

    if (lArgCount <= 1 || pcArgv == NULL) 
    {
        fprintf(stderr, FORMAT_MSG);
        return 1;
    }

    blExecutionResult = ExecuteApplication(lArgCount, pcArgv);

    if (blExecutionResult == true) 
    {
        fprintf(stderr, "Application executed successfully.\n");
        return 0;
    }
    else
    {
        fprintf(stdout, "Application execution failed.\n");
        return 1;
    }

    return 0;
}

// EOF
