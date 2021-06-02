#include "hsd2netcdf.h"

#include <string.h>



BIB* allocate_basic_information_block(bool allocate_data_p)
{
    BIB* result    = (BIB*)calloc(1, sizeof(BIB));
    result->data_p = NULL;

    if(allocate_data_p)
    {
        // TODO
        fprintf(stderr,
                "%s:%s: Not currently supported\n",
                __FILE__,
                (char*)__LINE__);
        exit(1);
    }

    return result;
}



void deallocate_basic_information_block(BIB* bib)
{
    // TODO
}



void read_basic_information_block(FILE* fp,
                                  BIB*  bib,
                                  bool  fill_data_p)
{
    bool     buffer_allocated = false;
    uint8_t* buffer           = NULL;

    // Read the block number/id and block size
    uint8_t  block_number = 0;
    uint16_t block_length = 0;
    fseek(fp,
          0,
          SEEK_SET);
    fread(&block_number,
          sizeof(uint8_t),
          1,
          fp);
    fread(&block_length,
          sizeof(uint16_t),
          1,
          fp);

    // Do we need to allocate a buffer?
    if(fill_data_p)
    {
        buffer = bib->data_p;
    }
    else
    {
        buffer = (uint8_t*)calloc(1, block_length);
        buffer_allocated = true;
    }

    // Read in the whole block
    fseek(fp,
          0,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    bib->header_block_number = block_number;
    bib->block_length        = block_length;
    memcpy(&(bib->total_header_blocks),
           buffer + 3,
           2);
    memcpy(&(bib->byte_order),
           buffer + 5,
           1);
    memcpy(bib->satellite_name,
           buffer + 6,
           16);
    memcpy(bib->processing_center,
           buffer + 22,
           16);
    bib->observation_area[4] = '\0';
    memcpy(bib->observation_area,
           buffer + 38,
           4);
    bib->other_observation_info[2] = '\0';
    memcpy(bib->other_observation_info,
           buffer + 42,
           2);
    memcpy(&(bib->observation_timeline),
           buffer + 44,
           2);
    memcpy(&(bib->observation_start_time),
           buffer + 46,
           8);
    memcpy(&(bib->observation_end_time),
           buffer + 54,
           8);
    memcpy(&(bib->file_creation_time),
           buffer + 62,
           8);



    if(buffer_allocated)
        free(buffer);

}



void print_basic_information_block(BIB* bib)
{
    printf("Basic Information Block:\n\n"
           "    Block number           : %u\n"
           "    Block length (bytes)   : %u\n"
           "    Total header blocks    : %u\n"
           "    Byte order             : %u\n"
           "    Satellite name         : %s\n"
           "    Processing Center      : %s\n"
           "    Observation area       : %s\n"
           "    Other Observation info : %s\n"
           "    Observation timeline   : %.4u\n"
           "    Observation start time : %f (mjd)\n"
           "    Observation end time   : %f (mjd)\n"
           "    File creation time     : %f (mjd)\n"
           "\n",
           bib->header_block_number,
           bib->block_length,
           bib->total_header_blocks,
           bib->byte_order,
           bib->satellite_name,
           bib->processing_center,
           bib->observation_area,
           bib->other_observation_info,
           bib->observation_timeline,
           bib->observation_start_time,
           bib->observation_end_time,
           bib->file_creation_time
           );

}



HSD* allocate_hsd(bool allocate_data_p)
{
    HSD* result = (HSD*)calloc(1, sizeof(HSD));

    result->bib = allocate_basic_information_block(allocate_data_p);

    return result;
}



void read_file(const char* filepath, HSD* hsd, bool fill_data_p)
{
    FILE* fp = fopen(filepath, "r");
    if(fp == NULL)
    {
        fprintf(stderr,
                "%s:%s: Failed to open file %s\n",
                __FILE__,
                (char*)__LINE__,
                filepath);
        exit(1);
    }

    
    read_basic_information_block(fp,
                                 hsd->bib,
                                 fill_data_p);
    

    fclose(fp);
}



