#ifndef HSD2NETCDF_H
#define HSD2NETCDF_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>



// Basic information block
typedef struct BIB
{
    uint8_t   header_block_number;       // 1 (fixed value)
    uint16_t  block_length;              // 282 bytes (fixed value)
    uint16_t  total_header_blocks;       // 11 (fixed value)
    uint8_t   byte_order;                // 0 = little endian, 1 = big endian
    char      satellite_name[16];    
    char      processing_center[16];
    char      observation_area[5];       // Note: extra byte required, they didn't leave space
                                         // for a null char.
    char      other_observation_info[3]; // Note: extra byte required, they didn't leave space
                                         // for a null char. Processing center use only
    uint16_t  observation_timeline;      // hhmm
    double    observation_start_time;    // mjd
    double    observation_end_time;      // mjd
    double    file_creation_time;        // mjd
    uint32_t  total_header_length;
    uint32_t  total_data_length;
    uint8_t   quality_flag_1;
    uint8_t   quality_flag_2;
    uint8_t   quality_flag_3;
    uint8_t   quality_flag_4;
    char      file_format_version[32];
    char      filename[128];
    uint8_t*  spare;
    uint32_t  spare_length;
}BIB;



// Data information block
typedef struct DIB
{
    uint8_t   header_block_number; // 2 (fixed value)
    uint16_t  block_length;        // 50 bytes (fixed value)
    uint16_t  bits_per_pixel;      // 16 (fixed value)
    uint16_t  number_of_columns;   // x
    uint16_t  number_of_rows;      // y
    uint8_t   compression_flag;    // 0 = no compression, 1 = gzip, 2 = bzip2
    uint8_t*  spare;
    uint32_t  spare_length;
}DIB;



// Projection information block
typedef struct PIB
{
    uint8_t   header_block_number; // 3 (fixed value)
    uint16_t  block_length;        // 127 bytes (fixed value)
    double    sub_lon;             // 140.7 degrees
    uint32_t  cfac;                // Column scaling factor
    uint32_t  lfac;                // Line scaling factor
    float     coff;                // Column offset
    float     loff;                // Line offset
    double    Rs;                  // Distance from earth's center to virtual satellite (Rs)
                                   // 42164 km (fixed value)
    double    Req;                 // Earth's equatorial radius (Req)
                                   // 6378.1370 km (fixed value)
    double    Rpol;                // Earth's polar radis (Rpol)
                                   // 6356.7523 km (fixed value)
    double    R1;                  // (Req^2 - Rpol^2) / Req^2
                                   // 0.00669438444 (fixed value)
    double    R2;                  // Rpol^2 / Req^2
                                   // 0.993305616 (fixed value)
    double    R3;                  // Req^2 / Rpol^2
                                   // 1.006739501 (fixed value)
    double    Sd_coefficient;      // Sd(Rs^2 - Req^2)
                                   // 1737122264 (fixed value)
    uint16_t  resampling_types;    // Resampling types
    uint16_t  resampling_size;     // Resampling size
    uint8_t*  spare;
    uint32_t  spare_length;
}PIB;



// Navigation information block
typedef struct NIB
{
    uint8_t   header_block_number;         // 4 (fixed value)
    uint16_t  block_length;                // 139 bytes (fixed value)
    double    navigation_information_time; // mjd
    double    ssp_longitude;               // sub-satellite point longitude (degrees)
    double    ssp_latitude;                // sub-satellite point latitude (degrees)
    double    earth_center_to_satellite;   // Distance from earth's centre to the satellite (km)
    double    nadir_longitude;             // Degrees
    double    nadir_latitude;              // Degrees
    double    sun_position_x;              // km (x, y, z) (J2000 inertial coordinate)
    double    sun_position_y;              
    double    sun_position_z;              
    double    moon_position_x;             // km (x, y, z) (J2000 inertial coordinate)
    double    moon_position_y;             
    double    moon_position_z;             
    uint8_t*  spare;
    uint32_t  spare_length;
}NIB;



// Calibration Information Block
typedef struct CIB
{
    uint8_t  header_block_number;      // 5 (fixed value)
    uint16_t block_length;             // 147 bytes (fixed value)
    uint16_t band_number;              // [1-16]
    double   central_wave_length;      // um (fixed for each band)
    uint16_t bits_per_pixel;           // 11, 12 or 14 (band dependent)
    uint16_t error_pixel_value;        // 65535 (fixed value)
    uint16_t outside_scan_pixel_value; // 65534 (fixed value)
    double   count_radiance_slope;
    double   count_radiance_intercept;
    double   ir_rad_to_bt_c0;
    double   ir_rad_to_bt_c1;
    double   ir_rad_to_bt_c2;
    double   ir_bt_to_rad_c0;
    double   ir_bt_to_rad_c1;
    double   ir_bt_to_rad_c2;
    double   vis_nir_c_prime;
    double   vis_nir_calib_update_time;        // Update time of the following two 
                                               // calibration values (mjd)
    double   vis_nir_count_radiance_slope;
    double   vis_nir_count_radiance_intercept;
    uint8_t  spare[80];
}CIB;



// Inter-calibration Information Block
typedef struct IIB
{
    uint8_t  header_block_number;      // 6 (fixed value)
    uint16_t block_length;             // 259 bytes (fixed value)
    double   gsics_calibration_intercept;
    double   gsics_calibration_slope;
    double   gsics_calibration_quadratic;
    double   radiance_bias;                    // For standard scene
    double   radiance_bias_uncertainty;        // For standard scene
    double   radiance;                         // For standard scene
    double   gsics_calibration_validity_start; // MJD
    double   gsics_calibration_validity_end;   // MJD
    float    gsics_radiance_validity_upper_limit;
    float    gsics_radiance_validity_lower_limit;
    char     gsics_correction_filename[128];
    uint8_t  spare[56];
}IIB;



// Segment information block
typedef struct SIB
{
    uint8_t  header_block_number;       // 7 (fixed value)
    uint16_t block_length;              // 47 bytes (fixed value)
    uint8_t  total_segments;            // Total number of segments for this band/obs type
    uint8_t  segment_number;            // Segment number out of total segments
    uint16_t segment_first_line_number; // First line number of sequence
    uint8_t  spare[40];
}SIB;


// Navigation correction information block
typedef struct NCIB
{
    uint8_t   header_block_number;       // 8 (fixed value)
    uint16_t  block_length;              // bytes
    float     center_column_of_rotation; 
    float     center_line_of_rotation;
    double    rotational_correction;     // urad
    uint16_t  number_of_corrections;      
    uint16_t* line_number_after_rotation;
    float*    shift_for_column_direction;
    float*    shift_for_line_direction;
    uint8_t   spare[40];
}NCIB;



// Observation time information block
typedef struct OTIB
{
    uint8_t   header_block_number;          // 9 (fixed value)
    uint16_t  block_length;                 // bytes
    uint16_t  number_of_observation_times;  // AKA scanline times
    uint16_t* observation_time_line_number; // The line number the observation time is for
    double*   observation_time;             // MJD
    uint8_t   spare[40];

}OTIB;



// Error information block
typedef struct EIB
{
    uint8_t   header_block_number;              // 10 (fixed value)
    uint32_t  block_length;                     // bytes NOTE: uint32_t instead of uint16_t
    uint16_t  number_of_error_information_data;
    uint16_t* error_line_number;
    uint16_t* error_pixels_for_line;
    uint8_t   spare[40];

}EIB;



// Spare block
typedef struct SB
{

    uint8_t  header_block_number; // 11 (fixed value)
    uint16_t block_length;        // 259 bytes (fixed value)
    uint8_t  spare[256];
}SB;



// Data block
typedef struct DB
{
    uint16_t* data; 
    uint32_t  length; // Length = DIB->number_of_columns * DIB->number_of_rows  
}DB;



// A whole HSD file
typedef struct HSD
{
    BIB*  bib;
    DIB*  dib;
    PIB*  pib;
    NIB*  nib;
    CIB*  cib;
    IIB*  iib;
    SIB*  sib;
    NCIB* ncib;
    OTIB* otib;
    EIB*  eib;
    SB*   sb;
    DB*   db;
}HSD;



BIB* allocate_basic_information_block();
void deallocate_basic_information_block(BIB* bib);
void read_basic_information_block(FILE*    fp,
                                  BIB*     bib,
                                  uint32_t header_offset);
void print_basic_information_block(BIB* bib);



DIB* allocate_data_information_block();
void deallocate_data_information_block(DIB* dib);
void read_data_information_block(FILE*    fp,
                                 DIB*     dib,
                                 uint32_t header_offset);
void print_data_information_block(DIB* dib);



PIB* allocate_projection_information_block();
void deallocate_projection_information_block(PIB* pib);
void read_projection_information_block(FILE*    fp,
                                       PIB*     pib,
                                       uint32_t header_offset);
void print_projection_information_block(PIB* pib);



NIB* allocate_navigation_information_block();
void deallocate_navigation_information_block(NIB* nib);
void read_navigation_information_block(FILE*    fp,
                                       NIB*     nib,
                                       uint32_t header_offset);
void print_navigation_information_block(NIB* nib);



CIB* allocate_calibration_information_block();
void deallocate_calibration_information_block(CIB* cib);
void read_calibration_information_block(FILE*    fp,
                                        CIB*     cib,
                                        uint32_t header_offset);
void print_calibration_information_block(CIB* cib);



IIB* allocate_inter_calibration_information_block();
void deallocate_inter_calibration_information_block(IIB* iib);
void read_inter_calibration_information_block(FILE*    fp,
                                              IIB*     iib,
                                              uint32_t header_offset);
void print_inter_calibration_information_block(IIB* iib);



SIB* allocate_segment_information_block();
void deallocate_segment_information_block(SIB* sib);
void read_segment_information_block(FILE*    fp,
                                    SIB*     sib,
                                    uint32_t header_offset);
void print_segment_information_block(SIB* sib);



NCIB* allocate_navigation_correction_information_block();
void deallocate_navigation_correction_information_block(NCIB* ncib);
void read_navigation_correction_information_block(FILE*    fp,
                                                  NCIB*    ncib,
                                                  uint32_t header_offset);
void print_navigation_correction_information_block(NCIB* ncib);



OTIB* allocate_observation_time_information_block();
void deallocate_observation_time_information_block(OTIB* otib);
void read_observation_time_information_block(FILE*    fp,
                                             OTIB*    otib,
                                             uint32_t header_offset);
void print_observation_time_information_block(OTIB* otib);



EIB* allocate_error_information_block();
void deallocate_error_information_block(EIB* eib);
void read_error_information_block(FILE*    fp,
                                  EIB*     eib,
                                  uint32_t header_offset);
void print_error_information_block(EIB* eib);



SB* allocate_spare_block();
void deallocate_spare_block(SB* sb);
void read_spare_block(FILE*    fp,
                      SB*      sb,
                      uint32_t header_offset);
void print_spare_block(SB* sb);



DB* allocate_data_block();
void deallocate_data_block(DB* db);
void read_data_block(FILE*    fp,
                     DB*      db,
                     uint32_t header_offset,
                     uint32_t length);
void print_data_block(DB* db);



HSD* allocate_hsd();
void read_file(const char* filepath,
               HSD*        hsd);
void write_file(const char* filepath,
                HSD*        hsd);
void deallocate_hsd(HSD* hsd);
void print_header(HSD* hsd);



#endif
