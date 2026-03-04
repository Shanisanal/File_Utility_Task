//**************************** File_Utility_Task ****************************** 
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//***************************************************************************** 
// 
// File      : hexdump.c 
// Summary   : Implementation of hexdump functionality
// Note      : Provides functions declared in Include/hexdump/hexdump.h 
// Author    : Shani
// Date      : Feb 27, 2026 
// 
//***************************************************************************** 
 
//******************************* Include Files ******************************* 
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include "Common/utility.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define HEXDUMP_BYTES_PER_LINE 16

//***************************** Local Variables ******************************* 
 
//****************************** Local Functions ******************************

//****************************** HexdumpConvert ******************************
// Purpose : Generates a hexadecimal dump of the input file and writes the formatted output (hex values + ASCII equivalents) to
//           the specified output file.
// Inputs  : pucInput  - path to the input file 
//           pucOutput - path to the output file 
// Outputs : None
// Return  : bool - Returns true if the hexdump was successfully generated and written to the output file, false otherwise.
// Notes   : None
//*****************************************************************************
bool HexdumpConvert(uint8_t* pucInput, uint8_t* pucOutput) 
{
    bool blSuccess = false;
    uint8_t ucbuffer[HEXDUMP_BYTES_PER_LINE] = {0};    
    uint32_t ulBytesRead = 0;
    uint32_t ulOffset = 0;
    FILE* pInputFile = NULL;
    FILE* pOutputFile = NULL;

    if(pucInput == NULL || pucOutput == NULL) 
    {
        fprintf(stderr, "Error: Input and output file paths NULL.\n");
        return blSuccess;
    }
    
    pInputFile = fopen((const char*)pucInput, FILE_MODE_READ_BINARY);
    pOutputFile = fopen((const char*)pucOutput, FILE_MODE_WRITE_TEXT);

    if (pInputFile == NULL) 
    { 
        fprintf(stderr, "Error opening input file.\n"); 
        blSuccess = false;
        return blSuccess; 
    }
 
    if (pOutputFile == NULL) 
    { 
        fclose(pInputFile); 
        blSuccess = false;
        return blSuccess; 
    }

    while(true)
    {
        ulBytesRead = fread(ucbuffer, BYTE_SIZE, sizeof(ucbuffer), pInputFile);

        if (ulBytesRead == 0) 
        {
            if (feof(pInputFile)) 
            {
                break; 
            } 
            else 
            {
                perror("Error reading input file");
                fclose(pInputFile);
                fclose(pOutputFile);
                blSuccess = false;
                return blSuccess;
            }
        }

        fprintf(pOutputFile, "%08x  ", ulOffset);

        for (uint32_t ulIndex = 0; ulIndex < HEXDUMP_BYTES_PER_LINE; ulIndex++) 
        {
            if (ulIndex < ulBytesRead)
            {
               fprintf(pOutputFile, "%02x ", ucbuffer[ulIndex]);
            }
            else
            {
                fprintf(pOutputFile, "   ");
            }
        }

        fprintf(pOutputFile, " |");

        for (uint32_t ulIndex = 0; ulIndex < ulBytesRead; ulIndex++) 
        {
            fprintf(pOutputFile, "%c", isprint(ucbuffer[ulIndex]) ? ucbuffer[ulIndex] : '.');
        }

        fprintf(pOutputFile, "|\n");
        ulOffset += ulBytesRead;
    }

    fclose(pInputFile);
    fclose(pOutputFile);
    blSuccess = true;
    return blSuccess;
}

//EOF