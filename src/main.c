#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "hsd2netcdf.h"


// TODO:
// Test with each HSD version, both VIS/NIR and IR band
// Read in HSD, then write out that HSD again. Should be byte to byte identical
int main(int argc, char** argv)
{
    HSD* hsd = allocate_hsd();

    /*
    read_file("../test_data/HS_H08_20170623_0250_B01_FLDK_R10_S0110.DAT",
              hsd,
              allocate_data_p);
    */
    read_file("../test_data/HS_H08_20210603_0300_B13_FLDK_R20_S0110.DAT",
              hsd);

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

    // TODO: Free structs

    return 0;
}


