cc = gcc
cflags = -W -g
target = main

objects = main.o calculate.o print.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

main.o calculate.o print.o : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
