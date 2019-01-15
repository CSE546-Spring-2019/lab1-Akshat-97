WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) 

TESTFLAGS = 

SRCS = count.c
OBJS = $(SRCS:%.c=%.o)

count: $(OBJS) 
	$(GCC) $(TESTFALGS) $(OBJS) -o count

.c.o: 
	$(GCC) $(TESTFALGS) -c $*.c 

testall: test1

testadd: count
	test1

test1: count
	./count example1 the example1out

clean: # remove all machine generated files
	rm -f count *.o output? 
