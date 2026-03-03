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
#include <stdbool.h>
#include "Common/utility.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define GZIP_BUFFER_SIZE       4096

//***************************** Local Variables ******************************* 

//****************************** GzipConvert ******************************
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
bool GzipConvert(uint8_t* pucInput, uint8_t* pucOutput)
{
    bool blSuccess = false;
    gzFile pstOutput_file;
    uint8_t ucBuffer[GZIP_BUFFER_SIZE] = {0};
    uint32_t  ulBytesRead = 0;

    FILE* pInput_file  = fopen((const char *)pucInput, FILE_MODE_READ_BINARY);
    pstOutput_file = gzopen((const char *)pucOutput, FILE_MODE_WRITE_BINARY);

    if (pInput_file == NULL) 
    { 
        perror("Error opening input file"); 
        blSuccess = false;
        return blSuccess; 
    }
 
    if (pstOutput_file == NULL) 
    { 
        fprintf(stderr, "Error: could not open output file '%s' for gzip compression\n", pucOutput);       
        fclose(pInput_file); 
        blSuccess = false;
        return blSuccess; 
    }

    while(true)
    {
        ulBytesRead = fread(ucBuffer, 1, sizeof(ucBuffer), pInput_file);
        if (ulBytesRead == 0) 
        {
        
            if (feof(pInput_file)) 
            {
                break; 
            } 
            else 
            {
                perror("Error reading input file");
                fclose(pInput_file);
                gzclose(pstOutput_file);
                blSuccess = false;
                return blSuccess;
            }
        }
    
        if (gzwrite(pstOutput_file, ucBuffer, ulBytesRead) != (int)ulBytesRead)
        {
            perror("Error writing compressed data");
            fclose(pInput_file);
            gzclose(pstOutput_file);
            blSuccess = false;
            return blSuccess;
        }
    }

    fclose(pInput_file);
    gzclose(pstOutput_file);
    blSuccess = true;

    return blSuccess;
}

//EOF