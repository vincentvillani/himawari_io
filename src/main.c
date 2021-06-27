#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <libgen.h>

#include "himawari_io.h"


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

    // Read file
    HSD* hsd = read_file(input_filepath,
                         true);

    // Print header information
    print_header(hsd);

    // Write file back out
    const uint32_t buffer_length = 4096;
    char*          buffer        = (char*)malloc(sizeof(char) * buffer_length);
    snprintf(buffer,
             buffer_length,
             "%s/%s",
             output_dir,
             hsd->bib->filename);

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
    //return 0;
}


