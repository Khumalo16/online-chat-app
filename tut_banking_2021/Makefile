# sample Makefile.
# It compiles every .c files in the src/ directory to object files in obj/ directory, and builds the ./my_executable my_executable.

# using gcc :
COMPILER ?= $(GCC_PATH)gcc

FLAGS ?= -O2 -Wall -Wno-variadic-macros -pedantic -g  -DDEBUG -fopenmp $(GCC_SUPPFLAGS)
#FLAGS ?= -O2 -Wall -Wno-variadic-macros -pedantic -g -fopenmp $(GCC_SUPPFLAGS)

LDFLAGS ?= -g -fopenmp
LDLIBS = -lm

EXECUTABLE = my_executable

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:src/%.c=obj/%.o)

all: release

release: $(OBJS)
	$(COMPILER) $(LDFLAGS) -o $(EXECUTABLE) $(OBJS) $(LDLIBS) 

obj/%.o: src/%.c
	$(COMPILER) $(FLAGS) -o $@ -c $<


clean:
	rm -f obj/*
	rm -f ${EXECUTABLE} 
