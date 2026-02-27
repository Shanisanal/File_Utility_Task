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

//****************************** hexdump_file ******************************
// Purpose : Generates a hexadecimal dump of the input file and writes the formatted output (hex values + ASCII equivalents) to
//           the specified output file.
// Inputs  : pInput  - path to the input file 
//           pOutput - path to the output file 
// Outputs : Creates a text file containing the hexdump at the given output path.
// Return  : None
// Notes   : None
//*****************************************************************************
void hexdump_file(const char *pInput, const char *pOutput) 
{
    FILE *fin = fopen(pInput, "rb");

    FILE *fout = fopen(pOutput, "w");

    if (!fin || !fout) 
    {
        perror("File open failed");
        if (fin) 
        {
            fclose(fin);
        }
        if (fout) 
        {
            fclose(fout);
        } 
        return;
    }

    uint8_t ucbuffer[HEXDUMP_BYTES_PER_LINE];    
    size_t bytesRead;
    size_t offset ;

    while ((bytesRead = fread(ucbuffer, 1, sizeof(ucbuffer), fin)) > 0) 
    {
        fprintf(fout, "%08lx  ", offset);

        // Write hex values
        for (size_t i = 0; i < 16; i++) 
        {
            if (i < bytesRead)
            {
               fprintf(fout, "%02x ", ucbuffer[i]);
            }
            else
            {
                fprintf(fout, "   ");
            }
        }

        // Write ASCII equivalents
        fprintf(fout, " |");
        for (size_t i = 0; i < bytesRead; i++) 
        {
            fprintf(fout, "%c", isprint(ucbuffer[i]) ? ucbuffer[i] : '.');
        }
        fprintf(fout, "|\n");

        offset += bytesRead;
    }

    fclose(fin);
    fclose(fout);
}


//EOF