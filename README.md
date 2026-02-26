# File_Utility_Task
This project is a command‑line file utility designed to perform different file conversions. It supports gzip , hexdump generation, and SREC conversion.

# Format
filename -t <-type> -i <-inputfilename> -o <-outputfilename>

-t <-type> → specifies the type of operation (gzip, hexdump, srec).

-i <-inputfilename> → path to the input file.

-o <-outputfilename> → path to the output file.

# Install Dependencies
- sudo apt-get update
- sudo apt-get install zlib1g-dev

# Build Instructions
make

