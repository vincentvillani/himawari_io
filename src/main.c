#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <libgen.h>

#include "himawari_io.h"


// TODO: write bz2 support?

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
    HSD* hsd = himawari_io_read_file(input_filepath,
                                     true);
    if(hsd == NULL)
    {
        fprintf(stderr,
                "%s:%d: himawari_io_read_file returned NULL\n",
                __FILE__,
                __LINE__);
        exit(1);
    }

    // Print header information
    //himawari_io_print_header(hsd);


    // Get the path to the input and output files, without the bz2 extention
    const uint32_t buffer_length = 4096;
    char*          input_dir     = dirname(input_filepath); 
    char*          inpath        = (char*)malloc(buffer_length);
    char*          outpath       = (char*)malloc(buffer_length);
    snprintf(inpath,
             buffer_length,
             "%s/%s",
             input_dir,
             hsd->bib->filename);
    snprintf(outpath,
             buffer_length,
             "%s/%s",
             output_dir,
             hsd->bib->filename);


    // Write file back out
    himawari_io_write_file(outpath,
                           hsd);
    

    // Compare the input and output files
    int compare_result = himawari_io_compare_files(inpath,
                                                   outpath);
    if(compare_result != 0)
    {
        fprintf(stderr,
                "ERROR - %s:%d: compare_result returned error code %d\n",
                __FILE__,
                __LINE__,
                compare_result);
    }

    // Free memory
    free(inpath);
    free(outpath);
    himawari_io_free_hsd(hsd);

    return compare_result;
}


