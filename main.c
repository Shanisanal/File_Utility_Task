#include <stdio.h>
#include <string.h>
#include "Include/gzip/gzip.h"

int main(int argc, char *argv[]) 
{
    char *type = NULL;
    char *input = NULL;
    char *output = NULL;

    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) 
        {
            type = argv[i + 1];
        } 
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) 
        {
            input = argv[i + 1];
        } 
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) 
        {
            output = argv[i + 1];
        }

        if (strcmp(type, "gzip") == 0) 
        { 
            gzip_file(input, output); 
        }
        
    }

    printf("Type: %s\n", type);
    printf("Input file: %s\n", input);
    printf("Output file: %s\n", output);

    return 0;
}
