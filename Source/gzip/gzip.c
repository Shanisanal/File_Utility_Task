#include <stdio.h> 
#include <zlib.h>



void gzip_file(const char *Input, const char *Output)
{
    FILE *Input_file = fopen(Input, "rb");
    if (!Input_file) 
    { 
        perror("Error opening input file"); 
        return; 
    }

    FILE *Output_file = gzopen(Output, "wb");
    if (!Output_file) 
    { 
        perror("Error opening output file"); 
        fclose(Input_file); 
        return; 
    }

    char buffer[4096];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), Input_file)) > 0) 
    {
        if (gzwrite(Output_file, buffer, bytes_read) != bytes_read) 
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