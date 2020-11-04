cc=gcc
cflags = -W -g
target = main

objects = main.o string_manipulation.o output.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

main.o string_manipulation.o output.o : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
