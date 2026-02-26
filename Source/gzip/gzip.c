//**************************** File_Utility_Task ****************************** 
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//***************************************************************************** 
// 
// File      : gzip.c 
// Summary   : Implementation of gzip compression functionality
// Note      : Provides functions declared in Include/gzip/gzip.h 
// Author    : Shani
// Date      : Feb 26, 2026 
// 
//***************************************************************************** 
 
//******************************* Include Files ******************************* 

#include <stdio.h> 
#include <zlib.h>
#include <stdint.h>

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 

//****************************** gzip_file ******************************
// Purpose : Compresses an input file using the gzip format and writes
//           the compressed data to the specified output file.
// Inputs  : pInput  - path to the input file (const char*)
//           pOutput - path to the output file (const char*)
// Outputs : Creates a gzip-compressed file at the given output path.
// Return  : None 
// Notes   : 
//   - Opens the input file in binary mode ("rb").
//   - Uses zlib's gzopen/gzwrite to perform compression.
//   - Reads data in 4096-byte chunks for efficiency.
//   - Handles errors .
//   - Ensures proper cleanup by closing both input and output files.
//*****************************************************************************
void gzip_file(const char *pInput, const char *pOutput)
{
    FILE *Input_file = fopen(pInput, "rb");
    if (!Input_file) 
    { 
        perror("Error opening input file"); 
        return; 
    }

    gzFile Output_file = gzopen(pOutput, "wb");
    if (!Output_file) 
    { 
        fprintf(stderr, "Error: could not open output file '%s' for gzip compression\n", pOutput);       
        fclose(Input_file); 
        return; 
    }

    uint8_t ucBuffer[4096];
    size_t bytes_read;

    while ((bytes_read = fread(ucBuffer, 1, sizeof(ucBuffer), Input_file)) > 0) 
    {
        if (gzwrite(Output_file, ucBuffer, bytes_read) != (int)bytes_read)
        {
            perror("Error writing compressed data");
            fclose(Input_file);
            gzclose(Output_file);
            return;
        }
    }

    fclose(Input_file);
    gzclose(Output_file);

}

//EOF