import sys
sys.path.append("../bin")

from hio import lib, ffi
import numpy as np
import matplotlib.pyplot as plt

# Load the provided input file and print the header contents
input_filepath = sys.argv[1]
input_filepath = input_filepath.encode("utf-8")
hsd            = lib.himawari_io_read_file(input_filepath, True)
lib.himawari_io_print_header(hsd)

# Load the digital counts and convert to a numpy array
db             = hsd.db
digital_counts = ffi.unpack(db.data, db.length)
data           = np.array(digital_counts,
                          dtype=np.uint16).reshape(hsd.dib.number_of_rows,
                                                   hsd.dib.number_of_columns)
#print(data.shape, data.dtype)

# Plot 
plt.imshow(data, vmin=0, vmax=2000, cmap="gray")
plt.show()
