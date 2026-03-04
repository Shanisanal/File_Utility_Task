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
static bool HexdumpDataWrite(FILE* pInputFile, FILE* pOutputFile);

//****************************** HexdumpConvert ******************************
// Purpose : Generates a hexadecimal dump of the input file and writes the 
//           formatted output(hex values + ASCII equivalents) to the output file.
// Inputs  : pucInput  - path to the input file 
//           pucOutput - path to the output file 
// Outputs : pucOutput file is created with the hexdump of pucInput.
// Return  : bool - Returns true if the hexdump was successfully generated and 
//           written to the output file, false otherwise.
// Notes   : None
//*****************************************************************************
bool HexdumpConvert(uint8_t* pucInput, uint8_t* pucOutput) 
{
    bool blSuccess = false;
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

    blSuccess = HexdumpDataCompre(pInputFile, pOutputFile);

    if(blSuccess == false) 
    {
        fprintf(stderr, "Error generating hexdump.\n");
        fclose(pInputFile);
        fclose(pOutputFile);
        return blSuccess;
    }
    else
    {   
        fclose(pInputFile);
        fclose(pOutputFile);
    }

    return blSuccess;
}

//****************************** HexdumpDataWrite ******************************
// Purpose : writes a formatted hexdump representation to the output text file
// Inputs  : pucInput  - path to the input file 
//           pucOutput - path to the output file 
// Outputs : pucOutput file is created with the hexdump of pucInput.
// Return  : bool - Returns true if the hexdump was successfully generated and 
//           written to the output file, false otherwise.
// Notes   : None
//*****************************************************************************
static bool HexdumpDataWrite(FILE* pInputFile, FILE* pOutputFile)
{
    uint8_t ucbuffer[HEXDUMP_BYTES_PER_LINE] = {0};
    uint32_t ulBytesRead = 0;
    uint32_t ulOffset = 0;
    bool  blHexdumpWriteSuccess = false;
    while(true)
    {
        ulBytesRead = fread(ucbuffer, BYTE_SIZE, sizeof(ucbuffer), pInputFile);

        if (ulBytesRead == 0) 
        {
            if (feof(pInputFile)) 
            {
                blHexdumpWriteSuccess = true;
                break; 
            } 
            else 
            {
                fprintf(stderr, "Error reading input file");
                blHexdumpWriteSuccess = false;
                break;
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
            fprintf(pOutputFile, "%c", 
                    isprint(ucbuffer[ulIndex]) ? ucbuffer[ulIndex] : '.');
        }
        fprintf(pOutputFile, "|\n");
        ulOffset += ulBytesRead;
    }
    return blHexdumpWriteSuccess;
}

//EOF