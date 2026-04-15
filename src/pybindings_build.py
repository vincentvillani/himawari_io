import cffi

# https://realpython.com/python-bindings-overview/
ffi = cffi.FFI()

# Once you have the FFI, you’ll use .cdef() to process the contents of the header file automatically. This creates wrapper functions for you to marshal data from Python:
header_path = "himawari_io.h"
library_dir = "../bin/"

def_file = open("pybindings_defs.h", "r")
ffi.cdef(def_file.read())
def_file.close()

# Reading and processing the header file is the first step. After that, you need to use .set_source() to describe the source file that CFFI will generate:
ffi.set_source("hio",
               '''
               #include "himawari_io.h"
               ''',
               libraries=["himawari_io"],
               library_dirs=[library_dir],
               extra_link_args=["-Wl,-rpath,."])

# Actually perform the compile
ffi.compile()
