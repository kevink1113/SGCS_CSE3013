C = gcc
target = animal
cflags = -W -g
objects = dog.o blackcow.o turtle.o main.o

$(target): $(objects)
	$(CC) -o $(target) $(objects)

%.o : %.c
	$(CC) $(cflags) -c -o $@ $<

.PHONY: clean
clean:
	rm $(target) $(objects)
