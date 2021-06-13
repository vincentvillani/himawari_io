#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "himawari_io.h"


// TODO:
// Test with each HSD version, both VIS/NIR and IR band
// Read in HSD, then write out that HSD again. Should be byte to byte identical
// Add overwrite flag
int main(int argc, char** argv)
{
    // Allocate
    HSD* hsd = allocate_hsd();

    // Read file
    //read_file("../test_data/HS_H08_20210603_0300_B01_FLDK_R10_S0110.DAT",
    //          hsd);
    read_file("../test_data/HS_H08_20210603_0300_B07_FLDK_R20_S0110.DAT",
              hsd);
    //read_file("../test_data/HS_H08_20210603_0300_B13_FLDK_R20_S0110.DAT",
    //          hsd);

    // Print header information
    print_header(hsd);

    // Deallocate
    deallocate_hsd(hsd);

    return 0;
}


