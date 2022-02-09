#includ global definitions:
include ../makefile.def

#include local definitions:
include makefile.def

#

.SUFFIXES: .cpp

.cpp.o:
	$(CC) $(CFLAGS) -c $<

#
all: ${ALL_OBJS}

obj: ${ALL_OBJS}

clean:
	rm -f *.o ${PROGS}
