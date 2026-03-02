//**************************** File_Utility_Task ****************************** 
// Copyright (c) 2026 Trenser Technology Solutions
// All Rights Reserved
//***************************************************************************** 
// 
// File      : srec.c 
// Summary   : Implementation of srec functionality
// Note      : Provides functions declared in Include/srec/srec.h 
// Author    : Shani
// Date      : Feb 27, 2026 
// 
//***************************************************************************** 
 
//******************************* Include Files ******************************* 
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//******************************* Local Types ********************************* 
 
//***************************** Local Constants ******************************* 
#define SREC_DATA_PER_LINE 16
#define SREC_HEADER "S0"
#define SREC_HEADER_ADDR 0x0000
#define SREC_HEADER_ADDR_SIZE 2
#define SREC_HEADER_CHECKSUM_SIZE 1
#define PROJECT_NAME "SREC File Converter"

//***************************** Local Variables ******************************* 
 
//****************************** Local Functions ******************************/

//************************** Calculate_Srec_Checksum ***************************
// Purpose : Calculates the Motorola S-record checksum for a single line of data.
//           The checksum is the one's complement of the sum of the count,  address, and data bytes.
// Inputs  : ucCount - The total number of bytes in the record (Address + Data + Checksum).
//           ulAddr  - The memory address for the record.
//           pData   - Pointer to the buffer containing the raw data bytes.
//           DataLen - The number of data bytes in the current record.
// Outputs : Returns a single byte representing the calculated checksum.
// Return  : uint8_t - The 8-bit checksum value.
// Notes   : None
//*****************************************************************************
uint8_t Calculate_Srec_Checksum(uint8_t ucCount, uint32_t ulAddr, uint8_t *pData, size_t DataLen) 
{
    uint32_t ulSum = ucCount;
    
    ulSum += (ulAddr >> 24) & 0xFF;
    ulSum += (ulAddr >> 16) & 0xFF;
    ulSum += (ulAddr >> 8)  & 0xFF;
    ulSum += (ulAddr & 0xFF);


    for (size_t i = 0; i < DataLen; i++) 
    {
        ulSum += pData[i];
    }

    return (uint8_t)(~ulSum & 0xFF); 
}
//****************************** srec_file *************************************
// Purpose : Converts a binary input file into a Motorola S-record (SREC) 
//           formatted text file using S3 (32-bit address) data records.
// Inputs  : pInput  - Path to the binary input file to be converted.
//           pOutput - Path to the output text file where SREC data is saved.
// Outputs : Generates an SREC file containing a header (S0), data records (S3),
//           and a termination record (S7).
// Return  : None
// Notes   : Uses a fixed data length per line defined by SREC_DATA_PER_LINE.
//           Calculates a one's complement checksum for each record.
//*****************************************************************************
void srec_file(const char *pInput, const char *pOutput) 
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

    uint8_t ucBuffer[SREC_DATA_PER_LINE];
    size_t bytesRead;
    uint32_t ulAddress = 0;

    /******************************************************************************
    * SREC Record: S0 (Header) 
    *****************************************************************************/
    const char *pProjectName = PROJECT_NAME;
    uint8_t ucProjectNameLen = (uint8_t)strlen(pProjectName);

    // Calculate Byte Count: Address (2 byte ) + Data len + Checksum (1 byte)
    uint8_t ucHeaderByteCount= ucProjectNameLen + SREC_HEADER_ADDR_SIZE + SREC_HEADER_CHECKSUM_SIZE; 
    
    // Calculate Checksum for the S0 Record
    uint8_t ucHeaderChecksum = Calculate_Srec_Checksum(ucHeaderByteCount, 0, (uint8_t *)pProjectName, ucProjectNameLen);

    // Print Type (S0), ucHeaderByteCount, and Address 
    fprintf(pOutputFile, "%s%02X%04X", SREC_HEADER, ucHeaderByteCount, SREC_HEADER_ADDR);
 
    // Print Project Name as Hex ASCII
    for (uint8_t iIndex = 0; iIndex < ucProjectNameLen; iIndex++) 
    {
        fprintf(pOutputFile, "%02X", (uint8_t)pProjectName[iIndex]);
    }

    // Print the calculated SO record Checksum and Terminate Line
    fprintf(pOutputFile, "%02X\n", ucHeaderChecksum);

    // 2. Write Data Records (S3 for 32-bit addresses)
    while ((bytesRead = fread(ucBuffer, 1, sizeof(ucBuffer), pInputFile)) > 0) 
    {
        // Count = Address(4 bytes) + Data(N bytes) + Checksum(1 byte)
        uint8_t ucCount = (uint8_t)(4 + bytesRead + 1);
        uint8_t ucChecksum = Calculate_Srec_Checksum(ucCount, ulAddress, ucBuffer, bytesRead);

        // Format: S3 [Count] [Address] [Data] [Checksum]
        fprintf(pOutputFile, "S3%02X%08X", ucCount, ulAddress);
        for (size_t i = 0; i < bytesRead; i++) {
            fprintf(pOutputFile, "%02X", ucBuffer[i]);
        }
        fprintf(pOutputFile, "%02X\n", ucChecksum);

        ulAddress += bytesRead;
    }
    fprintf(pOutputFile, "S70500000000FA\n");

    fclose(pInputFile);
    fclose(pOutputFile);

}

