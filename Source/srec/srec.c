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
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "srec/srec.h"
#include "Common/utility.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************
#define SREC_HEADER_RECORD_TYPE "S0"
#define SREC_HEADER_ADDR 0x0000
#define SREC_HEADER_ADDR_SIZE 2
#define PROJECT_NAME "SREC File Converter"

#define SREC_DATA_RECORD_TYPE "S3"
#define SREC_DATA_PER_LINE 16
#define SREC_DATA_RECORD_ADDR 0x08000000
#define SREC_DATA_RECORD_ADDR_SIZE 4

#define SREC_COUNT_RECORD_S5 "S5"
#define SREC_COUNT_RECORD_S6 "S6"
#define SREC_S5_RECORD_SIZE 2
#define SREC_S6_RECORD_SIZE 3
#define SREC_S5_MAX_COUNT 0xFFFF
#define SREC_S6_MAX_COUNT 0xFFFFFF

#define SREC_TERMINATION_RECORD "S7"

#define SREC_CHECKSUM_SIZE 1
#define BITS_SHIFT_8       8
#define BITS_SHIFT_16      16
#define BITS_SHIFT_24      24
#define BYTE_MASK          0xFF

//***************************** Local Variables *******************************/

//****************************** Local Functions ******************************/
static bool WriteSrecRecords(FILE* pInputFile, FILE* pOutputFile,
                             uint32_t* pulRecordCounter);

//************************** CalculateSrecChecksum ***************************
// Purpose : Calculates the Motorola S-record checksum for a single record.
// Inputs  : ucCount - The total number of bytes in the record (Address + Data +
//           Checksum).
//           ulAddr  - The memory address for the record.
//           pucData   - Pointer to the buffer containing the raw data bytes.
//           DataLen - The number of data bytes in the current record.
// Outputs : None.
// Return  : ucChecksum - The checksum value calculated.
// Notes   : None
//*****************************************************************************
uint8_t CalculateSrecChecksum(uint8_t ucCount, uint32_t ulAddr,uint8_t* pucData,
                              uint32_t ulDataLen) 
{
    uint32_t ulSum = ucCount;
    uint8_t ucChecksum = 0;

    ulSum += (ulAddr >> BITS_SHIFT_24) & BYTE_MASK;
    ulSum += (ulAddr >> BITS_SHIFT_16) & BYTE_MASK;
    ulSum += (ulAddr >> BITS_SHIFT_8) & BYTE_MASK;
    ulSum += (ulAddr & BYTE_MASK);

    for (uint32_t ulIndex = 0; ulIndex < ulDataLen; ulIndex++) 
    {
        ulSum += pucData[ulIndex];
    }
    ucChecksum = (uint8_t)(~ulSum & BYTE_MASK);

    return ucChecksum;
}
//****************************** SrecConvert ***********************************
// Purpose : Converts a binary input file into a Motorola S-record (SREC)
//           formatted text file .
// Inputs  : pucInput  - Path to the binary input file to be converted.
//           pucOutput - Path to the output text file where SREC data is saved.
// Outputs : pucOutput - The file is created with SREC formatted records
//           representing the binary data from the input file. 
// Return  : true if the SREC conversion was successful false otherwise. 
// Notes   : None
//*****************************************************************************
bool SrecConvert(uint8_t* pucInput, uint8_t* pucOutput) 
{
    bool blSuccess = true;
    uint32_t ulRecordCounter = 0;
    FILE* pInputFile = NULL;
    FILE* pOutputFile = NULL;

    if (pucInput == NULL || pucOutput == NULL) 
    {
        fprintf(stderr, "Error: Input and output file paths NULL.\n");
        blSuccess = false;
    }

    if(blSuccess == true)
    {
        pInputFile = fopen((const char*)pucInput, FILE_MODE_READ_BINARY);
        
        if (pInputFile == NULL) 
        { 
            fprintf(stderr, "Error opening input file.\n"); 
            blSuccess = false; 
        }
    }

    if(blSuccess == true)
    {
        pOutputFile = fopen((const char*)pucOutput, FILE_MODE_WRITE_TEXT);

        if (pOutputFile == NULL) 
        { 
            fprintf(stderr, "Error opening output file.\n"); 
            blSuccess = false; 
        }
    }

    /* SREC Record Header + Data Records + Count Records + Termination Record */
    if(blSuccess == true)
    {
        blSuccess = WriteSrecRecords(pInputFile, pOutputFile, &ulRecordCounter);

        if (blSuccess == false) 
        {
            fprintf(stderr, "Error writing SREC records.\n");
        }
    }   

    fclose(pInputFile);
    fclose(pOutputFile);;
    return blSuccess;
}

//************************** WriteSrecRecords ********************************
// Purpose : Writes all SREC records (Header, Data, Count, Termination).
// Inputs  : pInputFile     - Pointer to input binary file.
//           pOutputFile    - Pointer to output SREC file.
//           pulRecordCounter - Pointer to record counter.
// Outputs : pOutputFile - Updated with all SREC records.
// Return  : true if all records written successfully, false otherwise.
// Notes   : None
//*****************************************************************************
static bool WriteSrecRecords(FILE* pInputFile, FILE* pOutputFile,
                             uint32_t* pulRecordCnt)
{
    bool blSrecRecordSuccess = true;

    if(pInputFile == NULL || pOutputFile == NULL || pulRecordCnt == NULL) 
    {
        fprintf(stderr, "Error: Invalid parameters for SrecRecords\n");
        return blSrecRecordSuccess;
    }

    if(blSrecRecordSuccess == true)
    {
        blSrecRecordSuccess = WriteSrecHeaderRecord(pOutputFile);

        if (blSrecRecordSuccess == false) 
        {
            fprintf(stderr, "Error writing SREC header record\n");
        }
    }

    if(blSrecRecordSuccess == true)
    {
         blSrecRecordSuccess = WriteSrecDataRecord(pInputFile, pOutputFile,
                                                   pulRecordCnt);

        if (blSrecRecordSuccess == false) 
        {
            fprintf(stderr, "Error writing SREC data records\n");
        }
    }

    if(blSrecRecordSuccess == true)
    {
        blSrecRecordSuccess = WriteSrecCountRecord(pOutputFile, *pulRecordCnt);

        if (blSrecRecordSuccess == false) 
        {
            fprintf(stderr, "Error writing SREC count record\n");
        }
    }

    if(blSrecRecordSuccess == true) 
    {
        blSrecRecordSuccess = WriteSrecTerminationRecord(pOutputFile);

        if (blSrecRecordSuccess == false) 
        {
            fprintf(stderr, "Error writing SREC termination record\n");
        }   
    }

    return blSrecRecordSuccess;

}
//****************************** WriteSrecHeader ******************************
// Purpose : Formats and writes the Motorola S-Record S0 (Header) record
//           to the output file using the defined project name.
// Inputs  : pucOutputFile - Pointer to the destination file where the header
//           record is written.
// Outputs : pucOutputFile - The file is updated with the S0 header record
//           containing the project name. 
// Return  : true if the header was written successfully, false otherwise. 
// Notes   : None
//*****************************************************************************
bool WriteSrecHeaderRecord(FILE* pOutputFile) 
{
    bool blHeaderSuccess = true;
    const char* pProjectName = PROJECT_NAME;
    uint8_t ucHeaderChecksum = 0;
    uint8_t ucProjectNameLen = (uint8_t)strlen(pProjectName);
    uint8_t ucHederByteCount = 0;
    ucHederByteCount = ucProjectNameLen + SREC_CHECKSUM_SIZE + 
                                            SREC_HEADER_ADDR_SIZE;

    if(pOutputFile == NULL) 
    {
        fprintf(stderr, "Error: Invalid output file for SrecHeaderRecord\n");
        blHeaderSuccess =  false;
    }

    if(blHeaderSuccess == true) 
    {
        fprintf(pOutputFile, "%s%02X", SREC_HEADER_RECORD_TYPE, ucHederByteCount);
        fprintf(pOutputFile, "%04X", SREC_HEADER_ADDR);

        for (uint8_t ucIndex = 0; ucIndex < ucProjectNameLen; ucIndex++) 
        {
            fprintf(pOutputFile, "%02X", (uint8_t)pProjectName[ucIndex]);
        }

        ucHeaderChecksum = CalculateSrecChecksum ( ucHederByteCount, 0, 
                                (uint8_t*)pProjectName, ucProjectNameLen );

        fprintf(pOutputFile, "%02X\n", ucHeaderChecksum);
    }

    return blHeaderSuccess;
}

//****************************** WriteSrecDataRecord **************************
// Purpose : Reads data from a binary input file and formats it into Motorola
//           S-Record S3 data records.
// Inputs  : pInputFile     - Pointer to the source binary file.
//           pOutputFile    - Pointer to the destination text file.
//           pulRecordCounter - Pointer to store the total number of records
// Outputs : pucOutputFile - The file is updated with S3 records containing the
//           data from the input file.
//           pulRecordCounter - Updated with the total number of S3 records
//           written to the output file.
// Return  : true if all records were written successfully else false. 
// Notes   : Each record includes a type, byte count, address, hex-encoded data, 
//           and a checksum.
//*****************************************************************************
bool WriteSrecDataRecord(FILE* pInputFile, FILE* pOutputFile,
                         uint32_t* pulRecordCounter) 
{
    bool blDataRecordSuccess = true;
    uint8_t ucBuffer[SREC_DATA_PER_LINE] = {0};
    uint32_t ulBytesRead = 0;
    uint32_t ulAddress = SREC_DATA_RECORD_ADDR;
    uint32_t ulCounter = 0;
    uint8_t ucDataRecordByteCount = 0;
    uint8_t ucDataChecksum = 0;

    if (pInputFile == NULL || pOutputFile == NULL || pulRecordCounter == NULL) 
    {
        fprintf(stderr, "Invalid parameters for SrecDataRecord\n");
        blDataRecordSuccess = false;
    }

    if(blDataRecordSuccess == true) 
    {
        while (true) 
        {
            ulBytesRead = fread(ucBuffer, 1, sizeof(ucBuffer), pInputFile);

            if (ulBytesRead == 0) 
            {
                if (feof(pInputFile)) 
                {
                    break;
                } 
                else 
                {
                    perror("Error reading input file");
                    return blDataRecordSuccess;
                }
            }
            //Calculate ByteCount = Address(4 bytes) + Data  + Checksum(1 byte)
            ucDataRecordByteCount = (uint8_t)(SREC_DATA_RECORD_ADDR_SIZE +
                                            ulBytesRead + SREC_CHECKSUM_SIZE);
            ucDataChecksum = CalculateSrecChecksum(ucDataRecordByteCount, ulAddress,
                                                ucBuffer, ulBytesRead);

            // Format: S3 [ Byte Count] [4 byte Address] [Data] [Checksum]
            fprintf(pOutputFile, "%s%02X%08X", SREC_DATA_RECORD_TYPE,
                    ucDataRecordByteCount, ulAddress);

            for (uint32_t ulIndex = 0; ulIndex < ulBytesRead; ulIndex++) 
            {
                fprintf(pOutputFile, "%02X", ucBuffer[ulIndex]);
            }
            fprintf(pOutputFile, "%02X\n", ucDataChecksum);

            // Increment the address for the next line
            ulAddress += ulBytesRead;
            ulCounter++;
        }

    }

    *pulRecordCounter = ulCounter;
    return blDataRecordSuccess;
}

//****************************** WriteSrecCountRecord *************************
// Purpose : Formats and writes the Motorola S-Record count record (S5 or S6)
//           to the output file based on the total number of data records.
// Inputs  : pOutputFile  - Pointer to the destination SREC text file.
//           ulRecordCount- The total number of S3 records written.
// Outputs : pOutputFile - The file is updated with an S5 or S6 record
//                         containing the count of data records. 
// Return  : bool - true if the record was written successfully, false otherwise.
// Notes   : S5 is used for 16-bit counts (up to 65,535).
//           S6 is used for 24-bit counts (up to 16,777,215).
//*****************************************************************************
bool WriteSrecCountRecord(FILE* pOutputFile, uint32_t ulRecordCount) 
{
    bool blCountRecordSuccess = true ;
    uint8_t ucCountRecordByteCount = 0;
    uint8_t ucChecksum = 0;

    if (pOutputFile == NULL) 
    {
        fprintf(stderr, "Invalid output file for WriteSrecCountRecord\n");
        blCountRecordSuccess = false;
    }

    if(blCountRecordSuccess == true) 
    {
        if (ulRecordCount <= SREC_S5_MAX_COUNT) 
        {
            ucCountRecordByteCount = SREC_S5_RECORD_SIZE + SREC_CHECKSUM_SIZE;
            ucChecksum = CalculateSrecChecksum(ucCountRecordByteCount,
                                            ulRecordCount, NULL, 0);
            fprintf(pOutputFile, "%s%02X%04X%02X\n", SREC_COUNT_RECORD_S5,
                    ucCountRecordByteCount, (uint16_t)ulRecordCount, ucChecksum);
        } 
        else if (ulRecordCount <= SREC_S6_MAX_COUNT) 
        {
            ucCountRecordByteCount = SREC_S6_RECORD_SIZE + SREC_CHECKSUM_SIZE;
            ucChecksum = CalculateSrecChecksum(ucCountRecordByteCount,
                                            ulRecordCount, NULL, 0);
            fprintf(pOutputFile, "%s%02X%06X%02X\n", SREC_COUNT_RECORD_S6,
                    ucCountRecordByteCount, ulRecordCount, ucChecksum);
        } 
        else 
        {
            fprintf(stderr,
                    "Error: Record count exceeds maximum for S5/S6 records\n");
            blCountRecordSuccess = false;
        }
    }

    return blCountRecordSuccess;
}
//****************************** WriteSrecTerminationRecord *******************
// Purpose : Formats and writes the Motorola S-Record S7 (32-bit Address)
//           termination record to the output file. 
// Inputs  : pOutputFile - Pointer to the destination SREC text file. 
// Outputs : pOutputFile - The file is updated with an S7 termination record . 
// Return  : true if the record was written successfully, false otherwise. 
// Notes   : None
//*****************************************************************************
bool WriteSrecTerminationRecord(FILE* pOutputFile) 
{
    bool blTerminationSuccess = true;
    uint8_t ucTerminationByteCount = 0;
    uint8_t ucTerminationChecksum = 0;

    if(pOutputFile == NULL) 
    {
        fprintf(stderr, "Invalid output file for SrecTerminationRecord\n");
        blTerminationSuccess = false;
    }

    if (blTerminationSuccess == true) 
    {
        /* Byte Count: Address (4 bytes) + Checksum (1 byte) */
        ucTerminationByteCount =
            (uint8_t)(SREC_DATA_RECORD_ADDR_SIZE + SREC_CHECKSUM_SIZE);
        ucTerminationChecksum = CalculateSrecChecksum(
            ucTerminationByteCount, SREC_DATA_RECORD_ADDR, NULL, 0);

        fprintf(pOutputFile, "%s%02X%08X%02X\n", SREC_TERMINATION_RECORD,
                ucTerminationByteCount, SREC_DATA_RECORD_ADDR,
                ucTerminationChecksum);

    }

    return blTerminationSuccess;
}