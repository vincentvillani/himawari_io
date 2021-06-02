# Gather all source files
# ------------------------------------------------------

hsd2nc4_src_c=$(wildcard *.c)



# Generate .o file filenames
# ------------------------------------------------------

hsd2nc4_obj=$(hsd2nc4_src_c:.c=.o)


# Flags
# ------------------------------------------------------

CC=gcc
LIBS=-lm -lpthread
C_FLAGS=-m64 -Wall -std=c11
DEBUG_FLAGS=-g3 -O0
RELEASE_FLAGS=-O3
INCLUDES=-I/usr/include/

# Default value of compile flags
COMPILE_FLAGS=$(C_FLAGS) $(DEBUG_FLAGS) $(INCLUDES)

# Depending on what the end goal is, change the compile flags
ifeq ($(MAKECMDGOALS),release)
COMPILE_FLAGS=$(C_FLAGS) $(RELEASE_FLAGS) $(INCLUDES)
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

%.o:%.c
	    $(CC) $(COMPILE_FLAGS) -c $< -o $@



hsd2nc4: $(hsd2nc4_obj)
	$(CC) -o ../bin/hsd2nc4 $(hsd2nc4_obj) $(LIBS)


release: hsd2nc4
debug: hsd2nc4


.PHONY: clean
clean:
	rm -f $(hsd2nc4_obj) \
          ../bin/hsd2nc4


