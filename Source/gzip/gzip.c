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
#include "gzip/gzip.h"

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define GZIP_BUFFER_SIZE       4096

//***************************** Local Variables ******************************* 

//****************************** Local Functions ******************************
static bool GzipDataCompress(FILE* pInput_file, gzFile pstOutput_file);

//****************************** GzipConvert **********************************
// Purpose : Compresses an input file using the gzip format and writes
//           the compressed data to the specified output file.
// Inputs  : pInput  - path to the input file (const char*)
//           pOutput - path to the output file (const char*)
// Outputs : pOutput - Creates a gzip-compressed file at the given output path.
// Return  : true if the compression was successful, else false. 
// Notes   : Uses zlib's gzopen/gzwrite to perform compression.
//*****************************************************************************
bool GzipConvert(uint8_t* pucInput, uint8_t* pucOutput)
{
    bool blSuccess = false;
    gzFile pstOutput_file = NULL;

    if(pucInput == NULL || pucOutput == NULL) 
    {
        fprintf(stderr, "Error: Input and output file paths NULL.\n");
        return blSuccess;
    }

    FILE* pInput_file  = fopen((const char *)pucInput, FILE_MODE_READ_BINARY);
    pstOutput_file = gzopen((const char *)pucOutput, FILE_MODE_WRITE_BINARY);

    if (pInput_file == NULL) 
    { 
        fprintf(stderr, "Error opening input file.\n"); 
        return blSuccess; 
    }
 
    if (pstOutput_file == NULL) 
    { 
        fprintf(stderr, "Error opening output file for gzip compression\n");      
        fclose(pInput_file); 
        return blSuccess; 
    }

    blSuccess = GzipDataCompress(pInput_file, pstOutput_file);

    if(blSuccess == true) 
    {
        fclose(pInput_file);
        gzclose(pstOutput_file);
    }
    else
    {   
        fprintf(stderr, "Error: gzip compression failed.\n");
        fclose(pInput_file);
        gzclose(pstOutput_file);
    }

    return blSuccess;
}

//****************************** GzipDataCompress ********************************
// Purpose : Orchestrates the file compression process by managing file
//           resources and dispatching the data compression pump.
// Inputs  : pucInput  - Path to the source file to be compressed.
//           pucOutput - Path where the .gz file will be created.
// Outputs : pucOutput file is created with gzip-compressed data from pucInput.
// Return  : true if files were compressed successfully, else false.
// Notes   : None
//*****************************************************************************
static bool GzipDataCompress(FILE* pInputfile, gzFile pstOutputfile)
{
    bool blDataCompressionSuccess = false;
    uint8_t ucBuffer[GZIP_BUFFER_SIZE] = {0};
    uint32_t  ulBytesRead = 0;

    while(true)
    {
        ulBytesRead = fread(ucBuffer, BYTE_SIZE, sizeof(ucBuffer), pInputfile);

        if (ulBytesRead == 0) 
        {
            if (feof(pInputfile)) 
            {
                blDataCompressionSuccess = true;
                break; 
            } 
            else 
            {
                fprintf(stderr, "Error reading input file.\n");
                blDataCompressionSuccess = false;
                break;
            }
        }
    
        if (gzwrite(pstOutputfile, ucBuffer, ulBytesRead) != (int)ulBytesRead)
        {
            fprintf(stderr, "Error writing compressed data");
            blDataCompressionSuccess = false;
            break;
        }
    }
    return blDataCompressionSuccess; 
}
//EOF