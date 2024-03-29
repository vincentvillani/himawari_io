#ifndef HIMAWARI_IO_H 
#define HIMAWARI_IO_H 

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdlib.h>
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
    uint8_t   header_block_number;      // 5 (fixed value)
    uint16_t  block_length;             // 147 bytes (fixed value)
    uint16_t  band_number;              // [1-16]
    double    central_wave_length;      // um (fixed for each band)
    uint16_t  bits_per_pixel;           // 11, 12 or 14 (band dependent)
    uint16_t  error_pixel_value;        // 65535 (fixed value)
    uint16_t  outside_scan_pixel_value; // 65534 (fixed value)
    double    count_radiance_slope;
    double    count_radiance_intercept;
    double    ir_rad_to_bt_c0;
    double    ir_rad_to_bt_c1;
    double    ir_rad_to_bt_c2;
    double    ir_bt_to_rad_c0;
    double    ir_bt_to_rad_c1;
    double    ir_bt_to_rad_c2;
    double    ir_c;                             // Speed of light (m/s)
    double    ir_h;                             // Planck constant (Js)
    double    ir_k;                             // Boltzmann constant (J/K)
    double    vis_nir_c_prime;
    double    vis_nir_calib_update_time;        // Update time of the following two 
                                                // calibration values (mjd)
    double    vis_nir_count_radiance_slope;
    double    vis_nir_count_radiance_intercept;
    uint8_t*  spare;
    uint32_t  spare_length;
}CIB;



// Inter-calibration Information Block
typedef struct IIB
{
    uint8_t   header_block_number;      // 6 (fixed value)
    uint16_t  block_length;             // 259 bytes (fixed value)
    double    gsics_calibration_intercept;
    double    gsics_calibration_slope;
    double    gsics_calibration_quadratic;
    double    radiance_bias;                    // For standard scene
    double    radiance_bias_uncertainty;        // For standard scene
    double    radiance;                         // For standard scene
    double    gsics_calibration_validity_start; // MJD
    double    gsics_calibration_validity_end;   // MJD
    float     gsics_radiance_validity_upper_limit;
    float     gsics_radiance_validity_lower_limit;
    char      gsics_correction_filename[128];
    uint8_t*  spare;
    uint32_t  spare_length;
}IIB;



// Segment information block
typedef struct SIB
{
    uint8_t   header_block_number;       // 7 (fixed value)
    uint16_t  block_length;              // 47 bytes (fixed value)
    uint8_t   total_segments;            // Total number of segments for this band/obs type
    uint8_t   segment_number;            // Segment number out of total segments
    uint16_t  segment_first_line_number; // First line number of sequence
    uint8_t*  spare;
    uint32_t  spare_length;
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
    uint8_t*  spare;
    uint32_t  spare_length;
}NCIB;



// Observation time information block
typedef struct OTIB
{
    uint8_t   header_block_number;          // 9 (fixed value)
    uint16_t  block_length;                 // bytes
    uint16_t  number_of_observation_times;  // AKA scanline times
    uint16_t* observation_time_line_number; // The line number the observation time is for
    double*   observation_time;             // MJD
    uint8_t*  spare;
    uint32_t  spare_length;
}OTIB;



// Error information block
typedef struct EIB
{
    uint8_t   header_block_number;              // 10 (fixed value)
    uint32_t  block_length;                     // bytes NOTE: uint32_t instead of uint16_t
    uint16_t  number_of_error_information_data;
    uint16_t* error_line_number;
    uint16_t* error_pixels_for_line;
    uint8_t*  spare;
    uint32_t  spare_length;

}EIB;



// Spare block
typedef struct SB
{

    uint8_t   header_block_number; // 11 (fixed value)
    uint16_t  block_length;        // 259 bytes (fixed value)
    uint8_t*  spare;
    uint32_t  spare_length;
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



HSD* himawari_io_read_file(const char* filepath,
                           bool        read_data);
void himawari_io_write_file(const char* filepath,
                            HSD*        hsd);
void himawari_io_free_hsd(HSD* hsd);
int  himawari_io_compare_files(const char* file_1,
                               const char* file_2);
void himawari_io_print_header(HSD* hsd);


#ifdef __cplusplus
}
#endif

#endif
