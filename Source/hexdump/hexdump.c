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

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define HEXDUMP_BYTES_PER_LINE 16

//***************************** Local Variables ******************************* 
 
//****************************** Local Functions ******************************

//****************************** Hexdump_file ******************************
// Purpose : Generates a hexadecimal dump of the input file and writes the formatted output (hex values + ASCII equivalents) to
//           the specified output file.
// Inputs  : pInput  - path to the input file 
//           pOutput - path to the output file 
// Outputs : Creates a text file containing the hexdump at the given output path.
// Return  : None
// Notes   : None
//*****************************************************************************
void Hexdump_file(const char *pInput, const char *pOutput) 
{
    FILE *pInputFile = fopen(pInput, "rb");

    FILE *pOutputFile = fopen(pOutput, "w");

    if (!pInputFile || !pOutputFile) 
    {
        perror("File open failed");
        if (pInputFile) 
        {
            fclose(pInputFile);
        }
        if (pOutputFile) 
        {
            fclose(pOutputFile);
        } 
        return;
    }

    uint8_t ucbuffer[HEXDUMP_BYTES_PER_LINE] = {0};    
    size_t bytesRead;
    size_t offset = 0;

    while ((bytesRead = fread(ucbuffer, 1, sizeof(ucbuffer), pInputFile)) > 0) 
    {
        fprintf(pOutputFile, "%08lx  ", offset);

        for (size_t Index = 0; Index < HEXDUMP_BYTES_PER_LINE; Index++) 
        {
            if (Index < bytesRead)
            {
               fprintf(pOutputFile, "%02x ", ucbuffer[Index]);
            }
            else
            {
                fprintf(pOutputFile, "   ");
            }
        }

        // Write ASCII equivalents
        fprintf(pOutputFile, " |");
        for (uint16_t unIndex = 0; unIndex < bytesRead; unIndex++) 
        {
            fprintf(pOutputFile, "%c", isprint(ucbuffer[unIndex]) ? ucbuffer[unIndex] : '.');
        }
        fprintf(pOutputFile, "|\n");

        offset += bytesRead;
    }

    fclose(pInputFile);
    fclose(pOutputFile);
}


//EOF