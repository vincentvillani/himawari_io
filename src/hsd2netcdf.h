#ifndef HSD2NETCDF_H
#define HSD2NETCDF_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct BIB
{
    uint8_t  header_block_number;       // 1 (fixed value)
    uint16_t block_length;              // 282 bytes (fixed value)
    uint16_t total_header_blocks;       // 11 (fixed value)
    uint8_t  byte_order;                // 0 = little endian, 1 = big endian
    char     satellite_name[16];    
    char     processing_center[16];
    char     observation_area[5];       // Note: extra byte required, they didn't leave space
                                        // for a null char.
    char     other_observation_info[3]; // Note: extra byte required, they didn't leave space
                                        // for a null char. Processing center use only
    uint16_t observation_timeline;      // hhmm
    double   observation_start_time;    // mjd
    double   observation_end_time;      // mjd
    double   file_creation_time;        // mjd
    uint32_t total_header_length;
    uint32_t total_data_length;
    uint8_t  quality_flag_1;
    uint8_t  quality_flag_2;
    uint8_t  quality_flag_3;
    uint8_t  quality_flag_4;
    char     file_format_version[32];
    char     filename[32];
    uint8_t  spare[40];

    uint8_t* data_p;
    uint64_t data_length;

}BIB;



typedef struct HSD
{
    BIB* bib;

}HSD;


BIB* allocate_basic_information_block(bool allocate_data_p);
void deallocate_basic_information_block(BIB* bib);
void read_basic_information_block(FILE* fp, BIB* bib, bool fill_data_p);
void print_basic_information_block(BIB* bib);

HSD* allocate_hsd(bool allocate_data_p);


void read_file(const char* filepath, HSD* hsd, bool fill_data_p);


#endif
