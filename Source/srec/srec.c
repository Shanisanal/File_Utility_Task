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
#define SREC_HEADER                 "S0"
#define SREC_HEADER_ADDR            0x0000
#define SREC_HEADER_ADDR_SIZE       2
#define PROJECT_NAME                "SREC File Converter"

#define SREC_DATA_RECORD_TYPE       "S3"
#define SREC_DATA_PER_LINE          16
#define SREC_DATA_RECORD_ADDR       0x08000000
#define SREC_DATA_RECORD_ADDR_SIZE  4

#define SREC_COUNT_RECORD_S5        "S5"
#define SREC_COUNT_RECORD_S6        "S6"
#define SREC_S5_RECORD_SIZE          2
#define SREC_S6_RECORD_SIZE          3

#define SREC_TERMINATION_RECORD     "S7"

#define SREC_CHECKSUM_SIZE           1

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

    /* SREC Record: S0 (Header)*/ 
    const char *pProjectName = PROJECT_NAME;
    uint8_t ucProjectNameLen = (uint8_t)strlen(pProjectName);

    // Calculate Byte Count: Address (2 byte ) + Data len + Checksum (1 byte)
    uint8_t ucHeaderByteCount= ucProjectNameLen + SREC_HEADER_ADDR_SIZE + SREC_CHECKSUM_SIZE; 
    
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

    /* SREC Data Record: S3 (32 bit addressing )*/ 
    uint8_t ucBuffer[SREC_DATA_PER_LINE] = {0};
    size_t bytesRead;
    uint32_t ulAddress = SREC_DATA_RECORD_ADDR;
    uint32_t ulRecordCounter = 0;

    while ((bytesRead = fread(ucBuffer, 1, sizeof(ucBuffer), pInputFile)) > 0) 
    {
        // Caculate DataRecordByteCount = Address(4 bytes) + Data(N bytes) + Checksum(1 byte)
        uint8_t ucDataRecordByteCount = (uint8_t)(SREC_DATA_RECORD_ADDR_SIZE + bytesRead + SREC_CHECKSUM_SIZE);
        
        // Calculate Checksum for the S3 Record
        uint8_t ucDataChecksum = Calculate_Srec_Checksum(ucDataRecordByteCount, ulAddress, ucBuffer, bytesRead);

        // Format: S3 [ Byte Count] [4 byte Address] [Data] [Checksum]
        fprintf(pOutputFile, "%s%02X%08X", SREC_DATA_RECORD_TYPE, ucDataRecordByteCount, ulAddress);
        for (size_t i = 0; i < bytesRead; i++) 
        {
            fprintf(pOutputFile, "%02X", ucBuffer[i]);
        }
        fprintf(pOutputFile, "%02X\n", ucDataChecksum);

        //Increment the address for the next line
        ulAddress += bytesRead;

        // Count this data record
        ulRecordCounter++;
    }

    /* SREC  Record: S5/S6      */ 
    uint8_t ucCountRecordByteCount = 0;
    if(ulRecordCounter <= 0xFFFF)
    {
        ucCountRecordByteCount = SREC_S5_RECORD_SIZE + SREC_CHECKSUM_SIZE;
        uint8_t ucS5Checksum = Calculate_Srec_Checksum(ucCountRecordByteCount, ulRecordCounter, NULL, 0);
        fprintf(pOutputFile, "%s%02X%04X%02X\n", SREC_COUNT_RECORD_S5, ucCountRecordByteCount, (uint16_t)ulRecordCounter, ucS5Checksum);

    }
    else if(ulRecordCounter <= 0xFFFFFF)
    {
        ucCountRecordByteCount = SREC_S6_RECORD_SIZE + SREC_CHECKSUM_SIZE;
        uint8_t ucS6Checksum = Calculate_Srec_Checksum(ucCountRecordByteCount, ulRecordCounter, NULL, 0);
        fprintf(pOutputFile, "%s%02X%06X%02X\n", SREC_COUNT_RECORD_S6, ucCountRecordByteCount, ulRecordCounter, ucS6Checksum);
    }

    /* SREC Termination Record (S7): 32-bit Entry Point */
    uint8_t ucTerminationByteCount = SREC_DATA_RECORD_ADDR_SIZE + SREC_CHECKSUM_SIZE;
    uint8_t ucTerminationChecksum = Calculate_Srec_Checksum(ucTerminationByteCount, SREC_DATA_RECORD_ADDR, NULL, 0);

    // Termination record [Type] [Byte Count] [Start Address] [Checksum]
    fprintf(pOutputFile, "%s%02X%08X%02X\n", SREC_TERMINATION_RECORD, ucTerminationByteCount, SREC_DATA_RECORD_ADDR, ucTerminationChecksum);

    fclose(pInputFile);
    fclose(pOutputFile);

}

