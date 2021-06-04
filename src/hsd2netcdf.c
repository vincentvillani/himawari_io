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
    if(bib->data_p)
        free(bib->data_p);
    free(bib);
}



void read_basic_information_block(FILE* fp,
                                  BIB*  bib,
                                  bool  fill_data_p,
                                  uint32_t header_offset)
{
    bool     buffer_allocated = false;
    uint8_t* buffer           = NULL;

    // Read the block number/id and block size
    uint8_t  block_number = 0;
    uint16_t block_length = 0;
    fseek(fp,
          header_offset,
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
          header_offset,
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
    memcpy(&(bib->total_header_length),
           buffer + 70,
           4);
    memcpy(&(bib->total_data_length),
           buffer + 74,
           4);
    memcpy(&(bib->quality_flag_1),
           buffer + 78,
           1);
    memcpy(&(bib->quality_flag_2),
           buffer + 79,
           1);
    memcpy(&(bib->quality_flag_3),
           buffer + 80,
           1);
    memcpy(&(bib->quality_flag_4),
           buffer + 81,
           1);
    memcpy(&(bib->file_format_version),
           buffer + 82,
           32);
    memcpy(&(bib->filename),
           buffer + 114,
           32);
    memcpy(&(bib->spare),
           buffer + 146,
           40);

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
           "    Total header length    : %u\n"
           "    Total data length      : %u\n"
           "    Quality Flag 1         : %u\n"
           "    Quality Flag 2         : %u\n"
           "    Quality Flag 3         : %u\n"
           "    Quality Flag 4         : %u\n"
           "    File format version    : %s\n"
           "    Filename               : %s\n"
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
           bib->file_creation_time,
           bib->total_header_length,
           bib->total_data_length,
           bib->quality_flag_1,
           bib->quality_flag_2,
           bib->quality_flag_3,
           bib->quality_flag_4,
           bib->file_format_version,
           bib->filename);

}



DIB* allocate_data_information_block(bool allocate_data_p)
{
    DIB* result    = (DIB*)calloc(1, sizeof(DIB));
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



void deallocate_data_information_block(DIB* dib)
{
    if(dib->data_p)
        free(dib->data_p);
    free(dib);
}



void read_data_information_block(FILE* fp,
                                 DIB* dib,
                                 bool fill_data_p,
                                 uint32_t header_offset)
{
    bool     buffer_allocated = false;
    uint8_t* buffer           = NULL;

    // Read the block number/id and block size
    uint8_t  block_number = 0;
    uint16_t block_length = 0;
    fseek(fp,
          header_offset,
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
        buffer = dib->data_p;
    }
    else
    {
        buffer = (uint8_t*)calloc(1, block_length);
        buffer_allocated = true;
    }

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    dib->header_block_number = block_number;
    dib->block_length        = block_length;
    memcpy(&(dib->bits_per_pixel),
           buffer + 3,
           2);
    memcpy(&(dib->number_of_columns),
           buffer + 5,
           2);
    memcpy(&(dib->number_of_rows),
           buffer + 7,
           2);
    memcpy(&(dib->compression_flag),
           buffer + 9,
           1);
    memcpy(&(dib->spare),
           buffer + 10,
           40);

    if(buffer_allocated)
        free(buffer);
}



void print_data_information_block(DIB* dib)
{

    printf("Data Information Block:\n\n"
           "    Block number          : %u\n"
           "    Block length (bytes)  : %u\n"
           "    Bits per pixel        : %u\n"
           "    Number of columns (x) : %u\n"
           "    Number of rows    (y) : %u\n"
           "    Compression flag      : %u\n"
           "\n",
           dib->header_block_number,
           dib->block_length,
           dib->bits_per_pixel,
           dib->number_of_columns,
           dib->number_of_rows,
           dib->compression_flag);

}



PIB* allocate_projection_information_block(bool allocate_data_p)
{
    PIB* result    = (PIB*)calloc(1, sizeof(PIB));
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



void deallocate_projection_information_block(PIB* pib)
{
    if(pib->data_p)
        free(pib->data_p);
    free(pib);
}



void read_projection_information_block(FILE* fp, PIB* pib, bool fill_data_p, uint32_t header_offset)
{
    bool     buffer_allocated = false;
    uint8_t* buffer           = NULL;

    // Read the block number/id and block size
    uint8_t  block_number = 0;
    uint16_t block_length = 0;
    fseek(fp,
          header_offset,
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
        buffer = pib->data_p;
    }
    else
    {
        buffer = (uint8_t*)calloc(1, block_length);
        buffer_allocated = true;
    }

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    pib->header_block_number = block_number;
    pib->block_length        = block_length;
    memcpy(&(pib->sub_lon),
           buffer + 3,
           8);
    memcpy(&(pib->cfac),
           buffer + 11,
           4);
    memcpy(&(pib->lfac),
           buffer + 15,
           4);
    memcpy(&(pib->coff),
           buffer + 19,
           4);
    memcpy(&(pib->loff),
           buffer + 23,
           4);
    memcpy(&(pib->Rs),
           buffer + 27,
           8);
    memcpy(&(pib->Req),
           buffer + 35,
           8);
    memcpy(&(pib->Rpol),
           buffer + 43,
           8);
    memcpy(&(pib->R1),
           buffer + 51,
           8);
    memcpy(&(pib->R2),
           buffer + 59,
           8);
    memcpy(&(pib->R3),
           buffer + 67,
           8);
    memcpy(&(pib->Sd_coefficient),
           buffer + 75,
           8);
    memcpy(&(pib->resampling_types),
           buffer + 83,
           2);
    memcpy(&(pib->resampling_size),
           buffer + 85,
           2);
    memcpy(&(pib->spare),
           buffer + 87,
           40);

}



void print_projection_information_block(PIB* pib)
{

    printf("Projection Information Block:\n\n"
           "    Block number          : %u\n"
           "    Block length (bytes)  : %u\n"
           "    Sub longitude         : %f\n"
           "    Column scaling factor : %u\n"
           "    Line scaling factor   : %u\n"
           "    Column offset         : %f\n"
           "    Line offset           : %f\n"
           "    Rs                    : %f\n"
           "    Req                   : %f\n"
           "    Rpol                  : %f\n"
           "    R1:                   : %f\n"
           "    R2:                   : %f\n"
           "    R3:                   : %f\n"
           "    Sd coefficent:        : %f\n"
           "    Resampling types      : %u\n"
           "    Resampling sizes      : %u\n"
           "\n",
           pib->header_block_number,
           pib->block_length,
           pib->sub_lon,
           pib->cfac,
           pib->lfac,
           pib->coff,
           pib->loff,
           pib->Rs,
           pib->Req,
           pib->Rpol,
           pib->R1,
           pib->R2,
           pib->R3,
           pib->Sd_coefficient,
           pib->resampling_types,
           pib->resampling_size);


}



NIB* allocate_navigation_information_block(bool allocate_data_p)
{
    NIB* result    = (NIB*)calloc(1, sizeof(NIB));
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



void deallocate_navigation_information_block(NIB* nib)
{
    if(nib->data_p)
        free(nib->data_p);
    free(nib);
}



void read_navigation_information_block(FILE* fp, NIB* nib, bool fill_data_p, uint32_t header_offset)
{
    bool     buffer_allocated = false;
    uint8_t* buffer           = NULL;

    // Read the block number/id and block size
    uint8_t  block_number = 0;
    uint16_t block_length = 0;
    fseek(fp,
          header_offset,
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
        buffer = nib->data_p;
    }
    else
    {
        buffer = (uint8_t*)calloc(1, block_length);
        buffer_allocated = true;
    }

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    nib->header_block_number = block_number;
    nib->block_length        = block_length;
    memcpy(&(nib->navigation_information_time),
           buffer + 3,
           8);
    memcpy(&(nib->ssp_longitude),
           buffer + 11,
           8);
    memcpy(&(nib->ssp_latitude),
           buffer + 19,
           8);
    memcpy(&(nib->earth_center_to_satellite),
           buffer + 27,
           8);
    memcpy(&(nib->nadir_longitude),
           buffer + 35,
           8);
    memcpy(&(nib->nadir_latitude),
           buffer + 43,
           8);
    memcpy(&(nib->sun_position_x),
           buffer + 51,
           8);
    memcpy(&(nib->sun_position_y),
           buffer + 59,
           8);
    memcpy(&(nib->sun_position_z),
           buffer + 67,
           8);
    memcpy(&(nib->moon_position_x),
           buffer + 75,
           8);
    memcpy(&(nib->moon_position_y),
           buffer + 83,
           8);
    memcpy(&(nib->moon_position_z),
           buffer + 91,
           8);
    memcpy(&(nib->spare),
           buffer + 99,
           40);


}



void print_navigation_information_block(NIB* nib)
{
    printf("Projection Information Block:\n\n"
           "    Block number                   : %u\n"
           "    Block length (bytes)           : %u\n"
           "    Navigation information time    : %f\n"
           "    Sub-satellite point longitude  : %f\n"
           "    Sub-satellite point latitude   : %f\n"
           "    Earth center to satellite      : %f\n"
           "    Nadir longitude                : %f\n"
           "    Nadir latitude                 : %f\n"
           "    Sun's Position (x,y,z)(J2000)  : %f, %f, %f\n"
           "    Moon's Position (x,y,z)(J2000) : %f, %f, %f\n"
           "\n",
           nib->header_block_number,
           nib->block_length,
           nib->navigation_information_time,
           nib->ssp_longitude,
           nib->ssp_latitude,
           nib->earth_center_to_satellite,
           nib->nadir_longitude,
           nib->nadir_latitude,
           nib->sun_position_x,
           nib->sun_position_y,
           nib->sun_position_z,
           nib->moon_position_x,
           nib->moon_position_y,
           nib->moon_position_z);
}





HSD* allocate_hsd(bool allocate_data_p)
{
    HSD* result = (HSD*)calloc(1, sizeof(HSD));

    result->bib = allocate_basic_information_block(allocate_data_p);
    result->dib = allocate_data_information_block(allocate_data_p);
    result->pib = allocate_projection_information_block(allocate_data_p);
    result->nib = allocate_navigation_information_block(allocate_data_p);

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

    uint32_t block_offset = 0;

    read_basic_information_block(fp,
                                 hsd->bib,
                                 fill_data_p,
                                 block_offset);
    block_offset += hsd->bib->block_length;

    read_data_information_block(fp,
                                hsd->dib,
                                fill_data_p,
                                block_offset);
    block_offset += hsd->dib->block_length;

    read_projection_information_block(fp,
                                      hsd->pib,
                                      fill_data_p,
                                      block_offset);
    block_offset += hsd->pib->block_length;

    read_navigation_information_block(fp,
                                      hsd->nib,
                                      fill_data_p,
                                      block_offset);
    block_offset += hsd->nib->block_length;

    fclose(fp);
}



