#include "himawari_io.h"

#include <string.h>



BIB* allocate_basic_information_block()
{
    BIB* result = (BIB*)calloc(1,
                               sizeof(BIB));
    return result;
}



void deallocate_basic_information_block(BIB* bib)
{
    free(bib);
}



void read_basic_information_block(FILE* fp,
                                  BIB*  bib,
                                  uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

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



DIB* allocate_data_information_block()
{
    DIB* result = (DIB*)calloc(1,
                               sizeof(DIB));
    return result;
}



void deallocate_data_information_block(DIB* dib)
{
    free(dib);
}



void read_data_information_block(FILE*    fp,
                                 DIB*     dib,
                                 uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

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



PIB* allocate_projection_information_block()
{
    PIB* result = (PIB*)calloc(1,
                               sizeof(PIB));
    return result;
}



void deallocate_projection_information_block(PIB* pib)
{
    free(pib);
}



void read_projection_information_block(FILE*    fp,
                                       PIB*     pib,
                                       uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

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

    free(buffer);
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



NIB* allocate_navigation_information_block()
{
    NIB* result = (NIB*)calloc(1,
                               sizeof(NIB));
    return result;
}



void deallocate_navigation_information_block(NIB* nib)
{
    free(nib);
}



void read_navigation_information_block(FILE*    fp,
                                       NIB*     nib, 
                                       uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

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

    free(buffer);
}



void print_navigation_information_block(NIB* nib)
{
    printf("Navigation Information Block:\n\n"
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



CIB* allocate_calibration_information_block()
{
    CIB* result = (CIB*)calloc(1,
                               sizeof(CIB));
    return result;
}



void deallocate_calibration_information_block(CIB* cib)
{
    free(cib);
}



void read_calibration_information_block(FILE*    fp,
                                        CIB*     cib,
                                        uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    cib->header_block_number = block_number;
    cib->block_length        = block_length;
    memcpy(&(cib->band_number),
           buffer + 3,
           2);
    memcpy(&(cib->central_wave_length),
           buffer + 5,
           8);
    memcpy(&(cib->bits_per_pixel),
           buffer + 13,
           2);
    memcpy(&(cib->error_pixel_value),
           buffer + 15,
           2);
    memcpy(&(cib->outside_scan_pixel_value),
           buffer + 17,
           2);
    memcpy(&(cib->count_radiance_slope),
           buffer + 19,
           8);
    memcpy(&(cib->count_radiance_intercept),
           buffer + 27,
           8);

    // If this is an IR band
    if(cib->band_number >=7 )
    {
        memcpy(&(cib->ir_rad_to_bt_c0),
               buffer + 35,
               8);
        memcpy(&(cib->ir_rad_to_bt_c1),
               buffer + 43,
               8);
        memcpy(&(cib->ir_rad_to_bt_c2),
               buffer + 51,
               8);
        memcpy(&(cib->ir_bt_to_rad_c0),
               buffer + 59,
               8);
        memcpy(&(cib->ir_bt_to_rad_c1),
               buffer + 67,
               8);
        memcpy(&(cib->ir_bt_to_rad_c2),
               buffer + 75,
               8);
        memcpy(&(cib->spare),
               buffer + 83,
               40);
    }
    // Else this is a VIS/NIR band
    else
    {
        memcpy(&(cib->vis_nir_c_prime),
               buffer + 35,
               8);
        memcpy(&(cib->vis_nir_calib_update_time),
               buffer + 43,
               8);
        memcpy(&(cib->vis_nir_count_radiance_slope),
               buffer + 51,
               8);
        memcpy(&(cib->vis_nir_count_radiance_intercept),
               buffer + 59,
               8);
        memcpy(&(cib->spare),
               buffer + 67,
               80);
    }

    free(buffer);
}



void print_calibration_information_block(CIB* cib)
{
    if(cib->band_number >= 7)
    {
        printf("Calibration Information Block:\n\n"
               "    Block number                : %u\n"
               "    Block length (bytes)        : %u\n"
               "    band number                 : %u\n"
               "    Central wave length         : %f\n"
               "    Bits per pixel              : %u\n"
               "    Error pixel value           : %u\n"
               "    Outside scan pixel value    : %u\n"
               "    Count to radiance slope     : %f\n"
               "    Count to radiance intercept : %f\n"
               "    IR radiance to BT c0        : %f\n"
               "    IR radiance to BT c1        : %f\n"
               "    IR radiance to BT c2        : %f\n"
               "    IR BT to radiance c0        : %f\n"
               "    IR BT to radiance c1        : %f\n"
               "    IR BT to radiance c2        : %f\n"
               "\n",
               cib->header_block_number,
               cib->block_length,
               cib->band_number,
               cib->central_wave_length,
               cib->bits_per_pixel,
               cib->error_pixel_value,
               cib->outside_scan_pixel_value,
               cib->count_radiance_slope,
               cib->count_radiance_intercept,
               cib->ir_rad_to_bt_c0,
               cib->ir_rad_to_bt_c1,
               cib->ir_rad_to_bt_c2,
               cib->ir_bt_to_rad_c0,
               cib->ir_bt_to_rad_c1,
               cib->ir_bt_to_rad_c2);
    }
    else
    {
        printf("Calibration Information Block:\n\n"
               "    Block number                               : %u\n"
               "    Block length (bytes)                       : %u\n"
               "    band number                                : %u\n"
               "    Central wave length                        : %f\n"
               "    Bits per pixel                             : %u\n"
               "    Error pixel value                          : %u\n"
               "    Outside scan pixel value                   : %u\n"
               "    Count to radiance slope                    : %f\n"
               "    Count to radiance intercept                : %f\n"
               "    VIS/IR c'                                  : %f\n"
               "    VIS/IR calibration update time (mjd)       : %f\n"
               "    VIS/IR updated count to radiance slope     : %f\n"
               "    VIS/IR updated count to radiance intercept : %f\n"
               "\n",
               cib->header_block_number,
               cib->block_length,
               cib->band_number,
               cib->central_wave_length,
               cib->bits_per_pixel,
               cib->error_pixel_value,
               cib->outside_scan_pixel_value,
               cib->count_radiance_slope,
               cib->count_radiance_intercept,
               cib->vis_nir_c_prime,
               cib->vis_nir_calib_update_time,
               cib->vis_nir_count_radiance_slope,
               cib->vis_nir_count_radiance_intercept);
    }
}



IIB* allocate_inter_calibration_information_block()
{
    IIB* result = (IIB*)calloc(1,
                               sizeof(IIB));
    return result;
}



void deallocate_inter_calibration_information_block(IIB* iib)
{
    free(iib);
}



void read_inter_calibration_information_block(FILE*    fp,
                                              IIB*     iib,
                                              uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    iib->header_block_number = block_number;
    iib->block_length        = block_length;
    
    memcpy(&(iib->gsics_calibration_intercept),
           buffer + 3,
           8);
    memcpy(&(iib->gsics_calibration_slope),
           buffer + 11,
           8);
    memcpy(&(iib->gsics_calibration_quadratic),
           buffer + 19,
           8);
    memcpy(&(iib->radiance_bias),
           buffer + 27,
           8);
    memcpy(&(iib->radiance_bias_uncertainty),
           buffer + 35,
           8);
    memcpy(&(iib->radiance),
           buffer + 43,
           8);
    memcpy(&(iib->gsics_calibration_validity_start),
           buffer + 51,
           8);
    memcpy(&(iib->gsics_calibration_validity_end),
           buffer + 59,
           8);
    memcpy(&(iib->gsics_radiance_validity_upper_limit),
           buffer + 67,
           4);
    memcpy(&(iib->gsics_radiance_validity_lower_limit),
           buffer + 71,
           4);
    memcpy(&(iib->gsics_correction_filename),
           buffer + 75,
           128);
    memcpy(&(iib->spare),
           buffer + 203,
           56);

    free(buffer);
}



void print_inter_calibration_information_block(IIB* iib)
{
    printf("Inter-calibration Information Block:\n\n"
           "    Block number                                : %u\n"
           "    Block length (bytes)                        : %u\n"
           "    GSICS calibration intercept                 : %f\n"
           "    GSICS calibration slope                     : %f\n"
           "    GSICS calibration quadratic                 : %f\n"
           "    Radiance bias (standard scene)              : %f\n"
           "    Radiance bias uncertainty (standard scene)  : %f\n"
           "    Radiance (standard scene)                   : %f\n"
           "    GSICS calibration validity start time (mjd) : %f\n"
           "    GSICS calibration validity end time   (mjd) : %f\n"
           "    GSICS calibration validity upper limit      : %f\n"
           "    GSICS calibration validity lower limit      : %f\n"
           "    GSICS calibration correction filename       : %s\n"
           "\n",
           iib->header_block_number,
           iib->block_length,
           iib->gsics_calibration_intercept,
           iib->gsics_calibration_slope,
           iib->gsics_calibration_quadratic,
           iib->radiance_bias,
           iib->radiance_bias_uncertainty,
           iib->radiance,
           iib->gsics_calibration_validity_start,
           iib->gsics_calibration_validity_end,
           iib->gsics_radiance_validity_upper_limit,
           iib->gsics_radiance_validity_lower_limit,
           iib->gsics_correction_filename);
}



SIB* allocate_segment_information_block()
{
    SIB* result = (SIB*)calloc(1,
                               sizeof(SIB));
    return result;
}



void deallocate_segment_information_block(SIB* sib)
{
    free(sib);
}



void read_segment_information_block(FILE*    fp,
                                    SIB*     sib,
                                    uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    sib->header_block_number = block_number;
    sib->block_length        = block_length;
    memcpy(&(sib->total_segments),
           buffer + 3,
           1);
    memcpy(&(sib->segment_number),
           buffer + 4,
           1);
    memcpy(&(sib->segment_first_line_number),
           buffer + 5,
           2);
    memcpy(&(sib->spare),
           buffer + 7,
           40);

    free(buffer);
}



void print_segment_information_block(SIB* sib)
{
    printf("Segment Information Block:\n\n"
           "    Block number                 : %u\n"
           "    Block length (bytes)         : %u\n"
           "    Total segments               : %u\n"
           "    Segment number               : %u\n"
           "    First line number of segment : %u\n"
           "\n",
           sib->header_block_number,
           sib->block_length,
           sib->total_segments,
           sib->segment_number,
           sib->segment_first_line_number);
}



NCIB* allocate_navigation_correction_information_block()
{
    NCIB* result = (NCIB*)calloc(1,
                                 sizeof(NCIB));
    return result;
}



void deallocate_navigation_correction_information_block(NCIB* ncib)
{
    if(ncib->line_number_after_rotation)
        free(ncib->line_number_after_rotation);

    if(ncib->shift_for_column_direction)
        free(ncib->shift_for_column_direction);

    if(ncib->shift_for_line_direction)
        free(ncib->shift_for_line_direction);

    free(ncib);
}



void read_navigation_correction_information_block(FILE*    fp,
                                                  NCIB*    ncib,
                                                  uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    ncib->header_block_number = block_number;
    ncib->block_length        = block_length;
    memcpy(&(ncib->center_column_of_rotation),
           buffer + 3,
           4);
    memcpy(&(ncib->center_line_of_rotation),
           buffer + 7,
           4);
    memcpy(&(ncib->rotational_correction),
           buffer + 11,
           8);

    memcpy(&(ncib->number_of_corrections),
           buffer + 19,
           2);

    uint32_t buffer_offset = 21;

    // Allocate the required memory
    if(ncib->number_of_corrections > 0)
    {
        ncib->line_number_after_rotation = 
            (uint16_t*)malloc(sizeof(uint16_t) * ncib->number_of_corrections);
        ncib->shift_for_column_direction = 
            (float*)malloc(sizeof(float) * ncib->number_of_corrections);
        ncib->shift_for_line_direction = 
            (float*)malloc(sizeof(float) * ncib->number_of_corrections);
        
        for(uint32_t i = 0; i < ncib->number_of_corrections; ++i)
        {
            memcpy(ncib->line_number_after_rotation + i,
                   buffer + buffer_offset,
                   2);
            buffer_offset += 2;

            memcpy(ncib->shift_for_column_direction + i,
                   buffer + buffer_offset,
                   4);
            buffer_offset += 4;

            memcpy(ncib->shift_for_line_direction + i,
                   buffer + buffer_offset,
                   4);
            buffer_offset += 4;
        }
    }

    memcpy(&(ncib->spare),
          buffer + buffer_offset,
          40);

    free(buffer);
}



void print_navigation_correction_information_block(NCIB* ncib)
{
    const uint32_t buffer_length = 256 * ncib->number_of_corrections;
    char*          buffer        = (char*)malloc(sizeof(char) * buffer_length);
    uint32_t       written_chars = 0;

    for(uint32_t i = 0; i < ncib->number_of_corrections; ++i)
    {
        written_chars += snprintf(buffer + written_chars,
                                  buffer_length - written_chars,
                                  "      Correction %u:\n"
                                  "        Line number after rotation : %u\n"
                                  "        Shift for column direction : %f\n"
                                  "        Shift for line direction   : %f\n\n",
                                  i + 1,
                                  ncib->line_number_after_rotation[i],
                                  ncib->shift_for_column_direction[i],
                                  ncib->shift_for_line_direction[i]);
    }

    printf("Navigation Correction Information Block:\n\n"
           "    Block number                   : %u\n"
           "    Block length (bytes)           : %u\n"
           "    Center column of rotation      : %f\n"
           "    Center line of rotation        : %f\n"
           "    Rotational correction (urad)   : %f\n"
           "    Number of corrections          : %u\n\n"
           "%s\n"
           "\n",
           ncib->header_block_number,
           ncib->block_length,
           ncib->center_column_of_rotation,
           ncib->center_line_of_rotation,
           ncib->rotational_correction,
           ncib->number_of_corrections,
           buffer);

    free(buffer);
}



OTIB* allocate_observation_time_information_block()
{
    OTIB* result = (OTIB*)calloc(1,
                                 sizeof(OTIB));
    return result;
}



void deallocate_observation_time_information_block(OTIB* otib)
{
    if(otib->observation_time_line_number)
        free(otib->observation_time_line_number);

    if(otib->observation_time)
        free(otib->observation_time);

    free(otib);
}



void read_observation_time_information_block(FILE*    fp,
                                             OTIB*    otib,
                                             uint32_t header_offset)
{
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

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    otib->header_block_number = block_number;
    otib->block_length        = block_length;
    
    memcpy(&(otib->number_of_observation_times),
           buffer + 3,
           2);

    uint32_t buffer_offset = 5;
    if(otib->number_of_observation_times > 0)
    {
        otib->observation_time_line_number = 
            (uint16_t*)malloc(sizeof(uint16_t) * otib->number_of_observation_times);
        otib->observation_time = 
            (double*)malloc(sizeof(double) * otib->number_of_observation_times);

        for(uint32_t i = 0; i < otib->number_of_observation_times; ++i)
        {
            memcpy(otib->observation_time_line_number + i,
                   buffer + buffer_offset,
                   2);
            buffer_offset += 2;

            memcpy(otib->observation_time + i,
                   buffer + buffer_offset,
                   8);
            buffer_offset += 8;
        }
    }

    memcpy(&(otib->spare),
           buffer + buffer_offset,
           40);

    free(buffer);
}



void print_observation_time_information_block(OTIB* otib)
{
    const uint32_t buffer_length = 256 * otib->number_of_observation_times;
    char*          buffer        = (char*)malloc(sizeof(char) * buffer_length);
    uint32_t       written_chars = 0;

    for(uint32_t i = 0; i < otib->number_of_observation_times; ++i)
    {
        written_chars += snprintf(buffer + written_chars,
                                  buffer_length - written_chars,
                                  "      Observation time %u:\n"
                                  "        Line number              : %u\n"
                                  "        Observation time (mjd)   : %f\n\n",
                                  i + 1,
                                  otib->observation_time_line_number[i],
                                  otib->observation_time[i]);
    }

    printf("Observation Time Information Block:\n\n"
           "    Block number                 : %u\n"
           "    Block length (bytes)         : %u\n"
           "    Number of observation times  : %u\n\n"
           "%s\n"
           "\n",
           otib->header_block_number,
           otib->block_length,
           otib->number_of_observation_times,
           buffer);

    free(buffer);
}



EIB* allocate_error_information_block()
{
    EIB* result = (EIB*)calloc(1,
                               sizeof(EIB));
    return result;
}



void deallocate_error_information_block(EIB* eib)
{
    if(eib->error_line_number)
        free(eib->error_line_number);

    if(eib->error_pixels_for_line)
        free(eib->error_pixels_for_line);

    free(eib);
}



void read_error_information_block(FILE*    fp,
                                  EIB*     eib,
                                  uint32_t header_offset)
{
    // Read the block number/id and block size
    uint8_t  block_number = 0;
    uint32_t block_length = 0;
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(&block_number,
          sizeof(uint8_t),
          1,
          fp);
    fread(&block_length,
          sizeof(uint32_t),
          1,
          fp);

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    eib->header_block_number = block_number;
    eib->block_length        = block_length;

    memcpy(&(eib->number_of_error_information_data),
           buffer + 5,
           2);

    uint32_t buffer_offset = 7;
    if(eib->number_of_error_information_data > 0)
    {
        eib->error_line_number = 
            (uint16_t*)malloc(sizeof(uint16_t) * eib->number_of_error_information_data);
        eib->error_pixels_for_line = 
            (uint16_t*)malloc(sizeof(uint16_t) * eib->number_of_error_information_data);

        for(uint32_t i = 0; i < eib->number_of_error_information_data; ++i)
        {
            memcpy(eib->error_line_number + i,
                   buffer + buffer_offset,
                   2);
            buffer_offset += 2;

            memcpy(eib->error_pixels_for_line + i,
                   buffer + buffer_offset,
                   2);
            buffer_offset += 2;
        }
    }
    
    memcpy(&(eib->spare),
           buffer + buffer_offset,
           40);

    free(buffer);
}



void print_error_information_block(EIB* eib)
{
    const uint32_t buffer_length = 256 * eib->number_of_error_information_data;
    char*          buffer        = (char*)malloc(sizeof(char) * buffer_length);
    uint32_t       written_chars = 0;

    for(uint32_t i = 0; i < eib->number_of_error_information_data; ++i)
    {
        written_chars += snprintf(buffer + written_chars,
                                  buffer_length - written_chars,
                                  "      Error %u:\n"
                                  "        Line number                    : %u\n"
                                  "        Number of error pixels on line : %u\n",
                                  i + 1,
                                  eib->error_line_number[i],
                                  eib->error_pixels_for_line[i]);
    }

    printf("Error Information Block:\n\n"
           "    Block number                 : %u\n"
           "    Block length (bytes)         : %u\n"
           "    Number of error information  : %u\n"
           "%s\n"
           "\n",
           eib->header_block_number,
           eib->block_length,
           eib->number_of_error_information_data,
           buffer);

    free(buffer);
}



SB* allocate_spare_block()
{
    SB* result = (SB*)calloc(1,
                             sizeof(SB));
    return result;
}



void deallocate_spare_block(SB* sb)
{
    free(sb);
}



void read_spare_block(FILE*    fp,
                      SB*      sb,
                      uint32_t header_offset)
{
    // Read the block number/id and block size
    uint8_t  block_number = 0;
    uint32_t block_length = 0;
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(&block_number,
          sizeof(uint8_t),
          1,
          fp);
    fread(&block_length,
          sizeof(uint32_t),
          1,
          fp);

    // Allocate a buffer
    uint8_t* buffer = (uint8_t*)calloc(1,
                                       block_length);

    // Read in the whole block
    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(buffer,
          block_length,
          1,
          fp);

    sb->header_block_number = block_number;
    sb->block_length        = block_length;

    memcpy(&(sb->spare),
           buffer + 3,
           256);

    free(buffer);
}



void print_spare_block(SB* sb)
{
    printf("Spare Block:\n\n"
           "    Block number                 : %u\n"
           "    Block length (bytes)         : %u\n"
           "\n",
           sb->header_block_number,
           sb->block_length);
}



DB* allocate_data_block()
{
    DB* result = (DB*)calloc(1,
                             sizeof(DB));
    return result;
}



void deallocate_data_block(DB* db)
{
    if(db->data)
        free(db->data);
    free(db);
}



void read_data_block(FILE*    fp,
                     DB*      db,
                     uint32_t header_offset,
                     uint32_t length)
{
    db->data   = (uint16_t*)malloc(sizeof(uint16_t) * length);
    db->length = length;

    fseek(fp,
          header_offset,
          SEEK_SET);
    fread(db->data,
          sizeof(uint16_t) * db->length,
          1,
          fp);
}



void print_data_block(DB* db)
{
    printf("Data Block:\n\n"
           "    Block number                 : 12\n"
           "    Block length (bytes)         : %lu\n"
           "\n",
           sizeof(uint16_t) * db->length);
}



HSD* allocate_hsd()
{
    HSD* result = (HSD*)calloc(1,
                               sizeof(HSD));

    result->bib  = allocate_basic_information_block();
    result->dib  = allocate_data_information_block();
    result->pib  = allocate_projection_information_block();
    result->nib  = allocate_navigation_information_block();
    result->cib  = allocate_calibration_information_block();
    result->iib  = allocate_inter_calibration_information_block();
    result->sib  = allocate_segment_information_block();
    result->ncib = allocate_navigation_correction_information_block();
    result->otib = allocate_observation_time_information_block();
    result->eib  = allocate_error_information_block();
    result->sb   = allocate_spare_block();
    result->db   = allocate_data_block();

    return result;
}



void read_file(const char* filepath,
               HSD*        hsd)
{
    FILE* fp = fopen(filepath,
                     "r");
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
                                 block_offset);
    block_offset += hsd->bib->block_length;

    read_data_information_block(fp,
                                hsd->dib,
                                block_offset);
    block_offset += hsd->dib->block_length;

    read_projection_information_block(fp,
                                      hsd->pib,
                                      block_offset);
    block_offset += hsd->pib->block_length;

    read_navigation_information_block(fp,
                                      hsd->nib,
                                      block_offset);
    block_offset += hsd->nib->block_length;

    read_calibration_information_block(fp,
                                       hsd->cib,
                                       block_offset);
    block_offset += hsd->cib->block_length;

    read_inter_calibration_information_block(fp,
                                             hsd->iib,
                                             block_offset);
    block_offset += hsd->iib->block_length;
    
    read_segment_information_block(fp,
                                   hsd->sib,
                                   block_offset);
    block_offset += hsd->sib->block_length;

    read_navigation_correction_information_block(fp,
                                                 hsd->ncib,
                                                 block_offset);
    block_offset += hsd->ncib->block_length;

    read_observation_time_information_block(fp,
                                            hsd->otib,
                                            block_offset);
    block_offset += hsd->otib->block_length;
    
    read_error_information_block(fp,
                                 hsd->eib,
                                 block_offset);
    block_offset += hsd->eib->block_length;

    read_spare_block(fp,
                     hsd->sb,
                     block_offset);
    block_offset += hsd->sb->block_length;

    read_data_block(fp,
                    hsd->db,
                    block_offset,
                    hsd->dib->number_of_columns * hsd->dib->number_of_rows);

    fclose(fp);
}



void write_file(const char* filepath,
                HSD*        hsd)
{

}



void deallocate_hsd(HSD* hsd)
{
    deallocate_basic_information_block(hsd->bib);
    deallocate_data_information_block(hsd->dib);
    deallocate_projection_information_block(hsd->pib);
    deallocate_navigation_information_block(hsd->nib);
    deallocate_calibration_information_block(hsd->cib);
    deallocate_inter_calibration_information_block(hsd->iib);
    deallocate_segment_information_block(hsd->sib);
    deallocate_navigation_correction_information_block(hsd->ncib);
    deallocate_observation_time_information_block(hsd->otib);
    deallocate_error_information_block(hsd->eib);
    deallocate_spare_block(hsd->sb);
    deallocate_data_block(hsd->db);
    free(hsd);
}



void print_header(HSD* hsd)
{
    print_basic_information_block(hsd->bib);
    print_data_information_block(hsd->dib);
    print_projection_information_block(hsd->pib);
    print_navigation_information_block(hsd->nib);
    print_calibration_information_block(hsd->cib);
    print_inter_calibration_information_block(hsd->iib);
    print_segment_information_block(hsd->sib);
    print_navigation_correction_information_block(hsd->ncib);
    print_observation_time_information_block(hsd->otib);
    print_error_information_block(hsd->eib);
    print_spare_block(hsd->sb);
    print_data_block(hsd->db);
}



