#include "himawari_io.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/param.h>

#include <bzlib.h>


// Private function declarations
// ----------------------------------------

uint8_t* _decompress_bz2_file(FILE*     fp,
                              uint64_t* decompress_size);

HSD*  _allocate_hsd();
BIB*  _allocate_basic_information_block();
DIB*  _allocate_data_information_block();
PIB*  _allocate_projection_information_block();
NIB*  _allocate_navigation_information_block();
CIB*  _allocate_calibration_information_block();
IIB*  _allocate_inter_calibration_information_block();
SIB*  _allocate_segment_information_block();
NCIB* _allocate_navigation_correction_information_block();
OTIB* _allocate_observation_time_information_block();
EIB*  _allocate_error_information_block();
SB*   _allocate_spare_block();
DB*   _allocate_data_block();

void _deallocate_basic_information_block(BIB* bib);
void _deallocate_data_information_block(DIB* dib);
void _deallocate_projection_information_block(PIB* pib);
void _deallocate_navigation_information_block(NIB* nib);
void _deallocate_calibration_information_block(CIB* cib);
void _deallocate_inter_calibration_information_block(IIB* iib);
void _deallocate_segment_information_block(SIB* sib);
void _deallocate_navigation_correction_information_block(NCIB* ncib);
void _deallocate_observation_time_information_block(OTIB* otib);
void _deallocate_error_information_block(EIB* eib);
void _deallocate_spare_block(SB* sb);
void _deallocate_data_block(DB* db);

void _read_basic_information_block(FILE*    fp,
                                   BIB*     bib,
                                   uint32_t header_offset);
void _read_data_information_block(FILE*    fp,
                                  DIB*     dib,
                                  uint32_t header_offset);
void _read_projection_information_block(FILE*    fp,
                                       PIB*     pib,
                                       uint32_t header_offset);
void _read_navigation_information_block(FILE*    fp,
                                        NIB*     nib,
                                        uint32_t header_offset);
void _read_calibration_information_block(FILE*    fp,
                                         CIB*     cib,
                                         uint32_t header_offset);
void _read_inter_calibration_information_block(FILE*    fp,
                                               IIB*     iib,
                                               uint32_t header_offset);
void _read_segment_information_block(FILE*    fp,
                                     SIB*     sib,
                                     uint32_t header_offset);
void _read_navigation_correction_information_block(FILE*    fp,
                                                   NCIB*    ncib,
                                                   uint32_t header_offset);
void _read_observation_time_information_block(FILE*    fp,
                                              OTIB*    otib,
                                              uint32_t header_offset);
void _read_error_information_block(FILE*    fp,
                                   EIB*     eib,
                                   uint32_t header_offset);
void _read_spare_block(FILE*    fp,
                       SB*      sb,
                       uint32_t header_offset);
void _read_data_block(FILE*    fp,
                      DB*      db,
                      uint32_t header_offset,
                      uint32_t length);

void _write_basic_information_block(FILE*    fp,
                                    BIB*     bib,
                                    uint32_t header_offset);
void _write_data_information_block(FILE*    fp,
                                   DIB*     dib,
                                   uint32_t header_offset);
void _write_projection_information_block(FILE*    fp,
                                         PIB*     pib,
                                         uint32_t header_offset);
void _write_navigation_information_block(FILE*    fp,
                                         NIB*     nib,
                                         uint32_t header_offset);
void _write_calibration_information_block(FILE*    fp,
                                          CIB*     cib,
                                          uint32_t header_offset);
void _write_inter_calibration_information_block(FILE*    fp,
                                                IIB*     iib,
                                                uint32_t header_offset);
void _write_segment_information_block(FILE*    fp,
                                      SIB*     sib,
                                      uint32_t header_offset);
void _write_navigation_correction_information_block(FILE*    fp,
                                                    NCIB*    ncib,
                                                    uint32_t header_offset);
void _write_observation_time_information_block(FILE*    fp,
                                               OTIB*    otib,
                                               uint32_t header_offset);
void _write_error_information_block(FILE*    fp,
                                    EIB*     eib,
                                    uint32_t header_offset);
void _write_spare_block(FILE*    fp,
                        SB*      sb,
                        uint32_t header_offset);
void _write_data_block(FILE*    fp,
                       DB*      db,
                       uint32_t header_offset);

void _print_basic_information_block(BIB* bib);
void _print_data_information_block(DIB* dib);
void _print_projection_information_block(PIB* pib);
void _print_navigation_information_block(NIB* nib);
void _print_calibration_information_block(CIB* cib);
void _print_inter_calibration_information_block(IIB* iib);
void _print_segment_information_block(SIB* sib);
void _print_navigation_correction_information_block(NCIB* ncib);
void _print_observation_time_information_block(OTIB* otib);
void _print_error_information_block(EIB* eib);
void _print_spare_block(SB* sb);
void _print_data_block(DB* db);



// Private function implementations
// ----------------------------------------



uint8_t* _decompress_bz2_file(FILE*     fp,
                              uint64_t* decompress_size)
{
    // Prepare bz2 file for reading
    int bz2_error;
    BZFILE* bz_fp = BZ2_bzReadOpen(&bz2_error,
                                   fp,
                                   0,
                                   0,
                                   NULL,
                                   0);
    if(bz2_error != BZ_OK)
    {
        fprintf(stderr,
                "%s:%d: BZ2_bzReadOpen returned error code %d\n",
                __FILE__,
                __LINE__,
                bz2_error);
        exit(1);
    }

    // Read and decompress bz2 file
    uint64_t bytes_read               = 0;
    uint64_t decompress_buffer_length = 1000 * 1000 * 100;
    uint8_t* decompress_buffer        = (uint8_t*)malloc(decompress_buffer_length);
    while(bz2_error == BZ_OK)
    {
        const int current_bytes_read = BZ2_bzRead(&bz2_error,
                                                  bz_fp,
                                                  decompress_buffer + bytes_read,
                                                  decompress_buffer_length - bytes_read);
        bytes_read += current_bytes_read;

        // Grow the decompress_buffer if required
        if(bytes_read == decompress_buffer_length)
        {
            uint8_t* grow_buffer = (uint8_t*)malloc(decompress_buffer_length + decompress_buffer_length);
            memcpy(grow_buffer,
                   decompress_buffer,
                   decompress_buffer_length);
            free(decompress_buffer);
            decompress_buffer = grow_buffer;
            decompress_buffer_length += decompress_buffer_length;
        }
    }
    if(bz2_error != BZ_STREAM_END)
    {
        fprintf(stderr,
                "%s:%d: BZ2_bzRead returned error code %d\n",
                __FILE__,
                __LINE__,
                bz2_error);
        exit(1);
    }

    // Close the bz2 file
    BZ2_bzReadClose(&bz2_error,
                    bz_fp);
    if(bz2_error != BZ_OK)
    {
        fprintf(stderr,
                "%s:%d: bzReadClose returned error code %d\n",
                __FILE__,
                __LINE__,
                bz2_error);
        exit(1);
    }

    *decompress_size = bytes_read;
    return decompress_buffer;

}



BIB* _allocate_basic_information_block()
{
    BIB* result = (BIB*)calloc(1,
                               sizeof(BIB));
    return result;
}



DIB* _allocate_data_information_block()
{
    DIB* result = (DIB*)calloc(1,
                               sizeof(DIB));
    return result;
}



PIB* _allocate_projection_information_block()
{
    PIB* result = (PIB*)calloc(1,
                               sizeof(PIB));
    return result;
}



NIB* _allocate_navigation_information_block()
{
    NIB* result = (NIB*)calloc(1,
                               sizeof(NIB));
    return result;
}



CIB* _allocate_calibration_information_block()
{
    CIB* result = (CIB*)calloc(1,
                               sizeof(CIB));
    return result;
}



IIB* _allocate_inter_calibration_information_block()
{
    IIB* result = (IIB*)calloc(1,
                               sizeof(IIB));
    return result;
}



SIB* _allocate_segment_information_block()
{
    SIB* result = (SIB*)calloc(1,
                               sizeof(SIB));
    return result;
}



NCIB* _allocate_navigation_correction_information_block()
{
    NCIB* result = (NCIB*)calloc(1,
                                 sizeof(NCIB));
    return result;
}



OTIB* _allocate_observation_time_information_block()
{
    OTIB* result = (OTIB*)calloc(1,
                                 sizeof(OTIB));
    return result;
}



EIB* _allocate_error_information_block()
{
    EIB* result = (EIB*)calloc(1,
                               sizeof(EIB));
    return result;
}



SB* _allocate_spare_block()
{
    SB* result = (SB*)calloc(1,
                             sizeof(SB));
    return result;
}



DB* _allocate_data_block()
{
    DB* result = (DB*)calloc(1,
                             sizeof(DB));
    return result;
}



HSD* _allocate_hsd()
{
    HSD* result = (HSD*)calloc(1,
                               sizeof(HSD));

    result->bib  = _allocate_basic_information_block();
    result->dib  = _allocate_data_information_block();
    result->pib  = _allocate_projection_information_block();
    result->nib  = _allocate_navigation_information_block();
    result->cib  = _allocate_calibration_information_block();
    result->iib  = _allocate_inter_calibration_information_block();
    result->sib  = _allocate_segment_information_block();
    result->ncib = _allocate_navigation_correction_information_block();
    result->otib = _allocate_observation_time_information_block();
    result->eib  = _allocate_error_information_block();
    result->sb   = _allocate_spare_block();
    result->db   = _allocate_data_block();

    return result;
}



void _deallocate_basic_information_block(BIB* bib)
{
    if(bib->spare)
        free(bib->spare);
    free(bib);
}



void _deallocate_data_information_block(DIB* dib)
{
    if(dib->spare)
        free(dib->spare);
    free(dib);
}



void _deallocate_projection_information_block(PIB* pib)
{
    if(pib->spare)
        free(pib->spare);
    free(pib);
}



void _deallocate_navigation_information_block(NIB* nib)
{
    if(nib->spare)
        free(nib->spare);
    free(nib);
}



void _deallocate_calibration_information_block(CIB* cib)
{
    if(cib->spare)
        free(cib->spare);
    free(cib);
}



void _deallocate_inter_calibration_information_block(IIB* iib)
{
    if(iib->spare)
        free(iib->spare);
    free(iib);
}



void _deallocate_segment_information_block(SIB* sib)
{
    if(sib->spare)
        free(sib->spare);
    free(sib);
}



void _deallocate_navigation_correction_information_block(NCIB* ncib)
{
    if(ncib->line_number_after_rotation)
        free(ncib->line_number_after_rotation);

    if(ncib->shift_for_column_direction)
        free(ncib->shift_for_column_direction);

    if(ncib->shift_for_line_direction)
        free(ncib->shift_for_line_direction);

    if(ncib->spare)
        free(ncib->spare);

    free(ncib);
}



void _deallocate_observation_time_information_block(OTIB* otib)
{
    if(otib->observation_time_line_number)
        free(otib->observation_time_line_number);

    if(otib->observation_time)
        free(otib->observation_time);

    if(otib->spare)
        free(otib->spare);

    free(otib);
}



void _deallocate_error_information_block(EIB* eib)
{
    if(eib->error_line_number)
        free(eib->error_line_number);

    if(eib->error_pixels_for_line)
        free(eib->error_pixels_for_line);

    if(eib->spare)
        free(eib->spare);

    free(eib);
}



void _deallocate_spare_block(SB* sb)
{
    if(sb->spare)
        free(sb->spare);
    free(sb);
}



void _deallocate_data_block(DB* db)
{
    if(db->data)
        free(db->data);
    free(db);
}



void _read_basic_information_block(FILE*    fp,
                                   BIB*     bib,
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

    uint32_t buffer_offset = 3;
    memcpy(&(bib->total_header_blocks),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(bib->byte_order),
           buffer + buffer_offset,
           1);
    buffer_offset += 1;

    memcpy(bib->satellite_name,
           buffer + buffer_offset,
           16);
    buffer_offset += 16;

    memcpy(bib->processing_center,
           buffer + buffer_offset,
           16);
    buffer_offset += 16;

    bib->observation_area[4] = '\0';
    memcpy(bib->observation_area,
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    bib->other_observation_info[2] = '\0';
    memcpy(bib->other_observation_info,
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(bib->observation_timeline),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(bib->observation_start_time),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(bib->observation_end_time),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(bib->file_creation_time),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(bib->total_header_length),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(bib->total_data_length),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(bib->quality_flag_1),
           buffer + buffer_offset,
           1);
    buffer_offset += 1;

    memcpy(&(bib->quality_flag_2),
           buffer + buffer_offset,
           1);
    buffer_offset += 1;

    memcpy(&(bib->quality_flag_3),
           buffer + buffer_offset,
           1);
    buffer_offset += 1;

    memcpy(&(bib->quality_flag_4),
           buffer + buffer_offset,
           1);
    buffer_offset += 1;

    memcpy(&(bib->file_format_version),
           buffer + buffer_offset,
           32);
    buffer_offset += 32;

    memcpy(&(bib->filename),
           buffer + buffer_offset,
           128);
    buffer_offset += 128;

    bib->spare_length = bib->block_length - buffer_offset;
    bib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * bib->spare_length);
    memcpy(bib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * bib->spare_length);

    free(buffer);
}



void _read_data_information_block(FILE*    fp,
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

    uint32_t buffer_offset = 3;
    memcpy(&(dib->bits_per_pixel),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(dib->number_of_columns),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(dib->number_of_rows),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(dib->compression_flag),
           buffer + buffer_offset,
           1);
    buffer_offset += 1;

    dib->spare_length = dib->block_length - buffer_offset;
    dib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * dib->spare_length);
    memcpy(dib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * dib->spare_length);

    free(buffer);
}



void _read_projection_information_block(FILE*    fp,
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

    uint32_t buffer_offset = 3;
    memcpy(&(pib->sub_lon),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(pib->cfac),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(pib->lfac),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(pib->coff),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(pib->loff),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(pib->Rs),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(pib->Req),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(pib->Rpol),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(pib->R1),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(pib->R2),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(pib->R3),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(pib->Sd_coefficient),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(pib->resampling_types),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(pib->resampling_size),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    pib->spare_length = pib->block_length - buffer_offset;
    pib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * pib->spare_length);
    memcpy(pib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * pib->spare_length);

    free(buffer);
}



void _read_navigation_information_block(FILE*    fp,
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

    uint32_t buffer_offset = 3;
    memcpy(&(nib->navigation_information_time),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->ssp_longitude),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->ssp_latitude),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->earth_center_to_satellite),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->nadir_longitude),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->nadir_latitude),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->sun_position_x),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->sun_position_y),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->sun_position_z),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->moon_position_x),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->moon_position_y),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(nib->moon_position_z),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    nib->spare_length = nib->block_length - buffer_offset;
    nib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * nib->spare_length);
    memcpy(nib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * nib->spare_length);

    free(buffer);
}



void _read_calibration_information_block(FILE*    fp,
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

    uint32_t buffer_offset = 3;
    memcpy(&(cib->band_number),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(cib->central_wave_length),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(cib->bits_per_pixel),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(cib->error_pixel_value),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(cib->outside_scan_pixel_value),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    memcpy(&(cib->count_radiance_slope),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(cib->count_radiance_intercept),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    // If this is an IR band
    if(cib->band_number >=7 )
    {
        memcpy(&(cib->ir_rad_to_bt_c0),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->ir_rad_to_bt_c1),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->ir_rad_to_bt_c2),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->ir_bt_to_rad_c0),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->ir_bt_to_rad_c1),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->ir_bt_to_rad_c2),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->ir_c),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;
        
        memcpy(&(cib->ir_h),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->ir_k),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;
    }
    // Else this is a VIS/NIR band
    else
    {
        memcpy(&(cib->vis_nir_c_prime),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->vis_nir_calib_update_time),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->vis_nir_count_radiance_slope),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;

        memcpy(&(cib->vis_nir_count_radiance_intercept),
               buffer + buffer_offset,
               8);
        buffer_offset += 8;
    }

    cib->spare_length = cib->block_length - buffer_offset;
    cib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * cib->spare_length);
    memcpy(cib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * cib->spare_length);

    free(buffer);
}



void _read_inter_calibration_information_block(FILE*    fp,
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
    
    uint32_t buffer_offset = 3;
    memcpy(&(iib->gsics_calibration_intercept),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(iib->gsics_calibration_slope),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(iib->gsics_calibration_quadratic),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(iib->radiance_bias),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(iib->radiance_bias_uncertainty),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(iib->radiance),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(iib->gsics_calibration_validity_start),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(iib->gsics_calibration_validity_end),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(iib->gsics_radiance_validity_upper_limit),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(iib->gsics_radiance_validity_lower_limit),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(iib->gsics_correction_filename),
           buffer + buffer_offset,
           128);
    buffer_offset += 128;

    iib->spare_length = iib->block_length - buffer_offset;
    iib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * iib->spare_length);
    memcpy(iib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * iib->spare_length);

    free(buffer);
}



void _read_segment_information_block(FILE*    fp,
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

    uint32_t buffer_offset = 3;
    memcpy(&(sib->total_segments),
           buffer + buffer_offset,
           1);
    buffer_offset += 1;

    memcpy(&(sib->segment_number),
           buffer + buffer_offset,
           1);
    buffer_offset += 1;

    memcpy(&(sib->segment_first_line_number),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

    sib->spare_length = sib->block_length - buffer_offset;
    sib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * sib->spare_length);
    memcpy(sib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * sib->spare_length);

    free(buffer);
}



void _read_navigation_correction_information_block(FILE*    fp,
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

    uint32_t buffer_offset = 3;
    memcpy(&(ncib->center_column_of_rotation),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(ncib->center_line_of_rotation),
           buffer + buffer_offset,
           4);
    buffer_offset += 4;

    memcpy(&(ncib->rotational_correction),
           buffer + buffer_offset,
           8);
    buffer_offset += 8;

    memcpy(&(ncib->number_of_corrections),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

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

    ncib->spare_length = ncib->block_length - buffer_offset;
    ncib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * ncib->spare_length);
    memcpy(ncib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * ncib->spare_length);

    free(buffer);
}



void _read_observation_time_information_block(FILE*    fp,
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
    
    uint32_t buffer_offset = 3;

    memcpy(&(otib->number_of_observation_times),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

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

    otib->spare_length = otib->block_length - buffer_offset;
    otib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * otib->spare_length);
    memcpy(otib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * otib->spare_length);

    free(buffer);
}



void _read_error_information_block(FILE*    fp,
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

    uint32_t buffer_offset = 5;
    memcpy(&(eib->number_of_error_information_data),
           buffer + buffer_offset,
           2);
    buffer_offset += 2;

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
 
    eib->spare_length = eib->block_length - buffer_offset;
    eib->spare        = (uint8_t*)malloc(sizeof(uint8_t) * eib->spare_length);
    memcpy(eib->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * eib->spare_length);

    free(buffer);
}



void _read_spare_block(FILE*    fp,
                       SB*      sb,
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

    sb->header_block_number = block_number;
    sb->block_length        = block_length;

    uint32_t buffer_offset = 3;
    sb->spare_length       = sb->block_length - buffer_offset;
    sb->spare              = (uint8_t*)malloc(sizeof(uint8_t) * sb->spare_length);
    memcpy(sb->spare,
           buffer + buffer_offset,
           sizeof(uint8_t) * sb->spare_length);

    free(buffer);
}



void _read_data_block(FILE*    fp,
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



void _write_basic_information_block(FILE*    fp,
                                    BIB*     bib,
                                    uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * bib->block_length);

    uint32_t buffer_offset = 0;

    memcpy(buffer + buffer_offset,
           &(bib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(bib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(bib->total_header_blocks),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(bib->byte_order),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(bib->satellite_name),
           16);
    buffer_offset += 16;

    memcpy(buffer + buffer_offset,
           &(bib->processing_center),
           16);
    buffer_offset += 16;

    // Note: 5 bytes allocated here, but only 4 in the actual file
    //       I.E. the file doesn't have enough space for the null char
    memcpy(buffer + buffer_offset,
           &(bib->observation_area),
           4);
    buffer_offset += 4;

    // Note: 3 bytes allocated here, but only 2 in the actual file
    //       I.E. the file doesn't have enough space for the null char
    memcpy(buffer + buffer_offset,
           &(bib->other_observation_info),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(bib->observation_timeline),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(bib->observation_start_time),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(bib->observation_end_time),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(bib->file_creation_time),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(bib->total_header_length),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(bib->total_data_length),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(bib->quality_flag_1),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(bib->quality_flag_2),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(bib->quality_flag_3),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(bib->quality_flag_4),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(bib->file_format_version),
           32);
    buffer_offset += 32;

    memcpy(buffer + buffer_offset,
           &(bib->filename),
           128);
    buffer_offset += 128;

    memcpy(buffer + buffer_offset,
           bib->spare,
           bib->spare_length);

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * bib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_data_information_block(FILE*    fp,
                                   DIB*     dib,
                                   uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * dib->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(dib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(dib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(dib->bits_per_pixel),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(dib->number_of_columns),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(dib->number_of_rows),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(dib->compression_flag),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           dib->spare,
           dib->spare_length);
    buffer_offset += dib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * dib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_projection_information_block(FILE*    fp,
                                         PIB*     pib,
                                         uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * pib->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(pib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(pib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(pib->sub_lon),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(pib->cfac),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(pib->lfac),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(pib->coff),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(pib->loff),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(pib->Rs),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(pib->Req),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(pib->Rpol),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(pib->R1),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(pib->R2),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(pib->R3),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(pib->Sd_coefficient),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(pib->resampling_types),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(pib->resampling_size),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           pib->spare,
           pib->spare_length);
    buffer_offset += pib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * pib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_navigation_information_block(FILE*    fp,
                                         NIB*     nib,
                                         uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * nib->block_length);

    uint32_t buffer_offset = 0;

    memcpy(buffer + buffer_offset,
           &(nib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(nib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(nib->navigation_information_time),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->ssp_longitude),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->ssp_latitude),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->earth_center_to_satellite),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->nadir_longitude),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->nadir_latitude),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->sun_position_x),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->sun_position_y),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->sun_position_z),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->moon_position_x),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->moon_position_y),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(nib->moon_position_z),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           nib->spare,
           nib->spare_length);
    buffer_offset += nib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * nib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_calibration_information_block(FILE*    fp,
                                          CIB*     cib,
                                          uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * cib->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(cib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(cib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(cib->band_number),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(cib->central_wave_length),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(cib->bits_per_pixel),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(cib->error_pixel_value),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(cib->outside_scan_pixel_value),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(cib->count_radiance_slope),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(cib->count_radiance_intercept),
           8);
    buffer_offset += 8;

    // If this is an IR band
    if(cib->band_number >=7 )
    {
        memcpy(buffer + buffer_offset,
               &(cib->ir_rad_to_bt_c0),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->ir_rad_to_bt_c1),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->ir_rad_to_bt_c2),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->ir_bt_to_rad_c0),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->ir_bt_to_rad_c1),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->ir_bt_to_rad_c2),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->ir_c),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->ir_h),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->ir_k),
               8);
        buffer_offset += 8;
    }
    else
    {
        memcpy(buffer + buffer_offset,
               &(cib->vis_nir_c_prime),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->vis_nir_calib_update_time),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->vis_nir_count_radiance_slope),
               8);
        buffer_offset += 8;

        memcpy(buffer + buffer_offset,
               &(cib->vis_nir_count_radiance_intercept),
               8);
        buffer_offset += 8;
    }

    memcpy(buffer + buffer_offset,
           cib->spare,
           cib->spare_length);
    buffer_offset += cib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * cib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_inter_calibration_information_block(FILE*    fp,
                                                IIB*     iib,
                                                uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * iib->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(iib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(iib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(iib->gsics_calibration_intercept),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(iib->gsics_calibration_slope),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(iib->gsics_calibration_quadratic),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(iib->radiance_bias),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(iib->radiance_bias_uncertainty),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(iib->radiance),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(iib->gsics_calibration_validity_start),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(iib->gsics_calibration_validity_end),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(iib->gsics_radiance_validity_upper_limit),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(iib->gsics_radiance_validity_lower_limit),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(iib->gsics_correction_filename),
           128);
    buffer_offset += 128;

    memcpy(buffer + buffer_offset,
           iib->spare,
           iib->spare_length);
    buffer_offset += iib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * iib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_segment_information_block(FILE*    fp,
                                      SIB*     sib,
                                      uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * sib->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(sib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(sib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(sib->total_segments),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(sib->segment_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(sib->segment_first_line_number),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           sib->spare,
           sib->spare_length);
    buffer_offset += sib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * sib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_navigation_correction_information_block(FILE*    fp,
                                                    NCIB*    ncib,
                                                    uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * ncib->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(ncib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(ncib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(ncib->center_column_of_rotation),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(ncib->center_line_of_rotation),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(ncib->rotational_correction),
           8);
    buffer_offset += 8;

    memcpy(buffer + buffer_offset,
           &(ncib->number_of_corrections),
           2);
    buffer_offset += 2;

    for(uint32_t i = 0; i < ncib->number_of_corrections; ++i)
    {
        memcpy(buffer + buffer_offset,
               ncib->line_number_after_rotation + i,
               2);
        buffer_offset += 2;

        memcpy(buffer + buffer_offset,
               ncib->shift_for_column_direction + i,
               4);
        buffer_offset += 4;

        memcpy(buffer + buffer_offset,
               ncib->shift_for_line_direction + i,
               4);
        buffer_offset += 4;
    }

    memcpy(buffer + buffer_offset,
           ncib->spare,
           ncib->spare_length);
    buffer_offset += ncib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * ncib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_observation_time_information_block(FILE*    fp,
                                               OTIB*    otib,
                                               uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * otib->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(otib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(otib->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           &(otib->number_of_observation_times),
           2);
    buffer_offset += 2;

    for(uint32_t i = 0; i < otib->number_of_observation_times; ++i)
    {
        memcpy(buffer + buffer_offset,
               otib->observation_time_line_number + i,
               2);
        buffer_offset += 2;

        memcpy(buffer + buffer_offset,
               otib->observation_time + i,
               8);
        buffer_offset += 8;
    }

    memcpy(buffer + buffer_offset,
           otib->spare,
           otib->spare_length);
    buffer_offset += otib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * otib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_error_information_block(FILE*    fp,
                                    EIB*     eib,
                                    uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * eib->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(eib->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(eib->block_length),
           4);
    buffer_offset += 4;

    memcpy(buffer + buffer_offset,
           &(eib->number_of_error_information_data),
           2);
    buffer_offset += 2;

    for(uint32_t i = 0; i < eib->number_of_error_information_data; ++i)
    {
        memcpy(buffer + buffer_offset,
               eib->error_line_number + i,
               2);
        buffer_offset += 2;

        memcpy(buffer + buffer_offset,
               eib->error_pixels_for_line + i,
               2);
        buffer_offset += 2;
    }

    memcpy(buffer + buffer_offset,
           eib->spare,
           eib->spare_length);
    buffer_offset += eib->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * eib->block_length,
           1,
           fp);
    free(buffer);
}



void _write_spare_block(FILE*    fp,
                        SB*      sb,
                        uint32_t header_offset)
{
    // Allocate a buffer to store data before writing to disk
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t) * sb->block_length);

    uint32_t buffer_offset = 0;
    memcpy(buffer + buffer_offset,
           &(sb->header_block_number),
           1);
    buffer_offset += 1;

    memcpy(buffer + buffer_offset,
           &(sb->block_length),
           2);
    buffer_offset += 2;

    memcpy(buffer + buffer_offset,
           sb->spare,
           sb->spare_length);
    buffer_offset += sb->spare_length;

    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(buffer,
           sizeof(uint8_t) * sb->block_length,
           1,
           fp);
    free(buffer);
}



void _write_data_block(FILE*    fp,
                       DB*      db,
                       uint32_t header_offset)
{
    fseek( fp,
           header_offset,
           SEEK_SET);
    fwrite(db->data,
           sizeof(uint16_t) * db->length,
           1,
           fp);
}



void _print_basic_information_block(BIB* bib)
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
           "    Spare length (bytes)   : %lu\n"
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
           bib->filename,
           sizeof(uint8_t) * bib->spare_length);
}



void _print_data_information_block(DIB* dib)
{
    printf("Data Information Block:\n\n"
           "    Block number          : %u\n"
           "    Block length (bytes)  : %u\n"
           "    Bits per pixel        : %u\n"
           "    Number of columns (x) : %u\n"
           "    Number of rows    (y) : %u\n"
           "    Compression flag      : %u\n"
           "    Spare length (bytes)  : %lu\n"
           "\n",
           dib->header_block_number,
           dib->block_length,
           dib->bits_per_pixel,
           dib->number_of_columns,
           dib->number_of_rows,
           dib->compression_flag,
           sizeof(uint8_t) * dib->spare_length);
}



void _print_projection_information_block(PIB* pib)
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
           "    Spare length (bytes)  : %lu\n"
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
           pib->resampling_size,
           sizeof(uint8_t) * pib->spare_length);
}



void _print_navigation_information_block(NIB* nib)
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
           "    Spare length (bytes)           : %lu\n"
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
           nib->moon_position_z,
           sizeof(uint8_t) * nib->spare_length);
}



void _print_calibration_information_block(CIB* cib)
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
               "    Speed of light (c)          : %f\n"
               "    Planck constant (h)         : %f\n"
               "    Boltzmann constant (k)      : %f\n"
               "    Spare length (bytes)        : %lu\n"
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
               cib->ir_bt_to_rad_c2,
               cib->ir_c,
               cib->ir_h,
               cib->ir_k,
               sizeof(uint8_t) * cib->spare_length);
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
               "    Spare length (bytes)                       : %lu\n"
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
               cib->vis_nir_count_radiance_intercept,
               sizeof(uint8_t) * cib->spare_length);
    }
}



void _print_inter_calibration_information_block(IIB* iib)
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
           "    Spare length (bytes)                        : %lu\n"
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
           iib->gsics_correction_filename,
           sizeof(uint8_t) * iib->spare_length);
}



void _print_segment_information_block(SIB* sib)
{
    printf("Segment Information Block:\n\n"
           "    Block number                 : %u\n"
           "    Block length (bytes)         : %u\n"
           "    Total segments               : %u\n"
           "    Segment number               : %u\n"
           "    First line number of segment : %u\n"
           "    Spare length (bytes)         : %lu\n"
           "\n",
           sib->header_block_number,
           sib->block_length,
           sib->total_segments,
           sib->segment_number,
           sib->segment_first_line_number,
           sizeof(uint8_t) * sib->spare_length);
}



void _print_navigation_correction_information_block(NCIB* ncib)
{
    if(ncib->number_of_corrections > 0)
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
               "    Spare length (bytes)           : %lu\n"
               "\n",
               ncib->header_block_number,
               ncib->block_length,
               ncib->center_column_of_rotation,
               ncib->center_line_of_rotation,
               ncib->rotational_correction,
               ncib->number_of_corrections,
               buffer,
               sizeof(uint8_t) * ncib->spare_length);

        free(buffer);
    }
    else
    {
        printf("Navigation Correction Information Block:\n\n"
               "    Block number                   : %u\n"
               "    Block length (bytes)           : %u\n"
               "    Center column of rotation      : %f\n"
               "    Center line of rotation        : %f\n"
               "    Rotational correction (urad)   : %f\n"
               "    Number of corrections          : %u\n\n"
               "    Spare length (bytes)           : %lu\n"
               "\n",
               ncib->header_block_number,
               ncib->block_length,
               ncib->center_column_of_rotation,
               ncib->center_line_of_rotation,
               ncib->rotational_correction,
               ncib->number_of_corrections,
               sizeof(uint8_t) * ncib->spare_length);
    }
}



void _print_observation_time_information_block(OTIB* otib)
{
    if(otib->number_of_observation_times > 0)
    {
        const uint32_t buffer_length = 256 * otib->number_of_observation_times;
        char*          buffer        = (char*)malloc(sizeof(char) * buffer_length);
        uint32_t       written_chars = 0;

        for(uint32_t i = 0; i < otib->number_of_observation_times; ++i)
        {
            written_chars += snprintf(buffer + written_chars,
                                      buffer_length - written_chars,
                                      "      Observation time %u:\n"
                                      "        Line number             : %u\n"
                                      "        Observation time (mjd)  : %f\n\n",
                                      i + 1,
                                      otib->observation_time_line_number[i],
                                      otib->observation_time[i]);
        }

        printf("Observation Time Information Block:\n\n"
               "    Block number                : %u\n"
               "    Block length (bytes)        : %u\n"
               "    Number of observation times : %u\n\n"
               "%s\n"
               "    Spare length (bytes)        : %lu\n"
               "\n",
               otib->header_block_number,
               otib->block_length,
               otib->number_of_observation_times,
               buffer,
               sizeof(uint8_t) * otib->spare_length);

        free(buffer);
    }
    else
    {
        printf("Observation Time Information Block:\n\n"
               "    Block number                : %u\n"
               "    Block length (bytes)        : %u\n"
               "    Number of observation times : %u\n\n"
               "    Spare length (bytes)        : %lu\n"
               "\n",
               otib->header_block_number,
               otib->block_length,
               otib->number_of_observation_times,
               sizeof(uint8_t) * otib->spare_length);
    }
}



void _print_error_information_block(EIB* eib)
{
    if(eib->number_of_error_information_data > 0)
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
               "    Block number                : %u\n"
               "    Block length (bytes)        : %u\n"
               "    Number of error information : %u\n"
               "%s\n"
               "    Spare length (bytes)        : %lu\n"
               "\n",
               eib->header_block_number,
               eib->block_length,
               eib->number_of_error_information_data,
               buffer,
               sizeof(uint8_t) * eib->spare_length);

        free(buffer);
    }
    else
    {
        printf("Error Information Block:\n\n"
               "    Block number                : %u\n"
               "    Block length (bytes)        : %u\n"
               "    Number of error information : %u\n"
               "    Spare length (bytes)        : %lu\n"
               "\n",
               eib->header_block_number,
               eib->block_length,
               eib->number_of_error_information_data,
               sizeof(uint8_t) * eib->spare_length);

    }
}



void _print_spare_block(SB* sb)
{
    printf("Spare Block:\n\n"
           "    Block number         : %u\n"
           "    Block length (bytes) : %u\n"
           "    Spare length (bytes) : %lu\n"
           "\n",
           sb->header_block_number,
           sb->block_length,
           sizeof(uint8_t) * sb->spare_length);
}



void _print_data_block(DB* db)
{
    printf("Data Block:\n\n"
           "    Block number         : 12\n"
           "    Block length (bytes) : %lu\n"
           "\n",
           sizeof(uint16_t) * db->length);
}



// Public functions
// ----------------------------------------



HSD* read_file(const char* filepath,
               bool        read_data)
{
    FILE*    fp                = NULL;
    uint8_t* decompress_buffer = NULL;

    // Is this a bz2 file?
    size_t filepath_length = strlen(filepath);
    char   filepath_ext[4];
    memcpy(filepath_ext,
           filepath + filepath_length - 3,
           3);
    filepath_ext[3] = '\0';

    // If this is a bz2 file, decompress before use
    // Otherwise just treat as a normal file
    bool is_bz2_file = strcmp(filepath_ext, "bz2") == 0;
    if(is_bz2_file) 
    {
        fp = fopen(filepath,
                   "rb");
        if(fp == NULL)
        {
            fprintf(stderr,
                    "%s:%d: Failed to open file %s\n",
                    __FILE__,
                    __LINE__,
                    filepath);
            exit(1);
        }

        uint64_t decompress_buffer_length;
        decompress_buffer = _decompress_bz2_file(fp,
                                                 &decompress_buffer_length);
                                                 
        // Close the file pointer to the bz2 file
        fclose(fp);

        // Pretend the decompressed bz2 data is a file, so we have one set of function calls
        // that operate on FILE*
        fp = fmemopen(decompress_buffer,
                      decompress_buffer_length,
                      "rb");
        if(fp == NULL)
        {
            fprintf(stderr,
                    "%s:%d: fmemopen returned NULL\n",
                    __FILE__,
                    __LINE__);
            exit(1);
        }
    }
    else
    {
        fp = fopen(filepath,
                   "rb");
        if(fp == NULL)
        {
            fprintf(stderr,
                    "%s:%d: Failed to open file %s\n",
                    __FILE__,
                    __LINE__,
                    filepath);
            exit(1);
        }
    }


    HSD*     hsd          = _allocate_hsd();
    uint32_t block_offset = 0;

    _read_basic_information_block(fp,
                                  hsd->bib,
                                  block_offset);
    block_offset += hsd->bib->block_length;

    _read_data_information_block(fp,
                                 hsd->dib,
                                 block_offset);
    block_offset += hsd->dib->block_length;

    _read_projection_information_block(fp,
                                       hsd->pib,
                                       block_offset);
    block_offset += hsd->pib->block_length;

    _read_navigation_information_block(fp,
                                       hsd->nib,
                                       block_offset);
    block_offset += hsd->nib->block_length;

    _read_calibration_information_block(fp,
                                        hsd->cib,
                                        block_offset);
    block_offset += hsd->cib->block_length;

    _read_inter_calibration_information_block(fp,
                                              hsd->iib,
                                              block_offset);
    block_offset += hsd->iib->block_length;
    
    _read_segment_information_block(fp,
                                    hsd->sib,
                                    block_offset);
    block_offset += hsd->sib->block_length;

    _read_navigation_correction_information_block(fp,
                                                  hsd->ncib,
                                                  block_offset);
    block_offset += hsd->ncib->block_length;

    _read_observation_time_information_block(fp,
                                             hsd->otib,
                                             block_offset);
    block_offset += hsd->otib->block_length;
    
    _read_error_information_block(fp,
                                  hsd->eib,
                                  block_offset);
    block_offset += hsd->eib->block_length;

    _read_spare_block(fp,
                      hsd->sb,
                      block_offset);
    block_offset += hsd->sb->block_length;

    if(read_data)
    {
        _read_data_block(fp,
                         hsd->db,
                         block_offset,
                         hsd->dib->number_of_columns * hsd->dib->number_of_rows);
    }

    fclose(fp);
    if(is_bz2_file)
        free(decompress_buffer);

    return hsd;
}



void write_file(const char* filepath,
                HSD*        hsd)
{
    FILE* fp = fopen(filepath,
                     "wb");
    if(fp == NULL)
    {
        fprintf(stderr,
                "%s:%d Failed to open %s for writing.\n",
                __FILE__,
                __LINE__,
                filepath);
        exit(1);
    }

    uint32_t header_offset = 0;
    _write_basic_information_block(fp,
                                   hsd->bib,
                                   header_offset);
    header_offset += hsd->bib->block_length;

    _write_data_information_block(fp,
                                  hsd->dib,
                                  header_offset);
    header_offset += hsd->dib->block_length;

    _write_projection_information_block(fp,
                                        hsd->pib,
                                        header_offset);
    header_offset += hsd->pib->block_length;

    _write_navigation_information_block(fp,
                                        hsd->nib,
                                        header_offset);
    header_offset += hsd->nib->block_length;

    _write_calibration_information_block(fp,
                                         hsd->cib,
                                         header_offset);
    header_offset += hsd->cib->block_length;

    _write_inter_calibration_information_block(fp,
                                               hsd->iib,
                                               header_offset);
    header_offset += hsd->iib->block_length;

    _write_segment_information_block(fp,
                                     hsd->sib,
                                     header_offset);
    header_offset += hsd->sib->block_length;

    _write_navigation_correction_information_block(fp,
                                                   hsd->ncib,
                                                   header_offset);
    header_offset += hsd->ncib->block_length;

    _write_observation_time_information_block(fp,
                                              hsd->otib,
                                              header_offset);
    header_offset += hsd->otib->block_length;

    _write_error_information_block(fp,
                                   hsd->eib,
                                   header_offset);
    header_offset += hsd->eib->block_length;

    _write_spare_block(fp,
                       hsd->sb,
                       header_offset);
    header_offset += hsd->sb->block_length;

    _write_data_block(fp,
                      hsd->db,
                      header_offset);

    fclose(fp);
}



void free_hsd(HSD* hsd)
{
    _deallocate_basic_information_block(hsd->bib);
    _deallocate_data_information_block(hsd->dib);
    _deallocate_projection_information_block(hsd->pib);
    _deallocate_navigation_information_block(hsd->nib);
    _deallocate_calibration_information_block(hsd->cib);
    _deallocate_inter_calibration_information_block(hsd->iib);
    _deallocate_segment_information_block(hsd->sib);
    _deallocate_navigation_correction_information_block(hsd->ncib);
    _deallocate_observation_time_information_block(hsd->otib);
    _deallocate_error_information_block(hsd->eib);
    _deallocate_spare_block(hsd->sb);
    _deallocate_data_block(hsd->db);
    free(hsd);
}



int compare_files(const char* file_1,
                  const char* file_2)
{
    FILE* fp_1 = fopen(file_1,
                       "rb");
    FILE* fp_2 = fopen(file_2,
                       "rb");
    if(fp_1 == NULL)
    {
        fprintf(stderr,
                "%s:%d Failed to open %s for reading.\n",
                __FILE__,
                __LINE__,
                file_1);
        exit(1);
    }
    if(fp_2 == NULL)
    {
        fprintf(stderr,
                "%s:%d Failed to open %s for reading.\n",
                __FILE__,
                __LINE__,
                file_2);
        exit(1);
    }

    struct stat st_1;
    struct stat st_2;
    stat(file_1, &st_1);
    stat(file_2, &st_2);

    if(st_1.st_size != st_2.st_size)
    {
        fprintf(stderr,
                "Filesizes do not match\n"
                "%s: %ld bytes\n"
                "%s: %ld bytes\n",
                file_1,
                st_1.st_size,
                file_2,
                st_2.st_size);
        return -1;
    }

    uint8_t* fp_1_buffer = (uint8_t*)malloc(sizeof(uint8_t) * st_1.st_size);
    uint8_t* fp_2_buffer = (uint8_t*)malloc(sizeof(uint8_t) * st_1.st_size);
    fread(fp_1_buffer,
          sizeof(uint8_t) * st_1.st_size,
          1,
          fp_1);
    fread(fp_2_buffer,
          sizeof(uint8_t) * st_1.st_size,
          1,
          fp_2);

    // Compare the memory
    int compare_result = memcmp(fp_1_buffer,
                                fp_2_buffer,
                                st_1.st_size);

    fclose(fp_1);
    fclose(fp_2);
    free(fp_1_buffer);
    free(fp_2_buffer);

    return compare_result;
}



void print_header(HSD* hsd)
{
    _print_basic_information_block(hsd->bib);
    _print_data_information_block(hsd->dib);
    _print_projection_information_block(hsd->pib);
    _print_navigation_information_block(hsd->nib);
    _print_calibration_information_block(hsd->cib);
    _print_inter_calibration_information_block(hsd->iib);
    _print_segment_information_block(hsd->sib);
    _print_navigation_correction_information_block(hsd->ncib);
    _print_observation_time_information_block(hsd->otib);
    _print_error_information_block(hsd->eib);
    _print_spare_block(hsd->sb);
    _print_data_block(hsd->db);
}



