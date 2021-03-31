COMPILER ?= $(GCC_PATH)gcc

FLAGS ?= -O2 -Wall -Wno-variadic-macros -pedantic -g $(GCC_SUPPFLAGS)

LDFLAGS ?= -g 
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
	rm -f obj/*.o
	rm ${EXECUTABLE} 
