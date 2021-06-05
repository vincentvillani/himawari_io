#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "hsd2netcdf.h"


int main(int argc, char** argv)
{
    bool allocate_data_p = false;

    HSD* hsd = allocate_hsd(allocate_data_p);
    read_file("../test_data/HS_H08_20170623_0250_B01_FLDK_R10_S0110.DAT",
              hsd,
              allocate_data_p);
    print_basic_information_block(hsd->bib);
    print_data_information_block(hsd->dib);
    print_projection_information_block(hsd->pib);
    print_navigation_information_block(hsd->nib);
    print_calibration_information_block(hsd->cib);

    // TODO: Free structs

    return 0;
}


