#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <libgen.h>

#include "himawari_io.h"



// TODO:
// Test with each HSD version, both VIS/NIR and IR band
// Read in HSD, then write out that HSD again. Should be byte to byte identical

// TODO: Add arguments for reading header only
// TODO: Check if HSD has already been used? Then do something?
// TODO: bzip2 support

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        printf("Usage: %s <input filepath> <output directory>\n",
                argv[0]);
        exit(1);
    }

    char* input_filepath = argv[1];
    char* output_dir     = argv[2];
    char* input_file     = basename(input_filepath);

    // Read file
    HSD* hsd = read_file(input_filepath,
                         true);

    // Print header information
    //print_header(hsd);

    // Write file back out
    const uint32_t buffer_length = 4096;
    char*          buffer        = (char*)malloc(sizeof(char) * buffer_length);
    snprintf(buffer,
             buffer_length,
             "%s/%s",
             output_dir,
             input_file);

    write_file(buffer,
               hsd);
    

    int compare_result = compare_files(input_filepath,
                                       buffer);
                                       
    if(compare_result != 0)
    {
        fprintf(stderr,
                "ERROR - %s:%d: compare_result returned error code %d\n",
                __FILE__,
                __LINE__,
                compare_result);
    }

    // Deallocate
    free_hsd(hsd);
    free(buffer);

    return compare_result;
}


