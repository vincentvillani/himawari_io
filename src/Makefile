# Gather all source files
# ------------------------------------------------------

himawari_io_src_c=$(wildcard himawari*.c)



# Generate .o file filenames
# ------------------------------------------------------

himawari_io_obj=$(himawari_io_src_c:.c=.o)


# Flags
# ------------------------------------------------------

CC=gcc
LIBS=-lm -lpthread -lbz2
C_FLAGS=-m64 -Wall -std=c11 -fPIC
DEBUG_FLAGS=-g3 -O0
RELEASE_FLAGS=-O3
INCLUDES=-I/usr/include/

# Default value of compile flags
COMPILE_FLAGS=$(C_FLAGS) $(DEBUG_FLAGS) $(INCLUDES)

# Depending on what the end goal is, change the compile flags
ifeq ($(MAKECMDGOALS),release)
COMPILE_FLAGS=$(C_FLAGS) $(RELEASE_FLAGS) $(INCLUDES)
endif

ifeq ($(MAKECMDGOALS),debug)
COMPILE_FLAGS=$(C_FLAGS) $(DEBUG_FLAGS) $(INCLUDES)
endif


# Targets
# ------------------------------------------------------

# $@ = Target name
# # $^ = All dependencies
# # $< = First dependency
# # <target>: <dependencies>
#
# # Whenever there is a need to compile a .cpp file, do so with this wildcard rule
#

# Compiles himawari_io c files to object files
%.o:%.c
	    $(CC) $(COMPILE_FLAGS) -c $< -o $@


# Creates the shared library from the .o files
himawari_io_so: $(himawari_io_obj)
	$(CC) -shared -o ../bin/libhimawari_io.so $(himawari_io_obj) $(LIBS)


# Creates a test binary, linking in the .so file
himawari_io: himawari_io_so
	$(CC) -o ../bin/himawari_io main.c $(COMPILE_FLAGS) -L../bin/ -lhimawari_io


release: himawari_io
debug: himawari_io


.PHONY: clean
clean:
	rm -f $(himawari_io_obj)       \
		  main.o                   \
		  ../bin/himawari_io       \
	      ../bin/libhimawari_io.so


